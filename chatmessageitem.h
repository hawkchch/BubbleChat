#ifndef CHATMESSAGEITEM_H
#define CHATMESSAGEITEM_H

#include <QStandardItem>
#include "msgdef.h"
#include "message.h"

class ChatMessageItem : public QStandardItem
{
public:
    enum DATA_ROLE
    {
        DATA_ROLE_FROM = Qt::UserRole + 1,
        DATA_ROLE_TO,
        DATA_ROLE_AUTHOR,

        DATA_ROLE_MID,
        DATA_ROLE_ITEMS,
        DATA_ROLE_TERMINALTYPE,
        DATA_ROLE_STAMP,

        DATA_ROLE_IS_VOICE,
        DATA_ROLE_IS_OFFLINEFILE,
        DATA_ROLE_IS_GROUPCHANGE,
        DATA_ROLE_IS_TIMESEPARATOR
    };

    ChatMessageItem(const Message& msg);

    BasicDef::MessageContact from() const;
    void setFrom(const BasicDef::MessageContact& contact);

    BasicDef::MessageContact to() const;
    void setTo(const BasicDef::MessageContact& contact);

    BasicDef::MessageContact author() const;
    void setAuthor(const BasicDef::MessageContact& contact);

    qint64 mid() const;
    void setMid(const qint64& mid);

    QList<BasicDef::MsgItem> items() const;
    void setItems(const QList<BasicDef::MsgItem>& items);

    quint8 terminalType() const;
    void setTerminalType(const quint8& terminalType);

    qint64 stamp() const;
    void setStamp(const qint64& stamp);

    bool isOfflineFile() const;
    void setIsOfflineFile(const bool& isOfflineFile);

    bool isGroupChange() const;
    void setIsGroupChange(const bool& isGroupChange);

    bool isTimeSeparator() const;
    void setIsTimeSeparator(const bool& isTimeSeparator);
};

#endif // CHATMESSAGEITEM_H
