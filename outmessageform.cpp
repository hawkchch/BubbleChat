#include "outmessageform.h"
#include "ui_outmessageform.h"

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
