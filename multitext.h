#ifndef MESSAGEEDITOR_H
#define MESSAGEEDITOR_H
#include <QTextEdit>
#include <QMovie>
#include "message.h"

class MultiText : public QTextEdit
{
    Q_OBJECT
public:
    MultiText(const Message& msg, QWidget *parent);
    MultiText(QWidget *parent);
    ~MultiText();

    void setMessage(const Message& msg);
    Message message() const;

    bool autoResize() const;
    void setAutoResize(bool AResize);
    int minimumLines() const;
    void setMinimumLines(int ALines);
    void clear();
    QStringList pasteImgFilenames(){return m_pasteImgList;}
    void setEmoticonsPath(QString path);


public slots:
    void insertFace(int idx, bool addImgElement = true);
    void insertHuhooFace(int idx, bool addImgElement = true);
    void updateFaceFrame(int a);
    void copyImage();
    void saveImage();
    void slotKeyPressed(QKeyEvent *e);
    void slotTextChanged();
    void textCharFormatChanged(bool bBold, bool bItalic, bool bUnderline, const QString& fontFamily, int fontSize, const QColor& color);
    void setCopyAvailable(bool copyAvailable);

signals:
    void createDataRequest(QMimeData *ADestination) const;
    void canInsertDataRequest(const QMimeData *AData, bool &ACanInsert) const;
    void insertDataRequest(const QMimeData *AData, QTextDocument *ADocument) const;
    void sendMessage();
    void sendFile(const QString &fileName);

public:
    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;

protected:
    int textHeight(int ALines = 0) const;
    QString documentImagePath(QTextCharFormat tcf);

    virtual bool canInsertFromMimeData(const QMimeData *ASource) const;
    void mouseDoubleClickEvent(QMouseEvent *e);
    void contextMenuEvent(QContextMenuEvent *event);
    void keyPressEvent(QKeyEvent *e);

protected slots:
    void onTextChanged();

private:
    void appendText(const QString& text);
    void appendImage(const QString& src);
    void appendBaseFace(int idx);
    void appendHuhooFace(int idx);

private:
    bool m_autoResize;
    int  m_minimumLines;
    QAction* m_actionCopyImage;
    QAction* m_actionSaveIamge;
    QAction* m_actionCopy;
    QAction* m_actionSelect;

    QStringList  m_pasteImgList;
    QImage m_lastPasteImg;
    QFont  m_textFont;
    QColor m_textColor;
    QMap<int, QMovie*> m_facesMap;
    bool m_copyAvailable;
    QString m_emotionsPath;
    QString m_baseFacesPath;
    QString m_huhooFacesPath;

    Message m_message;
};

#endif // MESSAGEEDITOR_H
