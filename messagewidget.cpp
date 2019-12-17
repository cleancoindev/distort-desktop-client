#include "messagewidget.h"
#include "ui_messagewidget.h"

MessageWidget::MessageWidget(std::shared_ptr<Message> m, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessageWidget),
    message(m)
{
    ui->setupUi(this);
}

MessageWidget::~MessageWidget()
{
    delete ui;
}

std::shared_ptr<Message> MessageWidget::getMessage() const
{
    return message;
}

void MessageWidget::setMessage(std::shared_ptr<Message> m)
{
    message = m;
}
