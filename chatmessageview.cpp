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

//void ChatMessageView::wheelEvent(QWheelEvent *event)
//{
//    qDebug() << "ChatMessageView::wheelEvent" << event->pos() << event->globalPos();
//    // 暂时鼠标滚轮有问题
//    if(event->orientation() == Qt::Vertical)
//    {
//        QPoint numPixels = event->pixelDelta();
//        QPoint numDegrees = event->angleDelta()/8;

//        qDebug() << contentsRect() << verticalScrollBar()->minimum() << verticalScrollBar()->maximum();

//        if (!numPixels.isNull()) {
//            scrollContentsBy(0, numPixels.y());
//            verticalScrollBar()->setSliderPosition(verticalScrollBar()->sliderPosition() - numPixels.y());
//        } else if (!numDegrees.isNull()) {
//            //QPoint numSteps = numDegrees / 15;

//            int min = verticalScrollBar()->minimum();
//            int max = verticalScrollBar()->maximum();
//            double sdy = (numDegrees.y()*1.0)/contentsSize().height()*(max-min);

//            scrollContentsBy(0, numDegrees.y());
//            verticalScrollBar()->setSliderPosition(verticalScrollBar()->sliderPosition() - sdy);
//        }

//        event->accept();
//    }
//}

