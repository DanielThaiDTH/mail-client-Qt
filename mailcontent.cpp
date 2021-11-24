#include "mailcontent.h"
#include <iostream>

MailContent::MailContent(QWidget* parent) : QFrame(parent)
{
    id = -1;
    subject = new QLabel("Subject");
    date = new QLabel("Date");
    from = new QLabel("From: ");
    to = new QLabel("To: ");
    attachment_frame = new QFrame();
    responses = nullptr;
    text = new QLabel("");

    subject->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    date->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    from->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    to->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    text->setAlignment(Qt::AlignTop);
    text->setWordWrap(true);

    subject->setStyleSheet("* { font-size: 20px; font-weight: bold; }");
    date->setStyleSheet("* { color: #A5A5A5; font-weight: bold; }");

    line1 = new QFrame();
    line2 = new QFrame();
    line1->setFrameShape(QFrame::Box);
    line2->setFrameShape(QFrame::Box);
    line1->setMinimumHeight(1);
    line2->setMinimumHeight(1);
    line1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    line2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    QString linestyle = "* { border-color: #B0B0B0; border-width: 1px; background-color: #B0B0B0 }";
    line1->setStyleSheet(linestyle);
    line2->setStyleSheet(linestyle);

    attachment_layout = new QGridLayout();
    attachment_frame->setLayout(attachment_layout);
    attachment_frame->setStyleSheet("QFrame { border-style: none; background-color: white; }");
    attachment_frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    layout = new QVBoxLayout();

    layout->addWidget(subject);
    layout->addWidget(date);
    layout->addWidget(line1);
    layout->addWidget(from);
    layout->addWidget(to);
    layout->addWidget(line2);
    layout->addWidget(attachment_frame);
    layout->addWidget(text);
    setLayout(layout);
}

MailContent::~MailContent(){}


void MailContent::setContent(const Inbox::MailData& data)
{
    if (data.mail == nullptr)
        return;

    id = data.id;
    const Email* mail = data.mail;
    subject->setText(QString::fromStdString(mail->getSubject()));
    date->setText(QString::fromStdString(mail->getReceiveDate()));
    to->setText("To: " + QString::fromStdString(mail->getToAddress()));
    from->setText("From: " + QString::fromStdString(mail->getFromAddress()));
    text->setText(QString::fromStdString(mail->getContent()));

    //Setup attachments
    int attached_cnt = 0;
    for (auto i = 0u; i < attachments.size(); i++) {
        attachment_layout->removeWidget(attachments[i]);
        delete attachments[i];
    }
    attachments.clear();

    if (data.mail->getAttachments().size() > 0){
        const std::vector<std::string>& attach_vec = data.mail->getAttachments();
        static QString attach_style = QString("* { border: 1px solid #707070; border-radius: 5px; background-color: #DFF7F5}")
                + QString("*:hover { font-weight: bold; background-color: #9BDBCA }");
        for (auto i = 0u; i < attach_vec.size(); i++) {
            QPushButton* attachment_chip = new QPushButton(QString::fromStdString(attach_vec[i]));
            attachment_chip->setStyleSheet(attach_style);
            attachments.push_back(attachment_chip);
            attachment_layout->addWidget(attachment_chip, i/3, i%3);
        }
        attachment_frame->show();
        attached_cnt = 1;
    } else {
        attachment_frame->hide();
    }

    //Setup responses
    layout->removeWidget(responses);
    if (responses != nullptr)
        delete responses;
    responses = nullptr;

    if (data.mail->getResponseMails().size() > 0) {
        responses = new CollapsableSection("Replies >", this);

        layout->insertWidget(6 + attached_cnt, responses);
        QLayout* resp_layout = new QVBoxLayout();
        resp_layout->setSizeConstraint(QLayout::SetMinAndMaxSize);
        const std::vector<Email>& resp_mails = data.mail->getResponseMails();
        for (auto i = 0u; i < resp_mails.size(); i++) {
            std::string text = "To: "  + resp_mails[i].getToAddress() + "\n"
                    + "From: " + resp_mails[i].getFromAddress() + "\n"
                    + resp_mails[i].getReceiveDate() + "\n\n"
                    + resp_mails[i].getContent() + "\n\n";
            QLabel* responseText = new QLabel(QString::fromStdString(text));
            responseText->setWordWrap(true);
            resp_layout->addWidget(responseText);
            resp_layout->setSizeConstraint(QLayout::SetMinimumSize);
            resp_layout->setContentsMargins(0,15,0,15);
            //std::cout << responseText->text().toStdString() << " out " << std::endl;
        }

        responses->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        responses->setStyleSheet(" * { background-color: white; border-style: none; color: black; margin: 0px; }");

        responses->setContentLayout(resp_layout);
    }
}


void MailContent::clearContents()
{
    id = -1;
    subject->setText("Subject");
    date->setText("Date");
    from->setText("From: ");
    to->setText("To: ");

    for (auto i = 0u; i < attachments.size(); i++) {
        attachment_layout->removeWidget(attachments[i]);
        delete attachments[i];
    }
    attachments.clear();
    attachment_frame->hide();

    layout->removeWidget(responses);
    if (responses != nullptr)
        delete responses;
    responses = nullptr;

    text->setText("");
}
