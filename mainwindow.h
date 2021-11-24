#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFrame>
#include <QVBoxLayout>
#include <QSplitter>
#include <QScrollArea>
#include "inbox.h"
#include "inboxdisplay.h"
#include "mailframe.h"
#include "senddialog.h"
#include "forwarddialog.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Inbox* inbox;
    QFrame* inbox_frame;
    QVBoxLayout* inbox_layout;
    InboxDisplay* inbox_disp;
    QSplitter* splitter;
    QScrollArea* scrollArea;
    MailFrame* mail_frame;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setInbox(Inbox* ib);

public slots:
    /**
     * @brief writeClicked, creates a send mail dialog.
     */
    void writeClicked();

    /**
     * @brief itemClicked, triggers newMailContent, sending the mail data with the matching id.
     * @param id
     */
    void itemClicked(int id);

    /**
     * @brief trashMail, moves the mail identified by the given id to the trash.
     * @param id
     */
    void trashMail(int id);

    void openReplyDialog(int id);
    void openForwardDialog(int id);

signals:
    void newMailContent(const Inbox::MailData& data);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
