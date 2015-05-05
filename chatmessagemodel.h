#ifndef CHATMESSAGEMODEL_H
#define CHATMESSAGEMODEL_H

#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include "message.h"

class ChatMessageSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ChatMessageSortFilterProxyModel(QObject *parent = 0);
    void setFilterString(const QString& filterString);

protected:
    virtual bool filterAcceptsRow(int AModelRow, const QModelIndex &AModelParent) const;
    virtual bool lessThan(const QModelIndex &ALeft, const QModelIndex &ARight) const;

private:
    QString m_filterString;
};






class ChatMessageModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit ChatMessageModel(QObject *parent = 0);

signals:

public slots:
    void appendChatMessage(const Message& msg);
    void appendChatMessages(const QList<Message>& msgs);
};

#endif // CHATMESSAGEMODEL_H
