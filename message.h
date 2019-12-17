#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <QColor>

class Message
{
public:
    static QColor getInColour();
    static QColor getSentColour();
    static QColor getUnverifiedColour();
    static QColor getEnqueuedColour();

    static const QString TYPE_IN;
    static const QString TYPE_OUT;

    Message(QString message, uint64_t index);

    QString getMessage() const;
    uint64_t getIndex() const;

    virtual const QString& getType() const = 0;

    void setMessage(QString message);
    void setIndex(uint64_t index);

private:
    QString message;
    uint64_t index;
};

#endif // MESSAGE_H
