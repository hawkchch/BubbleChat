#ifndef OUTMESSAGEFORM_H
#define OUTMESSAGEFORM_H

#include <QWidget>

namespace Ui {
class OutMessageForm;
}

class OutMessageForm : public QWidget
{
    Q_OBJECT

public:
    explicit OutMessageForm(QWidget *parent = 0);
    ~OutMessageForm();

private:
    Ui::OutMessageForm *ui;
};

#endif // OUTMESSAGEFORM_H
