#include "forwarddialog.h"
#include "ui_forwarddialog.h"

ForwardDialog::ForwardDialog(const Inbox::MailData& data, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ForwardDialog)
{
    ui->setupUi(this);

    QPushButton* ok = ui->buttonBox->button(QDialogButtonBox::Ok);
    ok->setMinimumHeight(30);
    QString OKStyle = "*:hover { background-color: #00BC4B; }\n";
    OKStyle += "* { border-radius: 3; background-color: #1FF2A5; color: white; font-weight: bold; font-size: 14px; border: 1px solid #00BC4B; padding: 3px 20px 3px 20px;}";
    ok->setStyleSheet(OKStyle);
    QPushButton* cancel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    cancel->setMinimumHeight(30);
    cancel->setStyleSheet("* { border-radius: 3; font-size: 14px; background-color: #B0B0B0; border: 1px solid #707070; padding: 3px; } *:hover { background-color: gray; }");

    ui->sender_label->setText(QString::fromStdString(data.mail->getFromAddress()));
    ui->subject_label->setText(QString::fromStdString(data.mail->getSubject()));
;}

ForwardDialog::~ForwardDialog()
{
    delete ui;
}
