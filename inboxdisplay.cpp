#include "inboxdisplay.h"

InboxDisplay::InboxDisplay(QWidget* parent) : QWidget(parent)
{
    mails = new QVector<InboxItem*>();
    layout = new QVBoxLayout();
    layout->setSpacing(0);
    this->setLayout(layout);
}

InboxDisplay::~InboxDisplay(){
    for (InboxItem*& item : *mails) {
        if (item != nullptr)
            delete item;
    }
}


void InboxDisplay::setInbox(const QVector<MailSummary>& summary)
{
    delete mails;
    mails = new QVector<InboxItem*>();

    for (const MailSummary& sum : summary) {
        InboxItem* temp = new InboxItem(sum.id, sum.read, sum.subject, sum.sender);
        mails->push_back(temp);
        layout->addWidget(temp);
        connect(temp, &InboxItem::mailClicked, this, &InboxDisplay::itemClicked);
    }
}


void InboxDisplay::itemClicked(int id)
{
    emit mailSelected(id);
}

