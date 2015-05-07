#include "chatmessagedelegate.h"
#include "chatmessageitem.h"
#include "multitext.h"
#include <QPainter>
#include <QPushButton>
#include "chatmessageview.h"

ChatMessageDelegate::ChatMessageDelegate(QObject *parent) :
    QItemDelegate(parent)
{
    m_parent = (ChatMessageView* )parent;
}

void ChatMessageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
/*
    // here we have active painter provided by caller

    // by the way - we can't use painter->save() and painter->restore()
    // methods cause we have to call painter->end() method before painting
    // the QWidget, and painter->end() method deletes
    // the saved parameters of painter

    // we have to save paint device of the provided painter to restore the painter
    // after drawing QWidget
    QPaintDevice* original_pdev_ptr = painter->device();

    // example of simple drawing (selection) before widget
    if (option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.highlight());

    // creating local QWidget (that's why i think it should be fasted, cause we
    // don't touch the heap and don't deal with a QWidget except painting)
    QPushButton item_widget;

    item_widget.setText("Hello");

    // Setting some parameters for widget for example
        // spec. params
    //item_widget.SetPackageName(index.data(Qt::DisplayRole).toString());
        // geometry
    item_widget.setGeometry(option.rect);

    // here we have to finish the painting of provided painter, cause
    //     1) QWidget::render(QPainter *,...) doesn't work with provided external painter
    //          and we have to use QWidget::render(QPaintDevice *,...)
    //          which creates its own painter
    //     2) two painters can't work with the same QPaintDevice at the same time
    painter->end();

    // rendering of QWidget itself
    item_widget.render(painter->device(), QPoint(option.rect.x(), option.rect.y()), QRegion(0, 0, option.rect.width(), option.rect.height()), QWidget::DrawChildren);

    // starting (in fact just continuing) painting with external painter, provided
    // by caller
    painter->begin(original_pdev_ptr);

    // example of simple painting after widget
    //painter->drawEllipse(0,0, 10,10);
*/

    if (index.data(ChatMessageItem::DATA_ROLE_MESSAGE).canConvert<Message>())
    {
        if (m_parent)
        {
            m_parent->openPersistentEditor(index);
        }
    }
    else
    {
        QItemDelegate::paint(painter, option, index);
    }
}

QSize ChatMessageDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if ( index.data(ChatMessageItem::DATA_ROLE_MESSAGE).canConvert<Message>() )
    {
        return option.widget->sizeHint();
    }
    else
    {
        return QItemDelegate::sizeHint(option, index);
    }
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
            widget = new MultiText(msg, m_parent);
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
    if ( index.data(ChatMessageItem::DATA_ROLE_MESSAGE).canConvert<Message>() )
    {
        auto msg = index.data(ChatMessageItem::DATA_ROLE_MESSAGE).value<Message>();

        MultiText* text = qobject_cast<MultiText*>(editor);

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

//void ChatMessageDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
//{
//    if ( index.data(ChatMessageItem::DATA_ROLE_MESSAGE).canConvert<Message>() )
//    {
//        MultiText* text = qobject_cast<MultiText*>(editor);

//        if (text)
//        {
//            model->setData(index, QVariant::fromValue<Message>(text->message()));
//        }
//    }
//    else
//    {
//        QItemDelegate::setModelData(editor, model, index);
//    }
//}
