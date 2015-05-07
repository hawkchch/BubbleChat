#include "chatmessageview.h"

ChatMessageView::ChatMessageView(QWidget *parent)
    :QListView(parent)
{
    m_sourceModel = new ChatMessageModel(this);
    m_sortModel = new ChatMessageSortFilterProxyModel(this);
    m_sortModel->setSourceModel(m_sourceModel);

    setItemDelegate(new ChatMessageDelegate(this));

    setModel(m_sourceModel);
}

ChatMessageView::~ChatMessageView()
{

}

void ChatMessageView::appendMessage(const Message &msg)
{
    m_sourceModel->appendChatMessage(msg);
}

void ChatMessageView::appendMessages(const QList<Message> &msgs)
{
    m_sourceModel->appendChatMessages(msgs);
}

