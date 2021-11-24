#include "inboxitem.h"

InboxItem::InboxItem(int id, bool* read, QString subject, QString sender, QWidget* parent) : QFrame(parent)
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

    if (*isRead)
        subject_label->setStyleSheet(subjectReadStyle);
    else
        subject_label->setStyleSheet(subjectUnreadStyle);

    sender_label->setStyleSheet(senderStyle);

    layout = new QHBoxLayout(this);
    layout->addWidget(select_check);
    layout->addWidget(subject_label);
    layout->addWidget(sender_label);
    this->setLayout(layout);

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
        *isRead = true;
        subject_label->setStyleSheet(subjectReadStyle);
        emit mailClicked(mail_id);
    }
}


int InboxItem::getID() const
{
    return mail_id;
}
