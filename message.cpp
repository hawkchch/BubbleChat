#include "message.h"
#include "msgdef.h"
#include <QObject>
#include <QJsonDocument>

Message::Message()
{
    m_terminalType = BasicDef::TerminalType_PC;
    m_messageId = 0;
    m_author.id = 0;
    m_from.id = 0;
    m_direction = MessageOut;
    m_isOffLineFile = false;
    m_isGroupChange = false;
    m_isTimeSeparator = false;
    m_plainTextCount = 0;
    m_isVoice = false;
    m_stamp = 0;
}

void Message::setTo(const BasicDef::MessageContact &to)
{
    m_to = to;
}

BasicDef::MessageContact Message::to() const
{
    return m_to;
}

bool Message::initWithJsonMap(const QVariantMap &map)
{
    m_from.initWithJsonMap(map.value(BasicDef::MessagePacketKey_From).toMap());
    m_author.initWithJsonMap(map.value(BasicDef::MessagePacketKey_Author).toMap());
    m_to.initWithJsonMap(map.value(BasicDef::MessagePacketKey_To).toMap());

    m_ccs.clear();
    if(map.contains(BasicDef::MessagePacketKey_Ccs))
    {
        QVariantList tempList = map.value(BasicDef::MessagePacketKey_Ccs).toList();
        foreach (QVariant var, tempList) {
            BasicDef::MessageContact c;
            c.initWithJsonMap(var.toMap());
            m_ccs.append(c);
        }
    }

    m_terminalType = map.value(BasicDef::MessagePacketKey_AuthorTerminal).toInt();
    m_stamp = map.value(BasicDef::MessagePacketKey_Stamp).toLongLong();
    m_messageId = map.value(BasicDef::MessagePacketKey_Id).toLongLong();

    m_items.clear();
    QVariantList tempList = map.value(BasicDef::MessagePacketKey_Body).toList();
    foreach (QVariant var, tempList) {
        QVariantMap tempMap = var.toMap();
        bool ok;
        qint16 count = tempMap.value(BasicDef::MessagePacketKey_ItemCount).toInt(&ok);
        if(!ok)
            count = 1;
        BasicDef::MsgItem item;
        item.type = (BasicDef::MsgItemType)(tempMap.value(BasicDef::MessagePacketKey_ItemType).toInt());
        qint32 voiceDuration = tempMap.value(BasicDef::MessagePacketKey_ItemVoiceDuration).toInt(&ok);
        if(ok)
            item.voiceDuration = voiceDuration;
        QString data = tempMap.value(BasicDef::MessagePacketKey_ItemData).toString();

        if(item.type == BasicDef::MIT_EMOTICONS)
        {
            if(data.startsWith("face"))
                item.data = data.mid(4);
            else
                item.data = data;
        }
        else if(item.type == BasicDef::MIT_GIF)
        {
            if(data.startsWith("bface"))
                item.data = data.mid(5);
            else
                item.data = data;
        }
        else
            item.data = data;
        for(int index = 0; index < count; index++)
        {
            m_items.append(item);
        }
        if(item.type == BasicDef::MIT_OFFLINEFILE)
        {
            m_isOffLineFile = true;
        }
        else if(item.type == BasicDef::MIT_GROUPCHANGE)
        {
            m_isGroupChange = true;
        }
        else if(item.type == BasicDef::MIT_VOICE)
        {
            m_isVoice = true;
        }
    }
    return true;
}

QVariantMap Message::toJsonMap() const
{
    QVariantMap map;
    if(m_from.id != 0)
    {
        map.insert(BasicDef::MessagePacketKey_From, m_from.toJsonMap());
    }
    if(m_author.id != 0)
    {
        map.insert(BasicDef::MessagePacketKey_Author, m_author.toJsonMap());
    }
    map.insert(BasicDef::MessagePacketKey_To, m_to.toJsonMap());
    QVariantList tempList;
    foreach (BasicDef::MessageContact c, m_ccs) {
        tempList.append(c.toJsonMap());
    }

    if(!m_ccs.isEmpty())
        map.insert(BasicDef::MessagePacketKey_Ccs, tempList);
    map.insert(BasicDef::MessagePacketKey_AuthorTerminal, m_terminalType);
    map.insert(BasicDef::MessagePacketKey_Stamp, m_stamp);
    map.insert(BasicDef::MessagePacketKey_Id, m_messageId);

    tempList.clear();
    for(int i=0; i<m_items.count(); i++){
        BasicDef::MsgItem item = m_items.at(i);
        QVariantMap tempMap;
        tempMap.insert(BasicDef::MessagePacketKey_ItemType, item.type);
        if(item.voiceDuration > 0)
        {
            tempMap.insert(BasicDef::MessagePacketKey_ItemVoiceDuration, item.voiceDuration);
        }
        if(item.type == BasicDef::MIT_EMOTICONS)
        {
            if(item.data.startsWith("face"))
                tempMap.insert(BasicDef::MessagePacketKey_ItemData, item.data.mid(4));
            else
                tempMap.insert(BasicDef::MessagePacketKey_ItemData, item.data);
        }
        else if(item.type == BasicDef::MIT_GIF)
        {
            if(item.data.startsWith("bface"))
                tempMap.insert(BasicDef::MessagePacketKey_ItemData, item.data.mid(5));
            else
                tempMap.insert(BasicDef::MessagePacketKey_ItemData, item.data);
        }
        else
            tempMap.insert(BasicDef::MessagePacketKey_ItemData, item.data);
        if(!tempList.isEmpty())
        {
            if(item.type == (BasicDef::MsgItemType)(tempList[tempList.size() - 1].toMap().value(BasicDef::MessagePacketKey_ItemType).toInt()))
            {
                QString data = tempList[tempList.size() - 1].toMap().value(BasicDef::MessagePacketKey_ItemData).toString();
                //                if(item.type == BasicDef::MIT_EMOTICONS)
                //                    data = "face" + data;
                if(item.data == data)
                {
                    qint16 count = tempList[tempList.size() - 1].toMap().value(BasicDef::MessagePacketKey_ItemCount).toInt() + 1;
                    tempMap.insert(BasicDef::MessagePacketKey_ItemCount, count);
                    tempList.removeLast();
                }
                else
                    tempMap.insert(BasicDef::MessagePacketKey_ItemCount, 1);
            }
            else
                tempMap.insert(BasicDef::MessagePacketKey_ItemCount, 1);
        }
        else
            tempMap.insert(BasicDef::MessagePacketKey_ItemCount, 1);
        tempList.append(tempMap);
    }
    map.insert(BasicDef::MessagePacketKey_Body, tempList);

    return map;
}

QVariantMap Message::bodyToJsonMap() const
{
    QVariantMap map;
    QVariantMap tempMap;
    QVariantList tempList;
    for(int i=0; i<m_items.count(); i++){
        BasicDef::MsgItem item = m_items.at(i);
        tempMap.clear();
        tempMap.insert(BasicDef::MessagePacketKey_ItemType, item.type);
        if(item.voiceDuration > 0)
            tempMap.insert(BasicDef::MessagePacketKey_ItemVoiceDuration, item.voiceDuration);
        if(item.type == BasicDef::MIT_EMOTICONS)
        {
            if(item.data.startsWith("face"))
                tempMap.insert(BasicDef::MessagePacketKey_ItemData, item.data.mid(4));
            else
                tempMap.insert(BasicDef::MessagePacketKey_ItemData, item.data);
        }
        else if(item.type == BasicDef::MIT_GIF)
        {
            if(item.data.startsWith("bface"))
                tempMap.insert(BasicDef::MessagePacketKey_ItemData, item.data.mid(5));
            else
                tempMap.insert(BasicDef::MessagePacketKey_ItemData, item.data);
        }
        else
            tempMap.insert(BasicDef::MessagePacketKey_ItemData, item.data);
        if(!tempList.isEmpty())
        {
            if(item.type == (BasicDef::MsgItemType)(tempList[tempList.size() - 1].toMap().value(BasicDef::MessagePacketKey_ItemType).toInt()))
            {
                QString data = tempList[tempList.size() - 1].toMap().value(BasicDef::MessagePacketKey_ItemData).toString();
                //                if(item.type == BasicDef::MIT_EMOTICONS)
                //                    data = "face" + data;
                if(item.data == data)
                {
                    qint16 count = tempList[tempList.size() - 1].toMap().value(BasicDef::MessagePacketKey_ItemCount).toInt() + 1;
                    tempMap.insert(BasicDef::MessagePacketKey_ItemCount, count);
                    tempList.removeLast();
                }
                else
                    tempMap.insert(BasicDef::MessagePacketKey_ItemCount, 1);
            }
            else
                tempMap.insert(BasicDef::MessagePacketKey_ItemCount, 1);
        }
        else
            tempMap.insert(BasicDef::MessagePacketKey_ItemCount, 1);
        tempList.append(tempMap);
    }
    map.insert(BasicDef::MessagePacketKey_Body, tempList);

    return map;
}

bool Message::initItemsWithBodyJasonMap(const QVariantMap &map)
{
    QVariantMap tempMap;
    m_items.clear();
    QVariantList tempList = map.value(BasicDef::MessagePacketKey_Body).toList();
    foreach (QVariant var, tempList) {
        tempMap = var.toMap();
        bool ok;
        qint16 count = tempMap.value(BasicDef::MessagePacketKey_ItemCount).toInt(&ok);
        if(!ok)
            count = 1;
        BasicDef::MsgItem item;
        item.type = (BasicDef::MsgItemType)(tempMap.value(BasicDef::MessagePacketKey_ItemType).toInt());
        qint32 duration = tempMap.value(BasicDef::MessagePacketKey_ItemVoiceDuration).toInt(&ok);
        if(ok)
            item.voiceDuration = duration;
        QString data = tempMap.value(BasicDef::MessagePacketKey_ItemData).toString();
        if(item.type == BasicDef::MIT_EMOTICONS)
        {
            if(data.startsWith("face"))
                item.data = data.mid(4);
            else
                item.data = data;
        }
        else if(item.type == BasicDef::MIT_GIF)
        {
            if(data.startsWith("bface"))
                item.data = data.mid(5);
            else
                item.data = data;
        }
        else
            item.data = data;
        for(int index = 0; index < count; index++)
        {
            m_items.append(item);
        }
        if(item.type == BasicDef::MIT_OFFLINEFILE)
        {
            m_isOffLineFile = true;
        }
        else if(item.type == BasicDef::MIT_GROUPCHANGE)
        {
            m_isGroupChange = true;
        }
        else if(item.type == BasicDef::MIT_VOICE)
        {
            m_isVoice = true;
        }
    }
    return true;
}

int Message::imageCount()
{
    int count = 0;
    foreach(BasicDef::MsgItem item, m_items)
    {
        if(item.type == BasicDef::MIT_IMAGE)
            count++;
    }
    return count;
}

Message::GroupChange Message::groupChange()
{
    GroupChange change;
    change.gid = 0;
    change.stamp = 0;
    change.type = GroupChangedType_None;
    if(m_isGroupChange)
    {
        if(!m_items.isEmpty())
        {
            BasicDef::MsgItem item = m_items.at(0);
            QJsonDocument jsonDoc = QJsonDocument::fromJson(item.data.toUtf8());
            if(!jsonDoc.isNull())
            {
                QVariant v = jsonDoc.toVariant();
                if(v.type() == QVariant::Map)
                {
                    QVariantMap jsonMap = v.toMap();
                    change.gid = jsonMap.value("gid").toLongLong();
                    change.stamp = jsonMap.value("st").toLongLong();
                    change.type = jsonMap.value("t").toInt();
                    QVariantList uids = jsonMap.value("uids").toList();
                    foreach (QVariant vUid, uids)
                    {
                        qint64 uid = vUid.toLongLong();
                        if(uid > 0 && !change.uids.contains(uid))
                            change.uids.append(uid);
                    }
                }
            }
        }
    }
    return change;
}

void Message::setGroupChangeMsgItem(const QString &text)
{
    if(!m_isGroupChange)
        return;
    if(!m_items.isEmpty())
    {
        m_items[0].data = text;
    }
}

QString Message::description() const
{
    QString description;
    foreach (BasicDef::MsgItem item, m_items) {
        switch (item.type) {
        case BasicDef::MIT_TEXT:
            description += item.data;
            break;
        case BasicDef::MIT_IMAGE:
            description += QObject::tr("[image]");
            break;
        case BasicDef::MIT_EMOTICONS:
            description += QObject::tr("[Emotion]");
            break;
        case BasicDef::MIT_GIF:
            description += QObject::tr("[Emotion]");
            break;
        case BasicDef::MIT_VOICE:
            description += QObject::tr("[Voice]");
            break;
        default:
            break;
        }
    }
    return description;
}
