#ifndef MAILFRAME_H
#define MAILFRAME_H

#include <QObject>
#include <QWidget>
#include <QFrame>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include "mailcontent.h"

class MailFrame : public QFrame
{
    Q_OBJECT
    QVBoxLayout* main_layout;
    QHBoxLayout* tool_layout;
    QHBoxLayout* content_layout;
    QGridLayout* end_layout;

    MailContent* content_frame;
    //QScrollArea* content_scroll;

    QPushButton* reply_tool;
    QPushButton* forward_tool;
    QPushButton* tag_tool;
    QPushButton* trash_tool;
    QPushButton* option_tool;
    QPushButton* reply_button;
    QPushButton* forward_button;
    QPushButton* prev_button;
    QPushButton* next_button;

public:
    MailFrame(QWidget* parent = nullptr);
    ~MailFrame();

public slots:
    void updateContent(const Inbox::MailData& data);
};

#endif // MAILFRAME_H
