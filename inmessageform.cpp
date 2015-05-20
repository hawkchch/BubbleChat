#include "inmessageform.h"
#include "ui_inmessageform.h"
#include <QHBoxLayout>
#include <qevent.h>
#include <QDebug>
#include <QTextDocument>
#include <QTextEdit>
#include <QPainter>
#include "multitext.h"

InMessageForm::InMessageForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InMessageForm)
{
    ui->setupUi(this);
}

InMessageForm::~InMessageForm()
{
    delete ui;
}

QObject *InMessageForm::instance()
{
    return this;
}

Message InMessageForm::message() const
{
    if(m_contentWidget)
    {
        switch (m_msgType)
        {
        case BasicDef::MIT_NONE:
            break;
        case BasicDef::MIT_TEXT:
        case BasicDef::MIT_IMAGE:
        case BasicDef::MIT_GIF:
        case BasicDef::MIT_EMOTICONS:
        {
            auto widget = qobject_cast<MultiText*>(m_contentWidget);
            if(widget)
            {
                return widget->message();
            }

            break;
        }

        case BasicDef::MIT_VOICE:
            break;

        case BasicDef::MIT_OFFLINEFILE:
            break;

        default:
            break;
        }
    }

    return Message();
}

void InMessageForm::setMessage(const Message &msg)
{
    if ( msg.items().length() > 0 && msg.direction() == Message::MessageIn )
    {
        m_msgType = msg.items().at(0).type;

        switch (m_msgType)
        {
        case BasicDef::MIT_NONE:
            break;
        case BasicDef::MIT_TEXT:
        case BasicDef::MIT_IMAGE:
        case BasicDef::MIT_GIF:
        case BasicDef::MIT_EMOTICONS:
        {
            m_contentWidget = new MultiText(msg, this);
            m_contentWidget->setStyleSheet("border-image: url(:/picture/pic/chat.png) 27 27 27 27;"
                                           "border-width: 27 27 27 27;");

            QHBoxLayout *hLayout = qobject_cast<QHBoxLayout*>(ui->bubble->layout());
            if(hLayout)
            {
                hLayout->insertWidget(0, m_contentWidget, 1);
            }

            break;
        }

        case BasicDef::MIT_VOICE:
            break;

        case BasicDef::MIT_OFFLINEFILE:
            break;

        default:
            break;
        }
    }

    updateGeometry();
}

QSize InMessageForm::sizeHint() const
{
    QSize size;
    if(m_contentWidget)
    {
        size = m_contentWidget->sizeHint();
        size.setHeight(size.height()+4*9+25 + 54);
    }

    return size;
}

QSize InMessageForm::minimumSizeHint() const
{
    QSize size;
    if(m_contentWidget)
    {
        size = m_contentWidget->minimumSizeHint();
    }

    return size;
}

void InMessageForm::paintEvent(QPaintEvent *e)
{
    QStyleOption opt;

    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(e);
}
