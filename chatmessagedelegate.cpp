#include "chatmessagedelegate.h"
#include "chatmessageitem.h"
#include "imessageform.h"
#include "inmessageform.h"
#include "outmessageform.h"
#include <QPainter>
#include <QPushButton>
#include <QDebug>
#include "chatmessageview.h"
#include <QApplication>
#include <QLabel>
#include <QHBoxLayout>

ChatMessageDelegate::ChatMessageDelegate(QObject *parent) :
    QItemDelegate(parent)
{
    m_parent = (ChatMessageView* )parent;
    connect(&m_timerCheck, SIGNAL(timeout()), this, SLOT(timerCheck()));
    m_timerCheck.start(1000);
}

void ChatMessageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    if (index.data(ChatMessageItem::DATA_ROLE_MESSAGE).canConvert<Message>())
    {
        if (m_parent)
        {
            m_parent->openPersistentEditor(index);

            QRect parentRect = m_parent->rect();

            int viewStartHeight = parentRect.y();
            int viewEndHeight = parentRect.y() + parentRect.height();
            for(int row = 0; row < m_parent->model()->rowCount(); row++)
            {
                QModelIndex idx = m_parent->model()->index(row, 0);
                QRect rc = m_parent->visualRect(idx);
                int indexStartHeight = rc.y();
                if ( indexStartHeight <= viewStartHeight - rc.height()
                     || indexStartHeight >= viewEndHeight + rc.height() )
                {
                    if(m_mapIndexClock[index] >= 60)
                    {
                        m_parent->closePersistentEditor(idx);
                        m_mapIndexClock.remove(idx);
                    }
                }
            }
        }
    }
    else
    {
        QItemDelegate::paint(painter, option, index);
    }
    painter->restore();
}

QSize ChatMessageDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if ( index.isValid() && index.data(ChatMessageItem::DATA_ROLE_MESSAGE).canConvert<Message>() )
    {
        if ( index.data(ChatMessageItem::DATA_ROLE_MESSAGE).canConvert<Message>() )
        {
            if (m_mapEditorSize.find(index) != m_mapEditorSize.end() )
            {
                return m_mapEditorSize[index];
            }
        }
    }

    return QItemDelegate::sizeHint(option, index);
}

QWidget *ChatMessageDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if ( index.data(ChatMessageItem::DATA_ROLE_MESSAGE).canConvert<Message>() )
    {
        auto msg = index.data(ChatMessageItem::DATA_ROLE_MESSAGE).value<Message>();
        if(msg.direction() == Message::MessageIn)
        {
            auto in = new InMessageForm(parent);
            in->setMessage(msg);
            m_mapEditorSize[index] = in->sizeHint();
            m_mapIndexClock[index] = 0;
            return in;
        }
        else if(msg.direction() == Message::MessageOut)
        {
            auto out = new OutMessageForm(parent);
            out->setMessage(msg);
            m_mapEditorSize[index] = out->sizeHint();
            m_mapIndexClock[index] = 0;
            return out;
        }
    }

    return QItemDelegate::createEditor(parent, option, index);
}

void ChatMessageDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if ( index.data(ChatMessageItem::DATA_ROLE_MESSAGE).canConvert<Message>() )
    {
        auto msg = index.data(ChatMessageItem::DATA_ROLE_MESSAGE).value<Message>();

        if(msg.direction() == Message::MessageIn)
        {
            auto widget = qobject_cast<InMessageForm*>(editor);

            if (widget)
            {
                widget->setMessage(msg);
            }
        }
        else
        {
            auto widget = qobject_cast<OutMessageForm*>(editor);

            if (widget)
            {
                widget->setMessage(msg);
            }
        }
    }
    else
    {
        QItemDelegate::setEditorData(editor, index);
    }
}

void ChatMessageDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if ( index.data(ChatMessageItem::DATA_ROLE_MESSAGE).canConvert<Message>() )
    {
        auto msg = index.data(ChatMessageItem::DATA_ROLE_MESSAGE).value<Message>();

        if(msg.direction() == Message::MessageIn)
        {
            auto widget = qobject_cast<InMessageForm*>(editor);

            if (widget)
            {
                model->setData(index, QVariant::fromValue<Message>(widget->message()));
            }
        }
        else
        {
            auto widget = qobject_cast<OutMessageForm*>(editor);

            if (widget)
            {
                model->setData(index, QVariant::fromValue<Message>(widget->message()));
            }
        }
    }
    else
    {
        QItemDelegate::setModelData(editor, model, index);
    }
}

bool ChatMessageDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    QItemDelegate::editorEvent(event, model, option, index);
}

void ChatMessageDelegate::timerCheck()
{
    foreach (QModelIndex index, m_mapIndexClock.keys())
    {
        m_mapIndexClock[index]++;
//        if( m_parent && m_mapIndexClock[index] >= 5)
//        {
//            m_parent->closePersistentEditor(index);
//            m_mapIndexClock.remove(index);
//        }
    }
}
