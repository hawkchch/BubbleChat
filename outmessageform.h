#ifndef OUTMESSAGEFORM_H
#define OUTMESSAGEFORM_H

#include <QWidget>
#include "imessageform.h"

namespace Ui {
class OutMessageForm;
}

class OutMessageForm : public QWidget
{
    Q_OBJECT

public:
    explicit OutMessageForm(QWidget *parent = 0);
    ~OutMessageForm();
    // IMessageForm
    virtual QObject *instance();
    virtual Message message() const;
    virtual void setMessage(const Message& msg);

    // QWidget
    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;

private:
    Ui::OutMessageForm *ui;
    QWidget *m_contentWidget;
    BasicDef::MsgItemType m_msgType;
};

#endif // OUTMESSAGEFORM_H
