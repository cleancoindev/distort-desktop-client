#include "signindialog.h"
#include "ui_signindialog.h"
#include "signinworker.h"

SignInDialog::SignInDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::SignInDialog), authParams(nullptr)

{
    ui->setupUi(this);
    setWindowTitle("Sign In");

    SignInWorker *worker = new SignInWorker;
    worker->moveToThread(&signInThread);
    connect(&signInThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &SignInDialog::performSignIn, worker, &SignInWorker::doWork);
    connect(worker, &SignInWorker::success, this, &SignInDialog::onSuccess);
    connect(worker, &SignInWorker::failure, this, &SignInDialog::onFailure);
    signInThread.start();
}

SignInDialog::~SignInDialog()
{
    QApplication::restoreOverrideCursor();

    delete ui;
    signInThread.quit();
    signInThread.wait(2000);
}

std::shared_ptr<AuthParams> SignInDialog::getAuthParams() const
{
    return authParams;
}

void SignInDialog::accept()
{
    QString homeserver = ui->homeserverEdit->text();
    homeserver = homeserver.isEmpty() ? "http://localhost:6945/" : homeserver;

    // Ensure homeserver URL's end with '/' for consistency in request-path appending
    if(homeserver.back() != '/') {
        homeserver += '/';
    }

    QString account = ui->accountEdit->text();
    account = account.isEmpty() ? "root" : account;
    QString password = ui->passwordEdit->text();
    if(password.isEmpty())
    {
        return ui->errorLabel->setText("Password field cannot be empty");
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    emit performSignIn(homeserver, account, password);
}

void SignInDialog::onSuccess(AuthParams* auth)
{
    QApplication::restoreOverrideCursor();

    authParams.reset(auth);
    done(Accepted);
}

void SignInDialog::onFailure(QString e)
{
    QApplication::restoreOverrideCursor();

    return ui->errorLabel->setText(e);
}
