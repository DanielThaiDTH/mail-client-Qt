#ifndef INBOXITEM_H
#define INBOXITEM_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QFrame>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QEnterEvent>
#include <QMouseEvent>
#include <QPixmap>
#include <QIcon>

class InboxItem : public QFrame
{
    Q_OBJECT
    int mail_id;
    bool isRead;
    bool selected;
    bool isHovered;

    QCheckBox* select_check;
    QString subject;
    QString sender_addr;
    QLabel* subject_label;
    QLabel* sender_label;
    QLabel* attach_label;
    QLabel* read_label;
    QString subjectUnreadStyle = "* { font-weight: bold; font-size: 15px; color: #1D43BF; margin: 0px; }";
    QString subjectReadStyle = "* { font-weight: normal; font-size: 15px; color: #243B88; margin: 0px; }";
    QString senderStyle = "* { font-style: italic; color: #707070; margin: 0px; }";
    QString style;

    QHBoxLayout* layout;

public:
    InboxItem(int id, bool read, QString subject, QString sender, bool hasAttach, QWidget* parent = nullptr);
    ~InboxItem();
    void enterEvent(QEnterEvent* e) override;
    void leaveEvent(QEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
    int getID() const;
    void setReadState(bool state);
    void clearCurrentRead();

public slots:
    void checkChanged(int value);

signals:
    void mailClicked(int id);
};

#endif // INBOXITEM_H
