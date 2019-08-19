#ifndef CONVERSATIONWIDGET_H
#define CONVERSATIONWIDGET_H

#include "conversation.h"

#include <QWidget>
#include <memory>

namespace Ui {
class ConversationWidget;
}

class ConversationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConversationWidget(std::shared_ptr<Conversation> conversation, QWidget *parent = nullptr);
    ~ConversationWidget();

private:
    Ui::ConversationWidget *ui;
    std::shared_ptr<Conversation> conversation;
};

#endif // CONVERSATIONWIDGET_H
