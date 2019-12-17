#include "messagedelegate.h"
#include "messagelistmodel.h"
#include "inmessage.h"
#include "outmessage.h"

#include <QPainter>
#include <QApplication>
#include <messagewidget.h>

MessageDelegate::MessageDelegate(QObject* parent) :
    QStyledItemDelegate(parent) {}

void MessageDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{
    // Get relevent message fields
    bool inMessage = index.data(MessageListModel::typeRole).toString() == Message::TYPE_IN;
    QString from = inMessage ? peerName : "~";
    QString message = index.data(MessageListModel::messageRole).toString();
    QString dateString = inMessage ? index.data(MessageListModel::dateReceivedRole).toString() : index.data(MessageListModel::lastStatusChangeRole).toString();

    // Ensure antialiasing
    painter->setRenderHint(QPainter::Antialiasing);

    // Setup style options
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    // Create coloured message bubble
    QRect rect = opt.rect;
    const int MESSAGE_WIDTH = rect.width() > MAX_MESSAGE_WIDTH + 8 ? MAX_MESSAGE_WIDTH : rect.width()-8;
    QRect bubble = inMessage ? QRect(rect.left()+4, rect.top()+4, MESSAGE_WIDTH, rect.height()-8) : QRect(rect.right()-MESSAGE_WIDTH-4, rect.top()+4, MESSAGE_WIDTH, rect.height()-8);
    QPainterPath path;
    path.addRoundedRect(bubble, 12, 8);
    if(inMessage)
    {
        painter->fillPath(path, index.data(MessageListModel::verifiedRole).toBool() ? Message::getInColour() : Message::getUnverifiedColour());
    }
    else
    {
        painter->fillPath(path, index.data(MessageListModel::statusRole).toString() == "sent" ? Message::getSentColour() : Message::getEnqueuedColour());
    }

    // Draw text
    int textAlign = inMessage ? Qt::AlignLeft : Qt::AlignRight;

    QFont font("Times New Roman", 12);
    QFontMetrics fm(font);
    int l = fm.horizontalAdvance(message);
    int height = fm.height();
    int r = l / MAX_MESSAGE_WIDTH + 1;

    font.setBold(true);
    painter->setFont(font);
    if(inMessage)
    {
        painter->drawText(QRect(rect.left()+MARGIN+4, rect.top()+MARGIN+4, MESSAGE_WIDTH-(2*MARGIN), height),
            textAlign, from);
    }
    else
    {
        painter->drawText(QRect(rect.right()-MESSAGE_WIDTH-4+MARGIN, rect.top()+MARGIN+4, MESSAGE_WIDTH-(2*MARGIN), height),
            textAlign, from);
    }

    font.setBold(false);
    painter->setFont(font);
    int charStart = 0;
    for(int i = 0; i < r && charStart < message.length(); i++)
    {
        int charEnd = charStart;
        QString text;
        do
        {
            text = message.mid(charStart, ++charEnd - charStart);
        } while(fm.horizontalAdvance(text) < MESSAGE_WIDTH-(2*MARGIN) && charEnd < message.length());
        charEnd = charEnd < message.length() ? charEnd-1 : charEnd;
        text = message.mid(charStart, charEnd - charStart);
        charStart = charEnd;

        int vertPos = rect.top()+MARGIN+TEXT_MARGIN+(height*(i+1));
        if(inMessage)
        {
            painter->drawText(QRect(rect.left()+4+MARGIN, vertPos, MESSAGE_WIDTH-(2*MARGIN), height),
                textAlign, text);
        }
        else
        {
            painter->drawText(QRect(rect.right()-4-MESSAGE_WIDTH+MARGIN, vertPos, MESSAGE_WIDTH-(2*MARGIN), height),
                textAlign, text);
        }
    }

    font.setPointSize(10);
    painter->setFont(font);
    if(inMessage)
    {
        painter->drawText(QRect(rect.left()+4, rect.bottom()-MARGIN-height, MESSAGE_WIDTH-MARGIN, height),
            Qt::AlignRight, dateString);
    }
    else
    {
        painter->drawText(QRect(rect.right()-4-MAX_MESSAGE_WIDTH+MARGIN, rect.bottom()-MARGIN-height, MESSAGE_WIDTH-MARGIN, height),
            Qt::AlignLeft, dateString);
    }
}

QSize MessageDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QSize result = QStyledItemDelegate::sizeHint(option, index);
    result.setWidth(MAX_MESSAGE_WIDTH + 8);

    // Get pixel length of message and count of rows
    QString message = index.data(MessageListModel::messageRole).toString();
    QFont f("Times New Roman", 12);
    QFontMetrics fm(f);
    int l = fm.horizontalAdvance(message);
    int height = fm.height();
    int r = l / result.width() + 1;

    result.setHeight(height*(r+2) + 2*MARGIN + TEXT_MARGIN + 8);
    return result;
}

QWidget* MessageDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    bool inMessage = index.data(MessageListModel::typeRole).toString() == Message::TYPE_IN;

    QString m = index.data(MessageListModel::messageRole).toString();
    uint64_t i = index.data(MessageListModel::indexRole).toLongLong();
    QString type = index.data(MessageListModel::typeRole).toString();

    std::shared_ptr<Message> message;
    if(inMessage)
    {
        bool v = index.data(MessageListModel::verifiedRole).toBool();
        QString date = index.data(MessageListModel::dateReceivedRole).toString();
        message.reset(new InMessage(v, m, i, date));
    }
    else
    {
        QString status = index.data(MessageListModel::statusRole).toString();
        QString lastStatusChange = index.data(MessageListModel::lastStatusChangeRole).toString();
        message.reset(new OutMessage(status, m, i, lastStatusChange));
    }

    MessageWidget* editor = new MessageWidget(message, parent);
    return editor;
}

void MessageDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    return;
}

void MessageDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    return;
}

void MessageDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void MessageDelegate::setPeerName(QString name)
{
    peerName = name;
}
