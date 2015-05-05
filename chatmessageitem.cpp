#include "chatmessageitem.h"

Q_DECLARE_METATYPE(BasicDef::MessageContact)
Q_DECLARE_METATYPE(BasicDef::MsgItem)

ChatMessageItem::ChatMessageItem(const Message &msg)
{
    setFrom(msg.from());
    setTo(msg.to());
    setAuthor(msg.author());
    setMid(msg.messageId());
    setItems(msg.items());
    setTerminalType(msg.terminalType());
    setStamp(msg.stamp());
}

BasicDef::MessageContact ChatMessageItem::from() const
{
    return data(DATA_ROLE_FROM).value<BasicDef::MessageContact>();
}

void ChatMessageItem::setFrom(const BasicDef::MessageContact &contact)
{
    setData(QVariant::fromValue(contact), DATA_ROLE_FROM);
}

BasicDef::MessageContact ChatMessageItem::to() const
{
    return data(DATA_ROLE_TO).value<BasicDef::MessageContact>();
}

void ChatMessageItem::setTo(const BasicDef::MessageContact &contact)
{
    setData(QVariant::fromValue(contact), DATA_ROLE_TO);
}

BasicDef::MessageContact ChatMessageItem::author() const
{
    return data(DATA_ROLE_AUTHOR).value<BasicDef::MessageContact>();
}

void ChatMessageItem::setAuthor(const BasicDef::MessageContact &contact)
{
    setData(QVariant::fromValue(contact), DATA_ROLE_AUTHOR);
}

qint64 ChatMessageItem::mid() const
{
    return data(DATA_ROLE_MID).toInt();
}

void ChatMessageItem::setMid(const qint64 &mid)
{
    setData(mid, DATA_ROLE_MID);
}

QList<BasicDef::MsgItem> ChatMessageItem::items() const
{
    return data(DATA_ROLE_ITEMS).value<QList<BasicDef::MsgItem>>();
}

void ChatMessageItem::setItems(const QList<BasicDef::MsgItem> &items)
{
    setData(QVariant::fromValue(items), DATA_ROLE_ITEMS);
}

quint8 ChatMessageItem::terminalType() const
{
    return data(DATA_ROLE_TERMINALTYPE).toUInt();
}

void ChatMessageItem::setTerminalType(const quint8 &terminalType)
{
    setData(terminalType, DATA_ROLE_TERMINALTYPE);
}

qint64 ChatMessageItem::stamp() const
{
    return data(DATA_ROLE_STAMP).toInt();
}

void ChatMessageItem::setStamp(const qint64 &stamp)
{
    setData(stamp, DATA_ROLE_STAMP);
}

bool ChatMessageItem::isOfflineFile() const
{
    return data(DATA_ROLE_IS_OFFLINEFILE).toBool();
}

void ChatMessageItem::setIsOfflineFile(const bool &isOfflineFile)
{
    setData(isOfflineFile, DATA_ROLE_IS_OFFLINEFILE);
}

bool ChatMessageItem::isGroupChange() const
{
    return data(DATA_ROLE_IS_GROUPCHANGE).toBool();
}

void ChatMessageItem::setIsGroupChange(const bool &isGroupChange)
{
    setData(isGroupChange, DATA_ROLE_IS_GROUPCHANGE);
}

bool ChatMessageItem::isTimeSeparator() const
{
    return data(DATA_ROLE_IS_TIMESEPARATOR).toBool();
}

void ChatMessageItem::setIsTimeSeparator(const bool &isTimeSeparator)
{
    setData(isTimeSeparator, DATA_ROLE_IS_TIMESEPARATOR);
}
