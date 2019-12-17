#ifndef MESSAGELISTMODEL_H
#define MESSAGELISTMODEL_H

#include "message.h"

#include <QAbstractListModel>
#include <deque>
#include <memory>

class MessageListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit MessageListModel(std::deque<std::shared_ptr<Message>> messageList, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    void clearData();
    QVariant data(const QModelIndex &index, int role = Qt::UserRole) const override;
    void addOrUpdateMessage(std::shared_ptr<Message> message);

    enum {typeRole = Qt::UserRole + 100, messageRole = Qt::UserRole + 101, indexRole = Qt::UserRole + 102,
          fromRole = Qt::UserRole + 200, verifiedRole = Qt::UserRole + 201, dateReceivedRole = Qt::UserRole + 202,
          statusRole = Qt::UserRole + 300, lastStatusChangeRole = Qt::UserRole + 301};

private:
    std::deque<std::shared_ptr<Message>> messages;
};

#endif // MESSAGELISTMODEL_H
