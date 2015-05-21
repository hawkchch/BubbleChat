#include "outmessageform.h"
#include "ui_outmessageform.h"
#include <QHBoxLayout>
#include "multitext.h"

OutMessageForm::OutMessageForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OutMessageForm)
{
    ui->setupUi(this);
}

OutMessageForm::~OutMessageForm()
{
    delete ui;
}

QObject *OutMessageForm::instance()
{
    return this;
}

Message OutMessageForm::message() const
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

void OutMessageForm::setMessage(const Message &msg)
{
    if ( msg.items().length() > 0 && msg.direction() == Message::MessageOut )
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
            m_contentWidget->setObjectName("outMultiText");
            m_contentWidget->setStyleSheet("#outMultiText{border-image: url(:/picture/pic/chat.png) 27 27 27 27;"
                                           "border-width: 27 27 27 27;"
                                           "padding: -25 -25 -25 -25;}");

            QHBoxLayout *hLayout = qobject_cast<QHBoxLayout*>(ui->bubble->layout());
            if(hLayout)
            {
                hLayout->insertWidget(1, m_contentWidget, 1);
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
}

QSize OutMessageForm::sizeHint() const
{
    QSize size;
    if(m_contentWidget)
    {
        size = m_contentWidget->sizeHint();
        size.setHeight(size.height()+25);
        //size.setHeight(size.height()+4*9+25+54);
    }

    return size;
}

QSize OutMessageForm::minimumSizeHint() const
{
    QSize size;
    if(m_contentWidget)
    {
        size = m_contentWidget->minimumSizeHint();
    }

    return size;
}
