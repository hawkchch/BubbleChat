#ifndef INMESSAGEFORM_H
#define INMESSAGEFORM_H

#include <QWidget>
#include "imessageform.h"
#include "multitext.h"

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
    virtual Message message() const;
    virtual void setMessage(const Message& msg);

    // QWidget
    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;


private:
    Ui::InMessageForm *ui;

    MultiText *m_contentWidget;
};

#endif // INMESSAGEFORM_H
