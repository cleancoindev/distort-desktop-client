#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include "message.h"

#include <QWidget>
#include <memory>

namespace Ui {
class MessageWidget;
}

class MessageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MessageWidget(std::shared_ptr<Message> message, QWidget *parent = nullptr);
    ~MessageWidget();
    std::shared_ptr<Message> getMessage() const;
    void setMessage(std::shared_ptr<Message> message);

private:
    Ui::MessageWidget *ui;
    std::shared_ptr<Message> message;
};

#endif // MESSAGEWIDGET_H
