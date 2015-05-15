#include "chatmessageview.h"
#include <qdebug.h>
#include <QScrollBar>

ChatMessageView::ChatMessageView(QWidget *parent)
    :QListView(parent)
{
    setAutoScroll(false);
    m_sourceModel = new ChatMessageModel(this);
    m_sortModel = new ChatMessageSortFilterProxyModel(this);
    m_sortModel->setSourceModel(m_sourceModel);

    setItemDelegate(new ChatMessageDelegate(this));

    setModel(m_sourceModel);

    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setResizeMode(QListView::Adjust);
    setLayoutMode(QListView::Batched);
    setUpdatesEnabled(true);
}

ChatMessageView::~ChatMessageView()
{

}

void ChatMessageView::appendMessage(const Message &msg)
{
    m_sourceModel->appendChatMessage(msg);
    openPersistentEditor(m_sourceModel->index(m_sourceModel->rowCount()-1, 0));
}

void ChatMessageView::appendMessages(const QList<Message> &msgs)
{
    foreach(Message msg, msgs)
    {
        appendMessage(msg);
    }
}

