#include "mailframe.h"
#include <QPixmap>
#include <QToolButton>

MailFrame::MailFrame(QWidget* parent) : QFrame(parent)
{
    main_layout = new QVBoxLayout();
    tool_layout = new QHBoxLayout();
    content_layout = new QHBoxLayout();
    end_layout = new QGridLayout();
    mail_id = -1;


    //Action setup
    next_act = new QAction(this);
    prev_act = new QAction(this);

    connect(next_act, &QAction::triggered, this, &MailFrame::nextHandle);
    connect(prev_act, &QAction::triggered, this, &MailFrame::prevHandle);

    //Tool setup
    reply_tool = new QPushButton("");
    forward_tool = new QPushButton("");
    tag_tool = new QPushButton("");
    trash_tool = new QPushButton("");
    option_tool = new QPushButton("");

    QString iconStyle = "QPushButton { background-color: #A1C6DF; border-style: none; }";
    reply_tool->setStyleSheet(iconStyle);
    forward_tool->setStyleSheet(iconStyle);
    tag_tool->setStyleSheet(iconStyle);
    trash_tool->setStyleSheet(iconStyle);
    option_tool->setStyleSheet(iconStyle);

    reply_tool->setIcon(QPixmap(":/images/reply.png"));
    forward_tool->setIcon(QPixmap(":/images/forward.png"));
    tag_tool->setIcon(QPixmap(":/images/tag.png"));
    trash_tool->setIcon(QPixmap(":/images/trash.png"));
    option_tool->setIcon(QPixmap(":/images/options.png"));

    reply_tool->setIconSize(QSize(30, 30));
    forward_tool->setIconSize(QSize(30, 30));
    tag_tool->setIconSize(QSize(30, 30));
    trash_tool->setIconSize(QSize(30, 30));
    option_tool->setIconSize(QSize(28, 30));

    reply_tool->setToolTip("Reply");
    forward_tool->setToolTip("Forward");
    tag_tool->setToolTip("Add tag to mail");
    trash_tool->setToolTip("Send to Trash");
    option_tool->setToolTip("Options");

    connect(reply_tool, &QAbstractButton::clicked, this, &MailFrame::replyHandle);
    connect(forward_tool, &QAbstractButton::clicked, this, &MailFrame::forwardHandle);
    connect(tag_tool, &QAbstractButton::clicked, this, &MailFrame::tagHandle);
    connect(trash_tool, &QAbstractButton::clicked, this, &MailFrame::trashHandle);

    tool_layout->addWidget(reply_tool);
    tool_layout->addWidget(forward_tool);
    tool_layout->addWidget(tag_tool);
    tool_layout->addWidget(trash_tool);
    tool_layout->addWidget(option_tool);


    //Content setup
    content_frame = new MailContent();
    content_frame->setFrameShape(QFrame::Box);
    content_frame->setStyleSheet("MailContent { background-color: white; margin: 5px 25px 5px 25px; }");
    content_frame->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    content_layout->addWidget(content_frame);


    //Bottom setup
    QString button_style = "* { background-color: white; border: 1px solid black; border-radius: 3; font-size: 16px; padding: 5px; }";
    button_style += "\n *:hover { background-color: #C0C0C0; }";

    forward_button = new QPushButton("Forward");
    reply_button = new QPushButton(" Reply");
    forward_button->setIcon(QPixmap(":/images/forward.png"));
    reply_button->setIcon(QPixmap(":/images/reply.png"));
    forward_button->setIconSize(QSize(18, 18));
    reply_button->setIconSize(QSize(20, 20));
    forward_button->setStyleSheet(button_style);
    reply_button->setStyleSheet(button_style);

    end_layout->addWidget(reply_button, 0, 0, Qt::AlignRight);
    end_layout->addWidget(forward_button, 0, 1, Qt::AlignLeft);

    connect(reply_button, &QAbstractButton::clicked, this, &MailFrame::replyHandle);
    connect(forward_button, &QAbstractButton::clicked, this, &MailFrame::forwardHandle);

    next_button = new QPushButton("");
    prev_button = new QPushButton("");
    next_button->setIcon(QPixmap(":/images/down.png"));
    prev_button->setIcon(QPixmap(":/images/up.png"));
    next_button->setIconSize(QSize(25,25));
    prev_button->setIconSize(QSize(25,25));
    next_button->setStyleSheet(button_style);
    prev_button->setStyleSheet(button_style);
    next_button->addAction(next_act);
    prev_button->addAction(prev_act);
    end_layout->addWidget(next_button, 1, 0, Qt::AlignRight);
    end_layout->addWidget(prev_button, 1, 1, Qt::AlignLeft);


    //Setup layout
    this->setLayout(main_layout);
    main_layout->addLayout(tool_layout);
    main_layout->addLayout(content_layout);
    main_layout->addLayout(end_layout);
}


MailFrame::~MailFrame() {}


void MailFrame::updateContent(const Inbox::MailData& data)
{
    mail_id = data.id;
    content_frame->setContent(data);
}


void MailFrame::replyHandle()
{
    emit replyTo(mail_id);
}

void MailFrame::forwardHandle()
{
    emit forwardMail(mail_id);
}

void MailFrame::tagHandle()
{
    emit tagClicked(mail_id);
}

void MailFrame::trashHandle()
{
    if (mail_id >= 0) {
        content_frame->clearContents();
        emit mailTrashed(mail_id);
        mail_id = -1;
    }
}

void MailFrame::nextHandle()
{
    emit nextMail(mail_id);
}

void MailFrame::prevHandle()
{
    emit prevMail(mail_id);
}
