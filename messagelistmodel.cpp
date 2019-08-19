#include "messagelistmodel.h"

MessageListModel::MessageListModel(std::deque<std::shared_ptr<Message>> messagesDeq, QObject *parent) :
    QAbstractListModel(parent), messages(messagesDeq)
{
}

int MessageListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
    {
        return 0;
    }

    return static_cast<int>(messages.size());
}

QVariant MessageListModel::data(const QModelIndex &index, int role) const
{
    uint64_t size = messages.size();
    uint64_t i = static_cast<uint64_t>(index.row());
    if (!index.isValid() || i >= size)
    {
        return QVariant();
    }

    std::shared_ptr<Message> m = messages.at(i);
    switch(role)
    {
    case fromRole:
        return m->getFrom();
    case messageRole:
        return m->getMessage();
    case indexRole:
        return QVariant(static_cast<uint>(m->getIndex()));
    default:
        return QVariant();
    }
}
