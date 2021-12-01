#include "inboxitem.h"
#include <iostream>

InboxItem::InboxItem(int id, bool read, QString subject, QString sender, bool hasAttach, QWidget* parent) : QFrame(parent)
{
    this->setObjectName("inbox-item");
    mail_id = id;
    isRead = read;
    this->subject = subject;
    sender_addr = sender;
    selected = false;
    isHovered = false;

    select_check = new QCheckBox();
    select_check->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    select_check->setMinimumSize(20, 20);

    sender_label = new QLabel(sender_addr, this);
    subject_label = new QLabel(this->subject, this);
    subject_label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);

    if (isRead)
        subject_label->setStyleSheet(subjectReadStyle);
    else
        subject_label->setStyleSheet(subjectUnreadStyle);

    sender_label->setStyleSheet(senderStyle);

    read_label = new QLabel("");
    attach_label = new QLabel("");
    attach_label->setStyleSheet("* { margin-left: 5px; }");

    read_label->setPixmap(QPixmap(":/images/rec.png"));
    read_label->setMaximumSize(20, 20);
    read_label->setScaledContents(true);

    attach_label->setPixmap(QPixmap(":/images/clip.png"));
    attach_label->setScaledContents(true);
    attach_label->setMaximumSize(35, 40);

    layout = new QHBoxLayout(this);
    layout->addWidget(select_check);
    layout->addWidget(read_label);
    layout->addWidget(subject_label);
    layout->addWidget(sender_label);
    layout->addWidget(attach_label);
    this->setLayout(layout);

    read_label->hide();

    if (!hasAttach)
        attach_label->hide();

    style = "InboxItem#inbox-item { background-color: white; border-top: 1px solid #707070; \n";
    style += "border-bottom: 1px solid #707070; margin: 0px; padding: 0px; }\n";
    this->setStyleSheet(style);

    connect(select_check, &QCheckBox::stateChanged, this, &InboxItem::checkChanged);

    setMouseTracking(true);
}


InboxItem::~InboxItem() {}


void InboxItem::enterEvent(QEnterEvent* e)
{
    QString hoverStyle = "* { background-color: #DFF7F5; }\n";
    hoverStyle += "InboxItem#inbox-item{ border-top: 1px solid black; border-bottom: 1px solid black; margin: 0px; padding: 0px;}";
    this->setStyleSheet(hoverStyle);
    isHovered = true;
}


void InboxItem::leaveEvent(QEvent *e)
{
    if (!selected)
        this->setStyleSheet(style);
    isHovered = false;
}


void InboxItem::checkChanged(int value)
{
    if (value == Qt::CheckState::Checked) {
        QString hoverStyle = "* { background-color: #DFF7F5; }\n";
        hoverStyle += "InboxItem#inbox-item{ border-top: 1px solid black; border-bottom: 1px solid black; margin: 0px; padding: 0px;}";
        selected = true;
        this->setStyleSheet(hoverStyle);
    } else {
        selected = false;
        if (!isHovered)
            this->setStyleSheet(style);
    }
}


void InboxItem::mousePressEvent(QMouseEvent* e)
{
    if (rect().contains(e->pos())) {
        isRead = true;
        read_label->show();
        subject_label->setStyleSheet(subjectReadStyle);
        emit mailClicked(mail_id);
    }
}


int InboxItem::getID() const
{
    return mail_id;
}


void InboxItem::setReadState(bool state)
{
    isRead = state;
    read_label->show();

    if (isRead)
        subject_label->setStyleSheet(subjectReadStyle);
    else
        subject_label->setStyleSheet(subjectUnreadStyle);
}


void InboxItem::clearCurrentRead()
{
    read_label->hide();
}
