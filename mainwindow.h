#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFrame>
#include <QVBoxLayout>
#include <QSplitter>
#include <QScrollArea>
#include <QLineEdit>
#include <QMessageBox>
#include <QAction>
#include "inbox.h"
#include "inboxdisplay.h"
#include "mailframe.h"
#include "senddialog.h"
#include "forwarddialog.h"
#include "filterdialog.h"
#include "searchfilter.h"


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
    QAction* clear_search;
    QAction* make_search;
    SearchFilter filter;

    QString inactiveBoxStyle = "* { margin: 10px; color: #707070; } *:hover { text-decoration: underline; }";
    QString activeBoxStyle = "QPushButton { margin: 10px; color: #CF4F1F; }";

    void changeBox(BoxType type);
    void resetButtonStyles();

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

    /**
     * @brief searchEntered, calls the Inbox search function and sets the
     * inbox display to the search results.
     */
    void searchEntered();

    /**
     * @brief removeSearch, clears the search field and sets the inbox
     * display to the active inbox.
     */
    void removeSearch();

    /**
     * @brief searchFilterOpen, opens the search filter dialog.
     */
    void searchFilterOpen();
    void openReplyDialog(int id);
    void openForwardDialog(int id);
    void nextMail(int id);
    void prevMail(int id);
    void inboxSelected();
    void sentSelected();
    void draftSelected();
    void trashSelected();
    void junkSelected();

signals:
    void newMailContent(const Inbox::MailData& data);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
