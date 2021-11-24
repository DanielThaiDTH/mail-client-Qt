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

public:
    explicit SendDialog(QWidget *parent = nullptr);
    ~SendDialog();
    void setReplyMode(const Inbox::MailData& mail);

public slots:
    void bccClicked();

private:
    Ui::SendDialog *ui;
};

#endif // SENDDIALOG_H
