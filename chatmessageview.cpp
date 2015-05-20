#include "chatmessageview.h"
#include <qdebug.h>
#include <QScrollBar>
#include <QWheelEvent>
#include <QApplication>

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

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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

void ChatMessageView::wheelEvent(QWheelEvent *event)
{
    return;
    qDebug() << "ChatMessageView::wheelEvent";
    // 暂时鼠标滚轮有问题
    if(event->orientation() == Qt::Vertical)
    {
        QPoint numPixels = event->pixelDelta();
        QPoint numDegrees = event->angleDelta()/8;


        if (!numPixels.isNull()) {
            //qDebug() << "pixelDelta";
            scrollContentsBy(0, numPixels.y());
            verticalScrollBar()->setSliderPosition(verticalScrollBar()->sliderPosition() - numPixels.y());
        } else if (!numDegrees.isNull()) {
            //qDebug() << "angleDelta" << numDegrees;
            QPoint numSteps = numDegrees / 15;
            //qDebug() << numSteps;
            scrollContentsBy(0, numSteps.y());
            verticalScrollBar()->setSliderPosition(verticalScrollBar()->sliderPosition() - numSteps.y());
        }

        event->accept();
    }
}

