#ifndef INBOXDISPLAY_H
#define INBOXDISPLAY_H

#include <QObject>
#include <QWidget>
#include <QVector>
#include <QVBoxLayout>
#include "inbox.h"
#include "inboxitem.h"


class InboxDisplay : public QWidget
{
    Q_OBJECT
    QVector<InboxItem*>* mails;
    QVBoxLayout* layout;

public:
    InboxDisplay(QWidget* parent = nullptr);
    ~InboxDisplay();
    void setInbox(const QVector<MailSummary>& summary);
    void updateRead(int id);

public slots:
    void itemClicked(int id);

signals:
    void mailSelected(int id);
};

#endif // INBOXDISPLAY_H
