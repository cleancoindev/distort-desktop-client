#include "inmessage.h"

InMessage::InMessage(bool v, QString m, uint64_t i, QString d) :
    Message(m, i), verified(v), dateReceived(d)
{}
InMessage::~InMessage() {}

const QString& InMessage::getType() const {
    return Message::TYPE_IN;
}

bool InMessage::getVerified() const {
    return verified;
}

QString InMessage::getDateReceived() const {
    return dateReceived;
}
