#include "messagelistmodel.h"
#include "inmessage.h"
#include "outmessage.h"

MessageListModel::MessageListModel(std::deque<std::shared_ptr<Message>> messagesDeq, QObject *parent) :
    QAbstractListModel(parent), messages(messagesDeq)
{}

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

void MessageListModel::clearData()
{
    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    messages.clear();
    endRemoveRows();
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
    case messageRole:
        return m->getMessage();
    case indexRole:
        return QVariant(static_cast<uint>(m->getIndex()));
    case typeRole:
        return m->getType();
    case verifiedRole:
        return m->getType() == Message::TYPE_IN ? reinterpret_cast<InMessage*>(m.get())->getVerified() : QVariant();
    case dateReceivedRole:
        return m->getType() == Message::TYPE_IN ? reinterpret_cast<InMessage*>(m.get())->getDateReceived() : QVariant();
    case statusRole:
        return m->getType() == Message::TYPE_OUT ? reinterpret_cast<OutMessage*>(m.get())->getStatus() : QVariant();
    case lastStatusChangeRole:
        return m->getType() == Message::TYPE_OUT ? reinterpret_cast<OutMessage*>(m.get())->getLastStatusChange() : QVariant();

    default:
        return QVariant();
    }
}

void MessageListModel::addOrUpdateMessage(std::shared_ptr<Message> m)
{
    // Logarithmic binary search
    auto start = messages.begin();
    auto end = messages.end();
    auto it = end;
    while(start < end) {
        auto mid = (end-start)/2 + start;
        uint64_t j = mid->get()->getIndex();
        if(j == m->getIndex()) {
            it = mid;
            break;
        } else if(j < m->getIndex()) {
            start = mid+1;
        } else {
            end = mid;
        }
    }

    if(it == messages.end() || it->get()->getIndex() > m->getIndex()) {
        int i = it-messages.begin();
        beginInsertRows(QModelIndex(), i, i);
        messages.insert(end, m);
        endInsertRows();
        // emit dataChanged(index(i, 0), index(i, 0), QVector<int>());
    } else {
        bool change = false;
        if((*it)->getType() == Message::TYPE_IN)
        {
            InMessage* im1 = reinterpret_cast<InMessage*>((*it).get());
            InMessage* im2 = reinterpret_cast<InMessage*>(m.get());
            change = im1->getVerified() != im2->getVerified();
        }
        else
        {
            OutMessage* om1 = reinterpret_cast<OutMessage*>((*it).get());
            OutMessage* om2 = reinterpret_cast<OutMessage*>(m.get());
            change = om1->getLastStatusChange() != om2->getLastStatusChange() || om1->getStatus() != om2->getStatus();
        }

        if(change)
        {
            *it = m;
            emit dataChanged(index(it-messages.begin(), 0), index(it - messages.begin(), 0), QVector<int>());
        }
    }
}
