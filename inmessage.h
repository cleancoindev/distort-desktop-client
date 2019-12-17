#ifndef INMESSAGE_H
#define INMESSAGE_H

#include "message.h"

class InMessage : public Message
{
public:
    InMessage(bool verified, QString message, uint64_t index, QString dateReceived);
    ~InMessage();

    const QString& getType() const;
    bool getVerified() const;
    QString getDateReceived() const;

private:
    bool verified;
    QString dateReceived;
};

#endif // INMESSAGE_H
