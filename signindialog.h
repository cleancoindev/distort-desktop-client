#ifndef SIGNINDIALOG_H
#define SIGNINDIALOG_H

#include "account.h"
#include "authparams.h"
#include "distortexception.h"

#include <QDialog>
#include <QThread>
#include <memory>

namespace Ui {
class SignInDialog;
}

class SignInDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SignInDialog(QWidget *parent = nullptr);
    ~SignInDialog();

    std::shared_ptr<AuthParams> getAuthParams() const;

public slots:
    void onSuccess(AuthParams* authParams);
    void onFailure(std::string e);

signals:
    void performSignIn(std::string homeserver, std::string account, std::string password);

private:
    Ui::SignInDialog *ui;
    std::shared_ptr<Account> acc;
    std::shared_ptr<AuthParams> authParams;
    QThread signInThread;

private slots:
    void accept();
};

#endif // SIGNINDIALOG_H
