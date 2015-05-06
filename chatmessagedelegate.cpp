#include "chatmessagedelegate.h"
#include "chatmessageitem.h"
#include "multitext.h"

ChatMessageDelegate::ChatMessageDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

void ChatMessageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

}

QSize ChatMessageDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QItemDelegate::sizeHint(option, index);
}

QWidget *ChatMessageDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    QWidget *widget = nullptr;
    if ( index.data(ChatMessageItem::DATA_ROLE_MESSAGE).canConvert<Message>() )
    {
        auto msg = index.data(ChatMessageItem::DATA_ROLE_MESSAGE).value<Message>();
        switch (msg.items()[0].type) {
        case BasicDef::MIT_NONE:
            break;
        case BasicDef::MIT_TEXT:
        case BasicDef::MIT_IMAGE:
        case BasicDef::MIT_GIF:
        case BasicDef::MIT_EMOTICONS:
            widget = new MultiText(msg, nullptr);
            break;

        case BasicDef::MIT_VOICE:
            break;

        case BasicDef::MIT_OFFLINEFILE:
            break;

        default:
            break;
        }
    }

    if(widget != nullptr)
    {
        return widget;
    }
    else
    {
        return QItemDelegate::createEditor(parent, option, index);
    }
}

void ChatMessageDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{

}

void ChatMessageDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

}
