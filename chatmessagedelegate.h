#ifndef CHATMESSAGEDELEGATE_H
#define CHATMESSAGEDELEGATE_H

#include <QStyledItemDelegate>
#include <QItemDelegate>


class ChatMessageView;

class ChatMessageDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ChatMessageDelegate(QObject *parent = 0);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;

    QWidget *createEditor(QWidget *parent,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const;

    void setEditorData(QWidget *editor,
                       const QModelIndex &index) const;

    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const;

//    void updateEditorGeometry(QWidget *editor,
//                              const QStyleOptionViewItem &option,
//                              const QModelIndex &index) const;

signals:
    void indexSize(QModelIndex, QSize);

public slots:

private:
    ChatMessageView *m_parent;

    mutable QMap<QModelIndex, QSize> m_mapHeight;

};

#endif // CHATMESSAGEDELEGATE_H
