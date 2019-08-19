#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "messagelistmodel.h"
#include "messagedelegate.h"
#include "authparams.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionSign_in_triggered();

private:
    Ui::MainWindow *ui;
    MessageListModel *messageModel;
    std::shared_ptr<AuthParams> authParams;
};

#endif // MAINWINDOW_H
