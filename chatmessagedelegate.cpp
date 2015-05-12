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
                    m_parent->closePersistentEditor(idx);
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
            QSize sz;
            sz.setWidth(option.rect.width());
            if (m_mapEditorSize.find(index) != m_mapEditorSize.end() )
            {
                sz.setHeight(m_mapEditorSize[index].height());
                sz.setWidth(m_mapEditorSize[index].width());

                return sz;
            }
        }
    }

    return QItemDelegate::sizeHint(option, index);
}

QWidget *ChatMessageDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    QWidget *widget = nullptr;
    InMessageForm *in = nullptr;
    OutMessageForm *out = nullptr;

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
            in = new InMessageForm(parent);
            in->setMessage(msg);
            m_mapEditorSize[index] = in->sizeHint();
            m_mapIndexClock[index] = 0;
            widget = in;
            break;

        case BasicDef::MIT_VOICE:
            break;

        case BasicDef::MIT_OFFLINEFILE:
            break;

        default:
            break;
        }
    }

    if(in != nullptr)
    {
//        QLabel *avatar = new QLabel;
//        avatar->setPixmap(QPixmap(QString(":/picture/pic/5.jpg")));


//        QWidget *frame = new QWidget(m_parent);
//        frame->setLayout(new QHBoxLayout);
//        frame->layout()->addWidget(widget);
//        frame->layout()->addWidget(avatar);

//        return frame;

        return in;
    }
    else
    {
        return QItemDelegate::createEditor(parent, option, index);
    }
}

void ChatMessageDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if ( index.data(ChatMessageItem::DATA_ROLE_MESSAGE).canConvert<Message>() )
    {
        auto msg = index.data(ChatMessageItem::DATA_ROLE_MESSAGE).value<Message>();

        InMessageForm* text = qobject_cast<InMessageForm*>(editor);

        if (text)
        {
            text->setMessage(msg);
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
        InMessageForm* text = qobject_cast<InMessageForm*>(editor);

        if (text)
        {
            model->setData(index, QVariant::fromValue<Message>(text->message()));
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
//    foreach (QModelIndex index, m_mapIndexClock.keys())
//    {
//        m_mapIndexClock[index]++;
//        if( m_parent && m_mapIndexClock[index] >= 5)
//        {
//            m_parent->closePersistentEditor(index);
//            m_mapIndexClock.remove(index);
//        }
//    }
}
