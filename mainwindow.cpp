#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "conversation.h"
#include "group.h"
#include "groupwidget.h"
#include "conversationwidget.h"
#include "restclient.h"
#include "signindialog.h"
#include "messagewidget.h"

#include <deque>
#include <memory>
#include <QStandardItem>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Options.hpp>

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    messageModel(nullptr),
    authParams(nullptr)
{
    ui->setupUi(this);
    setWindowTitle(QCoreApplication::applicationName());

    // Initialize ui elements for no account
    ui->actionSign_in->setVisible(true);
    ui->actionLogout->setVisible(false);
    ui->actionSettings->setVisible(false);
    ui->conversationsTree->setEnabled(false);
    //ui->messagesList->setEnabled(false);
    ui->messageEdit->setEnabled(false);
    ui->sendButton->setEnabled(false);

    /*try
    {
        //RestClient::sendDistortRequest("GET", "http://ipfs.bootstrap.distort.network:6945/account", "QmWV77RTC7cwMPbPBfCbP68JGPt5ta8qPyiCWsUNZsXvpo", "root", "ElVd/8ULzqhzfMabtyMYVE6yI5g8D6/iAMQ1lEcWchk=");
        //RestClient::sendDistortRequest("GET", "http://ipfs.bootstrap.distort.network:6945/groups", "QmWV77RTC7cwMPbPBfCbP68JGPt5ta8qPyiCWsUNZsXvpo", "root", "ElVd/8ULzqhzfMabtyMYVE6yI5g8D6/iAMQ1lEcWchk=");
        //RestClient::sendDistortRequest("GET", "http://ipfs.bootstrap.distort.network:6945/groups/" + cURLpp::escape("パン"), "QmWV77RTC7cwMPbPBfCbP68JGPt5ta8qPyiCWsUNZsXvpo", "root", "ElVd/8ULzqhzfMabtyMYVE6yI5g8D6/iAMQ1lEcWchk=");

        RestClient::sendDistortGet("http://ipfs.bootstrap.distort.network:6945/groups/" + cURLpp::escape("パン"), "QmWV77RTC7cwMPbPBfCbP68JGPt5ta8qPyiCWsUNZsXvpo", "root", "ElVd/8ULzqhzfMabtyMYVE6yI5g8D6/iAMQ1lEcWchk=", RestClient::emptyParams);

        std::map<std::string, std::string> p;
        p.insert(std::pair<std::string, std::string>("peerId", "QmemJHsMDuBjUCAyiWeVdct2LGHqtZhu8QQCt8ZQVbY1qz"));
        RestClient::sendDistortAuthGet("groups/" + cURLpp::escape("パン") + "/0", authParams, p);
    }
    catch(curlpp::RuntimeError & e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch(curlpp::LogicError & e)
    {
        std::cerr << e.what() << std::endl;
    }*/

    // Setup Messages
    ui->messagesList->setItemDelegate(new MessageDelegate);
    ui->messagesList->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
    ui->messagesList->setSelectionBehavior(QAbstractItemView::SelectRows);
    std::deque<std::shared_ptr<Message>> messages;
    for(uint64_t i = 0; i < 2; i++)
    {
        std::shared_ptr<Message> m(new Message("hello", "hello to you good sir, the one I see before me on this splendiferous day. I see you are doing just fine over there", i));
        messages.push_back(m);
    }
    messageModel = new MessageListModel(messages, this);
    ui->messagesList->setModel(messageModel);

    std::shared_ptr<Message> m(new Message("hello", "hello to you 12345678901234567890123456789012345678901234567890123456789012345678901234567890"
                                                    "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
                                                    "1234567890abcdefghijklmnopqrstuvwxyz1234567890abcdefghijklmnopqrstuvwxyz", 2));
    std::shared_ptr<Message> m4(new Message("hello", "More tests X 4", 4));
    std::shared_ptr<Message> m3(new Message("hello", "More tests X 3", 3));
    messageModel->addOrUpdateMessage(m);
    messageModel->addOrUpdateMessage(m4);
    messageModel->addOrUpdateMessage(m3);
    m3->setMessage("More tests X 3 -- EDIT");


    // Setup groups
    QTreeWidgetItem *group1Item = new QTreeWidgetItem();
    ui->conversationsTree->addTopLevelItem(group1Item);
    std::shared_ptr<Group> group1(new Group("bread", 0));
    GroupWidget *group1Widget = new GroupWidget(group1);
    ui->conversationsTree->setItemWidget(group1Item, 0, group1Widget);

    QTreeWidgetItem *group2Item = new QTreeWidgetItem();
    ui->conversationsTree->addTopLevelItem(group2Item);
    std::shared_ptr<Group> group2(new Group("naan", 1));
    GroupWidget *group2Widget = new GroupWidget(group2);
    ui->conversationsTree->setItemWidget(group2Item, 0, group2Widget);

    // Setup conversations
    QTreeWidgetItem *conv11Item = new QTreeWidgetItem();
    group1Item->addChild(conv11Item);
    std::shared_ptr<Conversation> conv11(new Conversation(30, std::make_shared<Peer>(Peer("Q1234567890abcdefg", "root"))));
    ConversationWidget *conv11Widget = new ConversationWidget(conv11);
    ui->conversationsTree->setItemWidget(conv11Item, 1, conv11Widget);

    QTreeWidgetItem *conv12Item = new QTreeWidgetItem();
    group1Item->addChild(conv12Item);
    std::shared_ptr<Conversation> conv12(new Conversation(0, std::make_shared<Peer>(Peer("Q987654321hijklmnop", "ryan"))));
    ConversationWidget *conv12Widget = new ConversationWidget(conv12);
    ui->conversationsTree->setItemWidget(conv12Item, 1, conv12Widget);

    QTreeWidgetItem *conv21Item = new QTreeWidgetItem();
    group2Item->addChild(conv21Item);
    std::shared_ptr<Conversation> conv21(new Conversation(0, std::make_shared<Peer>(Peer("QALSKDJFHG0192837465", "trevor"))));
    ConversationWidget *conv21Widget = new ConversationWidget(conv21);
    ui->conversationsTree->setItemWidget(conv21Item, 1, conv21Widget);
}

MainWindow::~MainWindow()
{
    delete ui;

    if(messageModel != nullptr) {
        delete messageModel;
    }
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
    }
}
