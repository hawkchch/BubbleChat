#include "chatmessagedelegate.h"

ChatMessageDelegate::ChatMessageDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void ChatMessageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

}

QSize ChatMessageDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{

}

QWidget *ChatMessageDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

}

void ChatMessageDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{

}

void ChatMessageDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

}
