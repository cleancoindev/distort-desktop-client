#ifndef MAINWINDOW_H
#define MAINWINDOW_H

extern "C" {
    #include "yasl/yasl_state.h"
}

#include "messagelistmodel.h"
#include "messagedelegate.h"
#include "authparams.h"
#include "about.h"

#include <QMainWindow>
#include <QThread>
#include <QTreeWidget>
#include <QListView>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void startBackground(QString homeserver, QString peerId, QString accountName, QString authToken, QTreeWidget* conversationsTree, QListView* messagesList);

public slots:
    void setActiveGroup(QTreeWidgetItem* groupItem, bool active);
    void setSubgroupIndex(QTreeWidgetItem* groupItem, unsigned int index);
    void setConversationName(QTreeWidgetItem* convItem, QString name);
    void addGroup(QString name, unsigned int index, QTreeWidgetItem* groupItem);
    void addConv(QTreeWidgetItem* groupItem, QString peerId, QString account, QString nickname, unsigned int height, QTreeWidgetItem* item);
    void removeGroup(QTreeWidgetItem* groupItem);
    void removeConv(QTreeWidgetItem* convItem);
    void addOrUpdateMessage(Message* msg);

private slots:
    void on_actionSign_in_triggered();
    void on_actionAbout_triggered();
    void on_actionAbout_destroyed(QObject*);
    void onConversationSelected(QTreeWidgetItem* item, QTreeWidgetItem* old);

private:
    void closeEvent(QCloseEvent *event);
    void toggleMessaging(bool enable);

    Ui::MainWindow *ui;
    MessageListModel *messageModel;
    std::shared_ptr<AuthParams> authParams;
    QThread backgroundThread;

    // Windows
    About* aboutWindow;
};

#endif // MAINWINDOW_H
