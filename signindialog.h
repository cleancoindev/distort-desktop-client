#ifndef SIGNINDIALOG_H
#define SIGNINDIALOG_H

#include "account.h"
#include "authparams.h"

#include <QDialog>
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

private slots:
    void accept();

private:
    Ui::SignInDialog *ui;
    std::shared_ptr<Account> acc;
    std::shared_ptr<AuthParams> authParams;
};

#endif // SIGNINDIALOG_H
