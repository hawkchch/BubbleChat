#include "inmessageform.h"
#include "ui_inmessageform.h"
#include <QHBoxLayout>

InMessageForm::InMessageForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InMessageForm)
{
    ui->setupUi(this);
    m_contentWidget = new MultiText(parent);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(m_contentWidget);
    m_contentWidget->setAutoFillBackground(true);
    ui->inContentWidget->setLayout(layout);
}

InMessageForm::~InMessageForm()
{
    delete ui;
}

Message InMessageForm::message() const
{
    if(m_contentWidget)
    {
        return m_contentWidget->message();
    }

    return Message();
}

void InMessageForm::setMessage(const Message &msg)
{
    if(m_contentWidget)
    {
        m_contentWidget->setMessage(msg);
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
