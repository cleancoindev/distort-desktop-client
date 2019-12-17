#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "conversation.h"
#include "group.h"
#include "groupwidget.h"
#include "conversationwidget.h"
#include "restclient.h"
#include "signindialog.h"
#include "messagewidget.h"
#include "backgroundworker.h"
#include "atomicstate.h"
#include "inmessage.h"
#include "outmessage.h"

#include <deque>
#include <memory>
#include <QStandardItem>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Options.hpp>

namespace
{
    // TODO: DELETE THIS
    void patch_pushcstring(YASL_State* S, const char* cstr)
    {
        size_t len = strlen(cstr);
        char* c = (char*)malloc(len + 1);
        memcpy(c, cstr, len);
        c[len] = 0;

        YASL_pushszstring(S, c);
    }

    int addOrUpdateInMessage(YASL_State *S)
    {
        // From, verified, index, message, dateReceived, worker
        YASL_Object* w = YASL_popobject(S);
        MainWindow* mainWindow = reinterpret_cast<MainWindow*>(YASL_getuserpointer(w));

        YASL_Object* d = YASL_popobject(S);
        QString dateReceived = QString::fromUtf8(YASL_getstring(d), YASL_getstringlen(d));

        YASL_Object* m = YASL_popobject(S);
        QString message = QString::fromUtf8(YASL_getstring(m), YASL_getstringlen(m));

        YASL_Object* i = YASL_popobject(S);
        int64_t index = YASL_getinteger(i);

        YASL_Object* v = YASL_popobject(S);
        bool verified = YASL_getboolean(v);

        InMessage* im = new InMessage(verified, message, index, dateReceived);
        mainWindow->addOrUpdateMessage(im);

        YASL_pushundef(S);
        return YASL_SUCCESS;
    }
    int addOrUpdateOutMessage(YASL_State *S)
    {
        // Status, index, message, lastStatusChange, worker
        YASL_Object* w = YASL_popobject(S);
        MainWindow* mainWindow = reinterpret_cast<MainWindow*>(YASL_getuserpointer(w));

        YASL_Object* l = YASL_popobject(S);
        QString lastStatusChange = QString::fromUtf8(YASL_getstring(l), YASL_getstringlen(l));

        YASL_Object* m = YASL_popobject(S);
        QString message = QString::fromUtf8(YASL_getstring(m), YASL_getstringlen(m));

        YASL_Object* i = YASL_popobject(S);
        int64_t index = YASL_getinteger(i);

        YASL_Object* s = YASL_popobject(S);
        QString status = QString::fromUtf8(YASL_getstring(s), YASL_getstringlen(s));

        OutMessage* om = new OutMessage(status, message, index, lastStatusChange);
        mainWindow->addOrUpdateMessage(om);

        YASL_pushundef(S);
        return YASL_SUCCESS;
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    messageModel(nullptr),
    authParams(nullptr),
    aboutWindow(nullptr)
{
    ui->setupUi(this);
    setWindowTitle(QCoreApplication::applicationName());

    // Initialize ui elements for no account
    ui->actionSign_in->setVisible(true);
    ui->actionLogout->setVisible(false);
    ui->actionSettings->setVisible(false);
    ui->conversationsTree->setEnabled(false);
    toggleMessaging(false);

    // Setup background worker
    BackgroundWorker *worker = new BackgroundWorker;
    worker->moveToThread(&backgroundThread);
    connect(&backgroundThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &MainWindow::startBackground, worker, &BackgroundWorker::doWork);
    connect(worker, &BackgroundWorker::setActiveGroupUI, this, &MainWindow::setActiveGroup);
    connect(worker, &BackgroundWorker::setSubgroupIndexUI, this, &MainWindow::setSubgroupIndex);
    connect(worker, &BackgroundWorker::setConversationNameUI, this, &MainWindow::setConversationName);
    connect(worker, &BackgroundWorker::addGroupUI, this, &MainWindow::addGroup);
    connect(worker, &BackgroundWorker::addConvUI, this, &MainWindow::addConv);
    connect(worker, &BackgroundWorker::removeGroupUI, this, &MainWindow::removeGroup);
    connect(worker, &BackgroundWorker::removeConvUI, this, &MainWindow::removeConv);
    connect(worker, &BackgroundWorker::addOrUpdateMessageUI, this, &MainWindow::addOrUpdateMessage);
    backgroundThread.start();

    // Setup Messages
    ui->messagesList->setItemDelegate(new MessageDelegate);
    //ui->messagesList->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
    //ui->messagesList->setSelectionBehavior(QAbstractItemView::SelectRows);
    std::deque<std::shared_ptr<Message>> messages;
    messageModel = new MessageListModel(messages, this);
    ui->messagesList->setModel(messageModel);

    // Misc connections
    connect(ui->conversationsTree, &QTreeWidget::currentItemChanged, this, &MainWindow::onConversationSelected);
}

void MainWindow::closeEvent(QCloseEvent* closeEvent)
{
    // Close all open windows
    if(aboutWindow != nullptr) delete aboutWindow;

    closeEvent->accept();
}
void MainWindow::toggleMessaging(bool enable)
{
    ui->messagesList->setEnabled(enable);
    ui->messageEdit->setEnabled(enable);
    ui->sendButton->setEnabled(enable);
}
MainWindow::~MainWindow()
{
    // Close background thread
    backgroundThread.requestInterruption();
    AtomicState* state = AtomicState::getInstance();
    state->lockState();
    backgroundThread.quit();
    state->unlockState();
    backgroundThread.wait();

    // Free main window ui
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    if(aboutWindow == nullptr) {
        aboutWindow = new About;
        aboutWindow->setWindowFlags(Qt::Window);
        aboutWindow->setAttribute(Qt::WA_DeleteOnClose);
        aboutWindow->show();

        connect(aboutWindow, &About::destroyed, this, &MainWindow::on_actionAbout_destroyed);
    }
}
void MainWindow::on_actionAbout_destroyed(QObject*)
{
    aboutWindow = nullptr;
}
void MainWindow::on_actionSign_in_triggered()
{
    SignInDialog dialog(this);
    if(dialog.exec() == QDialog::Accepted)
    {
        // Get Account
        authParams = dialog.getAuthParams();

        // Unlock and populate UI
        ui->actionSign_in->setVisible(false);
        ui->actionLogout->setVisible(true);
        ui->actionSettings->setVisible(true);
        ui->conversationsTree->setEnabled(true);

        emit startBackground(authParams->getHomeserver(), authParams->getPeerId(), authParams->getAccount(),
                             authParams->getAuthToken(), ui->conversationsTree, ui->messagesList);
    }
}

void MainWindow::addOrUpdateMessage(Message* msg)
{
    std::shared_ptr<Message> msgPtr(msg);
    messageModel->addOrUpdateMessage(msgPtr);
}

void MainWindow::onConversationSelected(QTreeWidgetItem* item, QTreeWidgetItem* old)
{
    if(item->parent() != nullptr)
    {
        // Enable messaging windows
        toggleMessaging(true);

        // Clear current conversation
        messageModel->clearData();

        AtomicState* atomicState = AtomicState::getInstance();
        atomicState->lockState();
        YASL_State* S = atomicState->getState();

        // Pass self
        YASL_pushuserpointer(S, this);
        YASL_setglobal(S, "mainWindow");

        // Set current conversation
        ConversationWidget* cwgt = reinterpret_cast<ConversationWidget*>(ui->conversationsTree->itemWidget(item, 0));
        QString name = cwgt->getConversation().getPeer().getFriendlyName();
        patch_pushcstring(S, cwgt->getConversation().uniqueLabel().toUtf8());
        YASL_setglobal(S, "currentConv");

        // Pass addOrUpdateInMessage function
        //YASL_declglobal(S, "addOrUpdateInMessage");
        YASL_pushcfunction(S, addOrUpdateInMessage, 5);
        YASL_setglobal(S, "addOrUpdateInMessage");

        // Pass addOrUpdateOutMessage function
        //YASL_declglobal(S, "addOrUpdateOutMessage");
        YASL_pushcfunction(S, addOrUpdateOutMessage, 5);
        YASL_setglobal(S, "addOrUpdateOutMessage");

        YASL_resetstate(S, "scripts/switchconv.yasl");
        if(!S)
        {
            std::cerr << "scripts/switchconv.yasl: can't find it" << std::endl;
        }
        else if(int status = YASL_execute(S) != YASL_MODULE_SUCCESS)
        {
            std::cerr << "scripts/switchconv.yasl: module failed: " << status << std::endl;
        }
        else
        {
            YASL_Object* r = YASL_popobject(S);
            if(!YASL_getboolean(r))
            {
                std::cerr << "scripts/switchconv.yasl: returned failure" << std::endl;
            }
        }
        atomicState->unlockState();
    }
}

void MainWindow::setActiveGroup(QTreeWidgetItem* groupItem, bool active)
{
    reinterpret_cast<GroupWidget*>(ui->conversationsTree->itemWidget(groupItem, 0))->setActive(active);
}

void MainWindow::setSubgroupIndex(QTreeWidgetItem* groupItem, unsigned int index)
{
    reinterpret_cast<GroupWidget*>(ui->conversationsTree->itemWidget(groupItem, 0))->setSubgroupIndex(index);
}

void MainWindow::setConversationName(QTreeWidgetItem* convItem, QString name)
{
    reinterpret_cast<ConversationWidget*>(ui->conversationsTree->itemWidget(convItem, 0))->setName(name);
}

void MainWindow::addGroup(QString name, unsigned int index, QTreeWidgetItem* groupItem)
{
    Group g(name, index);
    GroupWidget* widget = new GroupWidget(g);
    ui->conversationsTree->addTopLevelItem(groupItem);
    ui->conversationsTree->setItemWidget(groupItem, 0, widget);
}

void MainWindow::addConv(QTreeWidgetItem* groupItem, QString peerId, QString account, QString nickname, unsigned int height, QTreeWidgetItem* convItem)
{
    QTreeWidget* convTree = groupItem->treeWidget();
    GroupWidget* gwgt = reinterpret_cast<GroupWidget*>(convTree->itemWidget(groupItem, 0));

    Conversation conv(gwgt->getGroup().getName(), Peer(peerId, account, nickname), height);
    ConversationWidget* widget = new ConversationWidget(conv);
    groupItem->addChild(convItem);
    ui->conversationsTree->setItemWidget(convItem, 0, widget);
}

void MainWindow::removeGroup(QTreeWidgetItem* groupItem)
{
    delete groupItem;
}

void MainWindow::removeConv(QTreeWidgetItem* convItem)
{
    delete convItem;
}
