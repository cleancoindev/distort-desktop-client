#ifndef BACKGROUNDWORKER_H
#define BACKGROUNDWORKER_H

#include "message.h"

#include <QObject>
#include <QTreeWidget>
#include <QListView>

struct YASL_State;

class BackgroundWorker : public QObject
{
    Q_OBJECT
public:
    explicit BackgroundWorker(QObject* parent = nullptr);

signals:
    void setActiveGroupUI(QTreeWidgetItem* groupItem, bool active);
    void setSubgroupIndexUI(QTreeWidgetItem* groupItem, unsigned int index);
    void setConversationNameUI(QTreeWidgetItem* convItem, QString name);
    void addGroupUI(QString name, unsigned int index, QTreeWidgetItem* item);
    void addConvUI(QTreeWidgetItem* groupItem, QString peerId, QString account, QString nickname, unsigned int height, QTreeWidgetItem* item);
    void removeGroupUI(QTreeWidgetItem* groupItem);
    void removeConvUI(QTreeWidgetItem* convItem);
    void addOrUpdateMessageUI(Message* groupItem);

public slots:
    void doWork(QString homeserver, QString peerId, QString account, QString authToken);
};

#endif // BACKGROUNDWORKER_H
