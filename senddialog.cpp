#include "senddialog.h"
#include "ui_senddialog.h"

SendDialog::SendDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SendDialog)
{
    ui->setupUi(this);

    bcc_visible = false;
    this->setWindowTitle("Send Email");

    ui->bcc_label->setVisible(bcc_visible);
    ui->bcc_edit->setVisible(bcc_visible);

    ui->buttonBox->setStyleSheet("QDialogButtonBox { button-layout: 2; }\n");

    QPushButton* ok = ui->buttonBox->button(QDialogButtonBox::Ok);
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
}


void SendDialog::bccClicked()
{
    bcc_visible = !bcc_visible;
    ui->bcc_label->setVisible(bcc_visible);
    ui->bcc_edit->setVisible(bcc_visible);
}
