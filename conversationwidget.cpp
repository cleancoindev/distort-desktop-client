#include "conversationwidget.h"
#include "ui_conversationwidget.h"

ConversationWidget::ConversationWidget(std::shared_ptr<Conversation> c, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConversationWidget),
    conversation(c)
{
    ui->setupUi(this);

    ui->conversationLabel->setText(c->getPeer()->getFriendlyName());
}

ConversationWidget::~ConversationWidget()
{
    delete ui;
}
