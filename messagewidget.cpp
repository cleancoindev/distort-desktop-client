#include "messagewidget.h"
#include "ui_messagewidget.h"

MessageWidget::MessageWidget(Message m, QWidget *parent) :
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

Message MessageWidget::getMessage() const
{
    return message;
}

void MessageWidget::setMessage(const Message& m)
{
    message = Message(m.getFrom(), m.getMessage(), m.getIndex());
}
