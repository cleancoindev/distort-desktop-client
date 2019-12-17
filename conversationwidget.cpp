#include "conversationwidget.h"
#include "ui_conversationwidget.h"

ConversationWidget::ConversationWidget(Conversation c, QWidget *parent) :
    QWidget(parent), ui(new Ui::ConversationWidget), conversation(c)
{
    ui->setupUi(this);
    ui->conversationLabel->setText(c.getPeer().getFriendlyName());
}

ConversationWidget::~ConversationWidget()
{
    delete ui;
}

void ConversationWidget::setName(QString name)
{
    conversation.setName(name);
    ui->conversationLabel->setText(name);
}

const Conversation& ConversationWidget::getConversation() const
{
    return conversation;
}
