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
    explicit ConversationWidget(Conversation conversation, QWidget *parent = nullptr);
    ~ConversationWidget();

    void setName(QString name);
    const Conversation& getConversation() const;

private:
    Ui::ConversationWidget *ui;
    Conversation conversation;
};

#endif // CONVERSATIONWIDGET_H
