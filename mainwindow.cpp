#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chatmessageview.h"
#include <QModelIndex>
#include <QAbstractItemModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Message msg;
    BasicDef::MsgItem msgItem;
    msgItem.type = BasicDef::MIT_TEXT;
    msgItem.data = "Hello World!";
    msgItem.count = msgItem.data.count();
    msg.setItems(QList<BasicDef::MsgItem>{msgItem});

    ui->listView->appendMessage(msg);
}

MainWindow::~MainWindow()
{
    delete ui;
}
