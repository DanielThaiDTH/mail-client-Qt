#ifndef FORWARDDIALOG_H
#define FORWARDDIALOG_H

#include <QDialog>
#include <QPushButton>
#include "inbox.h"
#include "Email.h"

namespace Ui {
class ForwardDialog;
}

class ForwardDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ForwardDialog(const Inbox::MailData& data, QWidget *parent = nullptr);
    ~ForwardDialog();

private:
    Ui::ForwardDialog *ui;
};

#endif // FORWARDDIALOG_H
