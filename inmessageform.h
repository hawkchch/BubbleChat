#ifndef INMESSAGEFORM_H
#define INMESSAGEFORM_H

#include <QWidget>
#include "imessageform.h"

namespace Ui {
class InMessageForm;
}

class InMessageForm : public QWidget, IMessageForm
{
    Q_OBJECT

public:
    explicit InMessageForm(QWidget *parent = 0);
    ~InMessageForm();

    // IMessageForm
    virtual QObject *instance();
    virtual Message message() const;
    virtual void setMessage(const Message& msg);

    // QWidget
    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;


private:
    Ui::InMessageForm *ui;

    QWidget *m_contentWidget;
    BasicDef::MsgItemType m_msgType;
};

#endif // INMESSAGEFORM_H
