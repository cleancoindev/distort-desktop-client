#ifndef MESSAGEITEM_H
#define MESSAGEITEM_H

#include <QStyledItemDelegate>

class MessageDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    using QStyledItemDelegate::QStyledItemDelegate;

    explicit MessageDelegate(QObject* parent = nullptr);
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setPeerName(QString name);

private:
    const int MARGIN = 16;
    const int TEXT_MARGIN = 6;
    const int MAX_MESSAGE_WIDTH = 800;

    QString peerName;
};

#endif // MESSAGEITEM_H
