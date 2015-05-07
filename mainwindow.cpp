#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chatmessageview.h"
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Message msg;
    BasicDef::MsgItem msgItem;
    msgItem.type = BasicDef::MIT_TEXT;
    msgItem.data = "Hello World!";
    msgItem.count = 1;
    msg.setItems(QList<BasicDef::MsgItem>{msgItem});

    //ui->textEdit->setMessage(msg);
    ui->listView->appendMessage(msg);

    msgItem.data = "HHHHHH WTF!!!";
    msg.setItems(QList<BasicDef::MsgItem>{msgItem});
    ui->listView->appendMessage(msg);

}

MainWindow::~MainWindow()
{
    delete ui;
}
