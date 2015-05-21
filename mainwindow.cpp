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
    msgItem.count = 1;

    BasicDef::MsgItem msgItemImage;
    msgItemImage.type = BasicDef::MIT_IMAGE;
    msgItemImage.count = 1;

    BasicDef::MsgItem msgItemGif;
    msgItemGif.type = BasicDef::MIT_GIF;
    msgItemGif.count = 1;

    for(int i=0; i<10; i++)
    {
        msgItem.data = QString("%1 "
                               "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW").arg(i);
        msgItemImage.data = QString("D:\\mygit\\BubbleChat\\pic\\1.jpg");

        msgItemGif.data = ":/picture/pic/play.gif";

        msg.setItems(QList<BasicDef::MsgItem>{
                         msgItem
                         , msgItemImage
//                         , msgItemGif
//                         , msgItem
                         , msgItem
                          });
        msg.setDirection(i%2==0?Message::MessageIn:Message::MessageOut);
        ui->listView->appendMessage(msg);
    }


    //ui->textEdit->setMessage(msg);
}

MainWindow::~MainWindow()
{
    delete ui;
}
