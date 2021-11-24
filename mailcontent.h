#ifndef MAILCONTENT_H
#define MAILCONTENT_H

#include <QObject>
#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include "Email.h"
#include "inbox.h"
#include "collapsablesection.h"

class MailContent : public QFrame
{
    Q_OBJECT
    int id;
    QLabel* subject;
    QLabel* date;
    QLabel* from;
    QLabel* to;
    QLabel* text;

    CollapsableSection* responses;
    QVector<QPushButton*> attachments;
    QGridLayout* attachment_layout;
    QFrame* attachment_frame;
    QVBoxLayout* layout;

    QFrame* line1;
    QFrame* line2;


public:
    MailContent(QWidget* parent = nullptr);
    ~MailContent();
    void setContent(const Inbox::MailData& mail);
    void clearContents();
};

#endif // MAILCONTENT_H
