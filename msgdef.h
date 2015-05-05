#ifndef MSGDEF_H
#define MSGDEF_H

#include <QtGlobal>
#include <QString>
#include <QDateTime>
#include <QVariantMap>

namespace BasicDef {

//TerminalType
enum{
    TerminalType_None = 0,
    TerminalType_PC,
    TerminalType_Mobile,
    TerminalType_Pad,
    TerminalType_Web
};

enum MsgItemType{
    MIT_NONE = 0,
    MIT_TEXT,          //"t"
    MIT_IMAGE,         //"i"
    MIT_EMOTICONS,     //"e"
    MIT_VOICE,         //"v"
    MIT_GIF,           //"g"
    MIT_OFFLINEFILE,   //"of"
    MIT_GROUPCHANGE    //"gc"
};

enum {
    MCT_None = 0,
    MCT_User,
    MCT_Group
};


const static QString MessagePacketKey_From = "fr";
const static QString MessagePacketKey_To = "to";
const static QString MessagePacketKey_Ccs = "cc";
const static QString MessagePacketKey_Author = "ar";
const static QString MessagePacketKey_AuthorTerminal = "at";
const static QString MessagePacketKey_Stamp = "st";
const static QString MessagePacketKey_Body = "bd";
const static QString MessagePacketKey_ItemType = "t";
const static QString MessagePacketKey_ItemData = "d";
const static QString MessagePacketKey_ItemCount = "c";
const static QString MessagePacketKey_ItemVoiceDuration = "vd";
//const static QString MessagePacketKey_ItemIndex = "i";
const static QString MessagePacketKey_Id = "id";

const static QString MessageContactKey_Id = "id";
const static QString MessageContactKey_Type = "t";


const static QString MessagePacketKey_ResOldId = "oid";
const static QString MessagePacketKey_ResServerId = "mid";

const static int MaxSendPlainCharacterCount = 600;

typedef struct _MsgItem{
    MsgItemType type;
    QString data;
    qint16 count;
    qint32 voiceDuration;
    _MsgItem(){
        count = 1;
        voiceDuration = 0;
    }

    /*qint16 count;
    bool operator==(const MsgItem &AOther) const {
        return type==AOther.type && data==AOther.data;
    }
    bool operator!=(const MsgItem &AOther) const {
        return !operator==(AOther);
    }*/
}MsgItem;


struct MessageContentOptions
{
    enum ContentKind {
        KindMessage,
        KindStatus,
        KindTopic
    };
    enum ContentType {
        TypeEmpty           =0x00,
        TypeGroupchat       =0x01,
        TypeHistory         =0x02,
        TypeEvent           =0x04,
        TypeMention         =0x08,
        TypeNotification    =0x10
    };
    enum ContentStatus {
        StatusEmpty,
        StatusOnline,
        StatusOffline,
        StatusAway,
        StatusAwayMessage,
        StatusReturnAway,
        StatusIdle,
        StatusReturnIdle,
        StatusDateSeparator,
        StatusJoined,
        StatusLeft,
        StatusError,
        StatusTimeout,
        StatusEncryption,
        StatusFileTransferBegan,
        StatusFileTransferComplete
    };
    enum ContentDirection {
        DirectionIn,
        DirectionOut
    };
    MessageContentOptions() {
        kind = KindMessage;
        type = TypeEmpty;
        status = StatusEmpty;
        direction = DirectionIn;
        noScroll = false;
        isGroup = false;
    }
    int kind;
    int type;
    int status;
    int direction;
    bool noScroll;
    bool isGroup;
    QDateTime time;
    QString senderId;
    QString senderName;
    QString senderAvatar;
    QString senderColor;
    QString textBGColor;
};


struct MessageContact{
    MessageContact() {
        id = 0;
        type = 0;
    }
    QString String() const{
        return QString("%1_%2").arg(id).arg(type);
    }

    bool isValid() const{
        return id > 0;
    }

    qint64 id;
    qint8 type;


    void initWithJsonMap(const QVariantMap &map)
    {
        id = map.value(BasicDef::MessageContactKey_Id).toLongLong();
        type = map.value(BasicDef::MessageContactKey_Type).toInt();
    }


    QVariantMap toJsonMap() const
    {
        QVariantMap map;
        map.insert(BasicDef::MessageContactKey_Id, id);
        map.insert(BasicDef::MessageContactKey_Type, type);
        return map;
    }

    bool operator==(const MessageContact &AOther) const {
        return id==AOther.id && type==AOther.type;
    }
    bool operator!=(const MessageContact &AOther) const {
        return !operator==(AOther);
    }
    bool operator <(const MessageContact &AOther) const {
        if(type < AOther.type)
            return true;
        else if(type == AOther.type && id < AOther.id)
            return true;
        else
            return false;
    }
};


}
#endif // MSGDEF_H

