#include "outmessageform.h"
#include "ui_outmessageform.h"
#include <QHBoxLayout>
#include "multitext.h"

OutMessageForm::OutMessageForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OutMessageForm)
{
    ui->setupUi(this);

    m_contentWidget = nullptr;
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    ui->outContentWidget->setLayout(layout);
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
        if ( m_contentWidget
            && ui->outContentWidget->layout() != nullptr )
        {
            ui->outContentWidget->layout()->removeWidget(m_contentWidget);
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
            auto widget = new MultiText(msg, this);
            m_contentWidget = widget;
            ui->outContentWidget->layout()->addWidget(m_contentWidget);

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
        size.setHeight(size.height()+9*5);
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
