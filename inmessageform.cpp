#include "inmessageform.h"
#include "ui_inmessageform.h"
#include <QHBoxLayout>

#include "multitext.h"

InMessageForm::InMessageForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InMessageForm)
{
    ui->setupUi(this);

    m_contentWidget = nullptr;
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    ui->inContentWidget->setLayout(layout);
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
        if ( m_contentWidget
            && ui->inContentWidget->layout() != nullptr )
        {
            ui->inContentWidget->layout()->removeWidget(m_contentWidget);
            delete m_contentWidget;
            m_contentWidget = nullptr;
        }

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
            ui->inContentWidget->layout()->addWidget(m_contentWidget);
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

QSize InMessageForm::sizeHint() const
{
    QSize size;
    if(m_contentWidget)
    {
        size = m_contentWidget->sizeHint();
        size.setHeight(size.height()+9*5);
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
