#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include "message.h"

#include <QWidget>

namespace Ui {
class MessageWidget;
}

class MessageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MessageWidget(Message message, QWidget *parent = nullptr);
    ~MessageWidget();
    Message getMessage() const;
    void setMessage(const Message& message);

private:
    Ui::MessageWidget *ui;
    Message message;
};

#endif // MESSAGEWIDGET_H
