#include "multitext.h"

#include <QFrame>
#include <QTextDocumentFragment>
#include <QAbstractTextDocumentLayout>
#include <QMimeData>
#include <QStandardPaths>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QFileDialog>
#include <QApplication>
#include <QClipboard>
#include <QAction>
#include <QMenu>
#include <QDateTime>
#include <QDebug>
#include <QtXml/QDomDocument>
#include <QTextCursor>
#include <QDebug>
#include "message.h"

static const int BASE_FACE_COUNT = 42;
static const int HUHOO_FACE_COUNT = 15;

MultiText::MultiText(const Message &msg, QWidget *parent) : MultiText(parent)
{
    setMessage(msg);
    qDebug() << msg.items().at(0).data;
}

MultiText::MultiText(QWidget *parent) : QTextEdit(parent)
{
    m_autoResize = false;
    m_minimumLines = 4;

    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    connect(this,SIGNAL(textChanged()),SLOT(onTextChanged()));

    clear();
    m_actionCopyImage = new QAction(tr("Copy image"), this);
    connect(m_actionCopyImage, SIGNAL(triggered()), SLOT(copyImage()));
    m_actionSaveIamge = new QAction(tr("Save Image"), this);
    connect(m_actionSaveIamge, SIGNAL(triggered()), SLOT(saveImage()));
    connect(this, SIGNAL(textChanged()), SLOT(slotTextChanged()));
    m_actionCopy = new QAction(tr("Copy"), this);
    m_actionCopy->setShortcut(QKeySequence::Copy);
    connect(m_actionCopy, SIGNAL(triggered()), SLOT(copy()));
    m_actionSelect = new QAction(tr("Select All"), this);
    m_actionSelect->setShortcut(QKeySequence::SelectAll);
    connect(m_actionSelect, SIGNAL(triggered()), SLOT(selectAll()));
    m_copyAvailable = false;
    connect(this, SIGNAL(copyAvailable(bool)), this, SLOT(setCopyAvailable(bool)));
   // setReadOnly(true);

    qDebug() << "MultiText";
}

MultiText::~MultiText()
{
    foreach(int id, m_facesMap.keys())
    {
        delete m_facesMap[id];
    }
    m_facesMap.clear();

    qDebug() << "~MultiText";
}


void MultiText::clear()
{
    m_lastPasteImg = QImage();
    m_pasteImgList.clear();
    QList<QMovie*> faces = m_facesMap.values();
    for(int i=0; i<faces.size(); i++)
    {
        delete faces[i];
    }
    m_facesMap.clear();
    QTextEdit::clear();
}

bool MultiText::autoResize() const
{
    return m_autoResize;
}

void MultiText::setAutoResize(bool AResize)
{
    if (AResize != m_autoResize)
    {
        m_autoResize = AResize;
        updateGeometry();
    }
}

int MultiText::minimumLines() const
{
    return m_minimumLines;
}

void MultiText::setMinimumLines(int ALines)
{
    if (ALines != m_minimumLines)
    {
        m_minimumLines = ALines>0 ? ALines : 1;
        updateGeometry();
    }
}

QSize MultiText::sizeHint() const
{
    QSize sh = QTextEdit::sizeHint();
    sh.setHeight(textHeight(!m_autoResize ? m_minimumLines : 0));
    //qDebug() << "MultiText::sizeHint "<<sh;
    return sh;
}

QSize MultiText::minimumSizeHint() const
{
    QSize sh = QTextEdit::minimumSizeHint();
    sh.setHeight(textHeight(m_minimumLines));
    return sh;
}

int MultiText::textHeight(int ALines) const
{
    if (ALines > 0)
    {
//        int fontHeight = fontMetrics().height()*ALines;
//        int marginHeight = (frameWidth() + qRound(document()->documentMargin()))*2;
//        qDebug() << "MultiText::textHeight " << fontHeight << marginHeight;
        return fontMetrics().height()*ALines + (frameWidth() + qRound(document()->documentMargin()))*2;
    }
    else
    {
        return qRound(document()->documentLayout()->documentSize().height()) + frameWidth()*2;
    }
}

bool MultiText::canInsertFromMimeData(const QMimeData *ASource) const
{
    if (ASource->hasImage() || ASource->hasText() || ASource->hasUrls()
        || QTextEdit::canInsertFromMimeData(ASource))
    {
        qDebug() << "MessageEditor can insert: true";
        return true;
    }
    qDebug() << "MessageEditor can insert: false";
    return false;
}

void MultiText::onTextChanged()
{
    updateGeometry();
}

void MultiText::insertFace(int idx, bool addImageElement /*= true*/)
{
    QString faceName = QString("%1/face%2.png").arg(m_baseFacesPath).arg(idx+1);
    if (addImageElement)
    {
        // 添加标签
        QTextImageFormat imageFormat;
        imageFormat.setName(QString("face%1").arg(idx));
        textCursor().insertImage(imageFormat);

        // 添加资源
        QUrl url(QString("face%1").arg(idx));
        QPixmap facePix(faceName);
        document()->addResource(QTextDocument::ImageResource, url, facePix);

        // 排版
        setLineWrapColumnOrWidth(lineWrapColumnOrWidth());
    }
}

void MultiText::insertHuhooFace(int idx, bool addImageElement /*= true*/)
{
    QString faceName = QString("%1/hu%2.gif").arg(m_huhooFacesPath).arg(idx+1);
    if (addImageElement)
    {
        QTextImageFormat imageFormat;
        imageFormat.setName(QString("bface%1").arg(idx));
        textCursor().insertImage(imageFormat);
    }

    if(!m_facesMap.contains(idx))
    {
        m_facesMap[idx] = new QMovie(faceName);
        connect(m_facesMap[idx], SIGNAL(frameChanged(int)), SLOT(updateFaceFrame(int)));
        m_facesMap[idx]->setCacheMode(QMovie::CacheNone);
        m_facesMap[idx]->start();
    }
}

void MultiText::mouseDoubleClickEvent(QMouseEvent *e)
{
    QTextEdit::mouseDoubleClickEvent(e);
    QTextCursor cursor = cursorForPosition(e->pos());
    QString fileName = documentImagePath(cursor.charFormat());
    if(fileName.isEmpty())
        return;
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}

QString MultiText::documentImagePath(QTextCharFormat tcf)
{
    if(!tcf.isImageFormat())
        return "";
    QString fileName = tcf.toImageFormat().name();
    QFileInfo fi(fileName);
    if(fi.exists())
        return fileName;
    return "";
}

void MultiText::setCopyAvailable(bool copyAvailable)
{
    m_copyAvailable = copyAvailable;
}

void MultiText::contextMenuEvent(QContextMenuEvent *event)
{
    qDebug() << "MessageEditor copy available: " << m_copyAvailable;
    qDebug() << "MessageEditor paste available: " << canPaste();
    QMenu *menu = createStandardContextMenu();
    menu->clear();
    if (m_copyAvailable)
    {
        m_actionCopy->setEnabled(true);
    }
    else
    {
        m_actionCopy->setEnabled(false);
    }
    m_actionSelect->setEnabled(false);
    if (document())
    {
        if (!document()->isEmpty())
            m_actionSelect->setEnabled(true);
    }

    menu->addAction(m_actionCopy);
    menu->addSeparator();
    menu->addAction(m_actionSelect);

    QTextCursor cursor = cursorForPosition(event->pos());
    QString imgPath = documentImagePath(cursor.charFormat());
    if(!imgPath.isEmpty())
    {
        menu->clear();
        m_actionCopyImage->setData(imgPath);
        menu->addSeparator();
        menu->addAction(m_actionCopyImage);
        m_actionSaveIamge->setData(imgPath);
        menu->addAction(m_actionSaveIamge);

    }
    menu->exec(event->globalPos());
    delete menu;
}

void MultiText::slotKeyPressed(QKeyEvent *e)
{
    this->setFocus();
    keyPressEvent(e);
}

void MultiText::slotTextChanged()
{
    if(toPlainText().isEmpty())
    {
        setCurrentFont(m_textFont);
        setTextColor(m_textColor);
        QTextCharFormat tcf;
        tcf.setFont(m_textFont);
        tcf.setForeground(m_textColor);
        textCursor().setCharFormat(tcf);
        setCurrentCharFormat(tcf);
    }
}


void MultiText::textCharFormatChanged(bool bBold, bool bItalic, bool bUnderline, const QString &fontFamily, int fontSize, const QColor &color)
{
    setStyleSheet(QString::fromUtf8("QTextEdit{"
        "font: %1pt;"
        "}")
        .arg(fontSize));
    m_textFont.setBold(bBold);
    m_textFont.setItalic(bItalic);
    m_textFont.setUnderline(bUnderline);
    m_textFont.setPointSize(fontSize);
    m_textFont.setFamily(fontFamily);
    m_textColor = color;
    selectAll();
    setCurrentFont(m_textFont);
    setTextColor(m_textColor);
    moveCursor(QTextCursor::End);
    QTextCharFormat tcf;
    tcf.setFont(m_textFont);
    tcf.setForeground(m_textColor);
    textCursor().setCharFormat(tcf);
    setCurrentCharFormat(tcf);
}

void MultiText::saveImage()
{
    QString fileName = m_actionSaveIamge->data().toString();
    QString suffixStr = "jpg";
    if(fileName.isEmpty())
        return;
    QImage img = QImage(fileName);
    if(img.isNull())
        return;
    QString defaultImageName = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + QString("/image.%1").arg(suffixStr);
    QString newName = QFileDialog::getSaveFileName(0, tr("Save Image"),
        defaultImageName,
        tr("Images (*.png *.gif *.jpg)"));
    if(newName.isEmpty())
        return;
    img.save(newName);
}

void MultiText::copyImage()
{
    QString fileName = m_actionSaveIamge->data().toString();
    if(fileName.isEmpty())
        return;
    QImage img = QImage(fileName);
    if(img.isNull())
        return;
    QMimeData *mimeData = new QMimeData;
    mimeData->setImageData(img);
    QApplication::clipboard()->setMimeData(mimeData);
}

void MultiText::updateFaceFrame(int /*a*/)
{
    QMovie* movie = qobject_cast<QMovie*>(sender());
    if(movie)
    {
        int idx = m_facesMap.key(movie);
        QString faceName = QString("bface%1").arg(idx);
        QUrl url(faceName);
        document()->addResource(QTextDocument::ImageResource,   //替换图片为当前帧
            url, movie->currentPixmap());
        setLineWrapColumnOrWidth(lineWrapColumnOrWidth());
    }
}


void MultiText::keyPressEvent(QKeyEvent *e)
{
    QTextEdit::keyPressEvent(e);
}

void MultiText::setEmoticonsPath(QString path)
{
    m_emotionsPath = path;
    m_baseFacesPath = m_emotionsPath + "/base";
    m_huhooFacesPath = m_emotionsPath + "/huhoo";
}

void MultiText::setMessage(const Message &msg)
{
    clear();
    m_message = msg;
    for(int i=0; i<msg.items().count(); i++)
    {
        QString emotion;
        bool    ok = false;
        int     emotionIndex = 0;
        QString localFileName;
        QString imageFile;
        QFile   file;
        auto item = msg.items().at(i);

        switch(item.type)
        {
        case BasicDef::MIT_TEXT:
            appendText(item.data);
            break;

        case BasicDef::MIT_IMAGE:
            localFileName = item.data;
            if(localFileName.contains("/"))
            {
                localFileName = localFileName.mid(localFileName.lastIndexOf("/") + 1);
            }
            imageFile = localFileName;//m_chatImagePath + "/" + localFileName;
            //QFile file(imageFile);
            file.setFileName(imageFile);
            if(file.exists())
            {
                //imgHtml = QString("<img src=\"%1\" />").arg(QUrl::fromLocalFile(imageFile).toString());
                appendImage(QUrl::fromLocalFile(imageFile).toString());
            }
//            else
//            {
//                QString imageId = QString("image_%1_%2").arg(QDateTime::currentMSecsSinceEpoch()).arg(QString::number(qrand()));
//                imgHtml = QString("<img src=%1 />").arg(imageId);
//                m_imageUrls[imageId] = QUrl::fromLocalFile(imageFile).toString();
//                imageIds.append(imageId);
//                //qDebug() << "AdiumStyleWidget image file not exeist! image id = " << imageId;
//                emit chatImageGet(item.data, imageId);
//            }
            break;

        case BasicDef::MIT_EMOTICONS:
            emotion = item.data;
            if (emotion.startsWith("face"))
            {
                emotion.remove("face");
            }
            emotionIndex = emotion.toInt(&ok);
            if (ok)
            {
                appendBaseFace(emotionIndex);
            }
            break;

        case BasicDef::MIT_GIF:
            emotion = item.data;
            if (emotion.startsWith("bface"))
            {
                emotion.remove("bface");
            }
            ok = false;
            emotionIndex = emotion.toInt(&ok);
            if (ok)
            {
                appendHuhooFace(emotionIndex);
            }
            break;

        default:
            break;
        }
    }

//    update();
//    updateGeometry();
}

Message MultiText::message() const
{
    return m_message;
}

void MultiText::appendText(const QString &text)
{
    QTextCursor cursor = textCursor();

    if (!cursor.atEnd())
    {
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
        setTextCursor(cursor);
    }

    cursor.insertText(text);
}

void MultiText::appendImage(const QString &src)
{
    QTextCursor cursor = textCursor();

    if (!cursor.atEnd())
    {
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
        setTextCursor(cursor);
    }

    QString html = QString("<img src=\"%1\" />").arg(src);
    cursor.insertHtml(html);
}

void MultiText::appendBaseFace(int idx)
{
    // 构造图片地址
    QString faceName = QString("%1/face%2.png").arg(m_baseFacesPath).arg(idx+1);

    // 定位游标尾部
    QTextCursor cursor = textCursor();
    if (!cursor.atEnd())
    {
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
        setTextCursor(cursor);
    }

    // 添加标签
    QTextImageFormat imageFormat;
    imageFormat.setName(QString("face%1").arg(idx));
    cursor.insertImage(imageFormat);

    // 添加资源
    QUrl url(QString("face%1").arg(idx));
    QPixmap facePix(faceName);
    document()->addResource(QTextDocument::ImageResource, url, facePix);

    // 排版
    setLineWrapColumnOrWidth(lineWrapColumnOrWidth());

}

void MultiText::appendHuhooFace(int idx)
{
    // 构造图片地址
    QString faceName = QString("%1/hu%2.gif").arg(m_huhooFacesPath).arg(idx+1);

    // 定位游标尾部
    QTextCursor cursor = textCursor();
    if (!cursor.atEnd())
    {
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
        setTextCursor(cursor);
    }

    // 添加标签
    QTextImageFormat imageFormat;
    imageFormat.setName(QString("bface%1").arg(idx));
    cursor.insertImage(imageFormat);

    if(!m_facesMap.contains(idx))
    {
        m_facesMap[idx] = new QMovie(faceName);
        connect(m_facesMap[idx], SIGNAL(frameChanged(int)), SLOT(updateFaceFrame(int)));
        m_facesMap[idx]->setCacheMode(QMovie::CacheNone);
        m_facesMap[idx]->start();
    }
}

