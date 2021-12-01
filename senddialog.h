#ifndef SENDDIALOG_H
#define SENDDIALOG_H

#include <QDialog>
#include "inbox.h"
#include "Email.h"

namespace Ui {
class SendDialog;
}

class SendDialog : public QDialog
{
    Q_OBJECT
    bool bcc_visible;
    Email** mail;
    const Email* reply;
    QString error_style = "* { background: pink }";
    QString local;

public:
    explicit SendDialog(Email** mail, QString local_addr, QWidget *parent = nullptr);
    ~SendDialog();
    void setReplyMode(const Inbox::MailData& mail);
    void setDraftMode(const Inbox::MailData& mail);
    void makeMail();

public slots:
    void bccClicked();
    void accept() override;
    void reject() override;
    void checkAddr();

private:
    Ui::SendDialog *ui;
};

#endif // SENDDIALOG_H
