#ifndef CHATMESSAGEVIEW_H
#define CHATMESSAGEVIEW_H

#include <QWidget>
#include <QListView>
#include "chatmessageitem.h"
#include "chatmessagemodel.h"
#include "chatmessagedelegate.h"
#include "message.h"

class ChatMessageView : public QListView
{
public:
    explicit ChatMessageView(QWidget *parent = 0);

    ~ChatMessageView();

    void appendMessage(const Message& msg);
    void appendMessages(const QList<Message>& msgs);


signals:

public slots:

private:
    ChatMessageModel *m_sourceModel;
    ChatMessageSortFilterProxyModel *m_sortModel;

};

#endif // CHATMESSAGEVIEW_H
