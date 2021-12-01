#include "senddialog.h"
#include "ui_senddialog.h"
#include <iostream>
#include <QValidator>
#include <QRegularExpression>

SendDialog::SendDialog(Email** mail, QString local_addr, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SendDialog)
{
    ui->setupUi(this);
    this->mail = mail;
    reply = nullptr;
    local = local_addr;

    bcc_visible = false;
    this->setWindowTitle("Send Email");

    ui->bcc_label->setVisible(bcc_visible);
    ui->bcc_edit->setVisible(bcc_visible);

    ui->buttonBox->setStyleSheet("QDialogButtonBox { button-layout: 2; }\n");

    QPushButton* ok = ui->buttonBox->button(QDialogButtonBox::Ok);
    ok->setEnabled(false);
    ok->setMinimumHeight(30);
    QString OKStyle = "*:hover { background-color: #00BC4B; }\n";
    OKStyle += "* { border-radius: 3; background-color: #1FF2A5; color: white; font-weight: bold; font-size: 14px; border: 1px solid #00BC4B;}";
    ok->setStyleSheet(OKStyle);
    ok->setText("Send");
    QPushButton* cancel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    cancel->setMinimumHeight(30);
    cancel->setStyleSheet("* { border-radius: 3; font-size: 14px; background-color: white; font-weight: bold; border: 1px solid #707070; } *:hover { background-color: #C0C0C0; }");

    ui->bcc_button->setStyleSheet("* { border-radius: 3; background-color: #BED5F1; border: 1px solid #A6C3E6; }\n *:hover { background-color: #A6C3E6; }");
    connect(ui->bcc_button, &QAbstractButton::clicked, this, &SendDialog::bccClicked);

    QRegularExpression re("\\S+@\\S+");
    auto v = new QRegularExpressionValidator(re, this);
    ui->to_edit->setValidator(v);
    ui->cc_edit->setValidator(v);
    ui->bcc_edit->setValidator(v);

    connect(ui->to_edit, &QLineEdit::textChanged, this, &SendDialog::checkAddr);
    connect(ui->cc_edit, &QLineEdit::textChanged, this, &SendDialog::checkAddr);
    connect(ui->bcc_edit, &QLineEdit::textChanged, this, &SendDialog::checkAddr);
}


SendDialog::~SendDialog()
{
    delete ui;
}


void SendDialog::setReplyMode(const Inbox::MailData& data)
{
    ui->to_edit->setText(QString::fromStdString(data.mail->getFromAddress()));
    ui->to_edit->setReadOnly(true);
    ui->to_edit->setStyleSheet("* { background-color: #D0D0D0; font-style: italic; }");
    ui->subject_edit->setText("Re: " + QString::fromStdString(data.mail->getSubject()));
    this->setWindowTitle("Reply to Email");
    this->reply = data.mail;
}


void SendDialog::setDraftMode(const Inbox::MailData& data)
{
    ui->to_edit->setText(QString::fromStdString(data.mail->getToAddress()));
    ui->subject_edit->setText(QString::fromStdString(data.mail->getSubject()));
    ui->plainTextEdit->setPlainText(QString::fromStdString(data.mail->getContent()));
}


void SendDialog::bccClicked()
{
    bcc_visible = !bcc_visible;
    ui->bcc_label->setVisible(bcc_visible);
    ui->bcc_edit->setVisible(bcc_visible);
}


void SendDialog::accept()
{
    makeMail();
    std::cout << "Made sent " << (*mail)->getSubject();
    QDialog::accept();
}


void SendDialog::reject()
{
    makeMail();
    std::cout << "Made draft \n";
    QDialog::reject();
}


void SendDialog::makeMail()
{
    *mail = new Email();
    (*mail)->setToAddress(ui->to_edit->text().toStdString());
    (*mail)->setFromAddress(local.toStdString());
    (*mail)->setSubject(ui->subject_edit->text().toStdString());
    (*mail)->setContent(ui->plainTextEdit->toPlainText().toStdString());
    (*mail)->setSendDate("02/07/2011 16:47 UTC+3");
    (*mail)->setReceiveDate("02/07/2011 16:47 UTC+3");

    if (reply) {
        //Strip the previous email of the replies
        Email plainMail;
        plainMail.setToAddress(reply->getToAddress());
        plainMail.setFromAddress(reply->getFromAddress());
        plainMail.setContent(reply->getContent());
        plainMail.setReceiveDate(reply->getReceiveDate());
        plainMail.setSendDate(reply->getSendDate());

        (*mail)->addResponseMail(plainMail);
        (*mail)->setFromAddress(reply->getToAddress());
        for (const Email& m : reply->getResponseMails()) {
            (*mail)->addResponseMail(const_cast<Email&>(m));
        }
    }
}


void SendDialog::checkAddr()
{
    bool ok = true;
    if (ui->to_edit->hasAcceptableInput()) {
        ui->to_edit->setStyleSheet("");
        ok = true;
    } else {
        ui->to_edit->setStyleSheet(error_style);
        ok = false;
    }

    if (ui->cc_edit->hasAcceptableInput() || ui->cc_edit->text().size() == 0) {
        ui->cc_edit->setStyleSheet("");
        ok = ok && true;
    } else {
        ui->cc_edit->setStyleSheet(error_style);
        ok = false;
    }

    if (ui->bcc_edit->hasAcceptableInput() || ui->bcc_edit->text().size() == 0) {
        ui->bcc_edit->setStyleSheet("");
        ok = ok && true;
    } else {
        ui->bcc_edit->setStyleSheet(error_style);
        ok = false;
    }

    if (ok) {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    } else {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
}

