#include "chatmessagemodel.h"
#include "chatmessageitem.h"


ChatMessageSortFilterProxyModel::ChatMessageSortFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    setSortLocaleAware(true);
    setDynamicSortFilter(true);
    setSortCaseSensitivity(Qt::CaseInsensitive);
}

void ChatMessageSortFilterProxyModel::setFilterString(const QString &filterString)
{
    m_filterString = filterString;
    invalidate();
}

bool ChatMessageSortFilterProxyModel::filterAcceptsRow(int AModelRow, const QModelIndex &AModelParent) const
{
    if(m_filterString.isEmpty())
        return true;

    QModelIndex index = sourceModel()->index(AModelRow,0,AModelParent);

    if (index.isValid())
    {
//        if(index.data(BasicDef::OMDR_TYPE).toInt() != OrgItemType_Contact && index.data(BasicDef::OMDR_TYPE).toInt() != OrgItemType_Org_Contact
//                && index.data(BasicDef::OMDR_TYPE).toInt() != OrgItemType_Org_Roster)
//            return false;
//        QString name = index.data(Qt::DisplayRole).toString();
//        if(name.contains(m_filterName, Qt::CaseInsensitive))
//            return true;
//        return false;
    }
    return true;
}

bool ChatMessageSortFilterProxyModel::lessThan(const QModelIndex &ALeft, const QModelIndex &ARight) const
{
    qint64 lStamp = ALeft.data(ChatMessageItem::DATA_ROLE_STAMP).toInt();
    qint64 rStamp = ARight.data(ChatMessageItem::DATA_ROLE_STAMP).toInt();
    return lStamp < rStamp;
    //return QSortFilterProxyModel::lessThan(ALeft, ARight);
}



ChatMessageModel::ChatMessageModel(QObject *parent) :
    QStandardItemModel(parent)
{
}

void ChatMessageModel::appendChatMessage(const Message &msg)
{
    ChatMessageItem *item = new ChatMessageItem(msg);
    appendRow(item);
}

void ChatMessageModel::appendChatMessages(const QList<Message> &msgs)
{
    foreach (auto msg, msgs)
    {
        appendChatMessage(msg);
    }
}

