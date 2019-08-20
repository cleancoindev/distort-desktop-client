#include "signindialog.h"
#include "ui_signindialog.h"
#include "signinworker.h"

SignInDialog::SignInDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::SignInDialog), acc(nullptr), authParams(nullptr)

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
    signInThread.wait(3000);
}

std::shared_ptr<AuthParams> SignInDialog::getAuthParams() const
{
    return authParams;
}

void SignInDialog::accept()
{
    std::string homeserver = ui->homeserverEdit->text().toStdString();
    /*if(homeserver.empty())
    {
        return ui->errorLabel->setText("Must specify a homeserver network address");
    }*/
    homeserver = homeserver.empty() ? "http://localhost:6945/" : homeserver;

    // Ensure homeserver URL's end with '/' for consistency in request-path appending
    if(homeserver.back() != '/') {
        homeserver += '/';
    }

    std::string account = ui->accountEdit->text().toStdString();
    account = account.empty() ? "root" : account;
    std::string password = ui->passwordEdit->text().toStdString();
    if(password.empty())
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

void SignInDialog::onFailure(std::string e)
{
    QApplication::restoreOverrideCursor();

    return ui->errorLabel->setText(e.c_str());
}
