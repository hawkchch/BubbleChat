#ifndef IMESSAGEFORM_H
#define IMESSAGEFORM_H
#include <functional>
#include <QMap>
#include "msgdef.h"

#include "message.h"

class IMessageForm
{
public:
    virtual QObject *instance() = 0;
    virtual Message message() const = 0;
    virtual void setMessage(const Message& msg) = 0;
};


class IMessageWidget : public QWidget
{
    Q_OBJECT
public:
    IMessageWidget(QWidget *parent){}
    ~IMessageWidget(){}
    virtual Message message() const{}
    virtual void setMessage(const Message& msg){}
};


class IMessageWidgetFactory
{
public:

    static void Register(const BasicDef::MsgItemType &type, std::function<IMessageWidget*(QWidget*)> cb)
    {
        m_creators[type] = cb;
    }

    static void Unregister(const BasicDef::MsgItemType &type)
    {
        m_creators.remove(type);
    }

    static IMessageWidget *Create(const BasicDef::MsgItemType &type, QWidget* parent)
    {
        if(m_creators.contains(type))
        {
            return m_creators[type](parent);
        }

        return nullptr;
    }

private:
    static QMap<BasicDef::MsgItemType, std::function<IMessageWidget*(QWidget*)> > m_creators;
};

#endif // IMESSAGEFORM_H
