#ifndef MESSAGE_H
#define MESSAGE_H


#include <QtGlobal>
#include <QList>

#include "msgdef.h"
#include <QVariantMap>


class Message
{
public:
    enum GroupChangedType
    {
        GroupChangedType_None = 0x00,
        GroupChangedType_Create,
        GroupChangedType_Updated,
        GroupChangedType_Removed
    };
    struct GroupChange{
        qint64 gid;
        QList<qint64> uids;
        int type;
        qint64 stamp;
    };
    enum MessageDirection {
            MessageIn,
            MessageOut
        };
    Message();
    BasicDef::MessageContact from() const {return m_from;}
    void setFrom(const BasicDef::MessageContact &from) {m_from = from;}
    QList<BasicDef::MessageContact> ccs() const {return m_ccs;}
    void setCcs(const QList<BasicDef::MessageContact> &ccs) {m_ccs = ccs;}
    void setTo(const BasicDef::MessageContact& to);
    BasicDef::MessageContact to() const;
    BasicDef::MessageContact author() const {return m_author;}
    void setAuthor(const BasicDef::MessageContact& author){m_author = author;}
    quint8 terminalType() const {return m_terminalType; }
    void setTerminalType(const quint8 type) {m_terminalType = type; }
    qint64 stamp() const {return m_stamp;}
    void setStamp(qint64 stamp) {m_stamp = stamp;}
    QList<BasicDef::MsgItem> items() const {return m_items;}
    void setItems(const QList<BasicDef::MsgItem>& items) {m_items = items;}
    void appendItem(const BasicDef::MsgItem &item) {m_items.append(item);}
    qint64 messageId() const {return m_messageId;}
    void setMessageId(const qint64 id){m_messageId = id;}
    MessageDirection direction() const {return m_direction;}
    void setDirection(const MessageDirection dir) {m_direction = dir;}
    bool initWithJsonMap(const QVariantMap &map);
    QVariantMap toJsonMap() const;
    QVariantMap bodyToJsonMap() const;
    bool initItemsWithBodyJasonMap(const QVariantMap &map);
    int imageCount();
    QString description() const;
    bool isOffLineFile() const {return m_isOffLineFile;}
    void setIsOffLineFile(bool isOffLineFile) {m_isOffLineFile = isOffLineFile;}
    bool isGroupChange() const {return m_isGroupChange;}
    void setIsGroupChange(bool isGroupChange) {m_isGroupChange = isGroupChange;}
    GroupChange groupChange();
    void setGroupChangeMsgItem(const QString &text);
    bool isTimeSeparator() const {return m_isTimeSeparator;}
    void setIsTimeSeparator(bool isTimeSeparator) {m_isTimeSeparator = isTimeSeparator;}
    void setPlainTextCount(int count) {m_plainTextCount = count;}
    int plainTextCount() const {return m_plainTextCount;}
    bool isVoice() const {return m_isVoice;}
    void setIsVoice(bool isVoice) {m_isVoice = isVoice;}

private:
    BasicDef::MessageContact m_from;
    BasicDef::MessageContact m_to;
    QList<BasicDef::MessageContact> m_ccs;
    BasicDef::MessageContact m_author;
    quint8 m_terminalType;
    qint64 m_stamp;
    QList<BasicDef::MsgItem> m_items;
    qint64 m_messageId;
    MessageDirection m_direction;

    bool m_isOffLineFile;
    bool m_isGroupChange;
    bool m_isVoice;
    bool m_isTimeSeparator;

    int m_plainTextCount;
};

#endif // MESSAGE_H
