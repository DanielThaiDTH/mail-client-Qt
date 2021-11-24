#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Mail Application");

    QString writeStyle = "QPushButton { background-color: #E1F1F1; font-size: 18px; height: 45; width: 120; border-radius: 15;";
    writeStyle += " margin-right: 90px; color: #707070; border: solid; border-width: 1px; border-color: #707070; }\n";
    writeStyle += "*:hover { background-color: #B4EBEB; }";
    ui->writeMailButton->setStyleSheet(writeStyle);

    QString filterStyle = "* { background-color: #BED5F1; font-size: 14px; color:#707070; border-radius: 5; }";
    ui->filterButton->setStyleSheet(filterStyle);
    ui->filterButton->setMinimumHeight(30);
    ui->filterButton->setMinimumWidth(80);

    QString inboxStyle = "* { background-color: white; }";

    inbox_frame = new QFrame(this);
    inbox_frame->setStyleSheet(inboxStyle);
    inbox_layout = new QVBoxLayout(this);
    inbox_frame->setFrameStyle(QFrame::Plain);
    inbox_frame->setLayout(inbox_layout);
    inbox_frame->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    inbox_disp = new InboxDisplay();
    inbox_disp->setMinimumSize(700, 750);
    inbox_disp->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    inbox_layout->addWidget(inbox_disp);

    mail_frame = new MailFrame();
    mail_frame->setObjectName("mail-frame");
    mail_frame->setFrameShape(QFrame::Panel);
    mail_frame->setStyleSheet("QFrame#mail-frame { background-color: #A1C6DF; }");
    mail_frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidget(inbox_frame);
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    splitter = new QSplitter(this);
    splitter->addWidget(scrollArea);
    splitter->addWidget(mail_frame);
    splitter->setHandleWidth(1);
    splitter->setStyleSheet("QSplitter::handle { background-color: white; border: 1px solid #707070;}");
    ui->layoutInbox->addWidget(splitter);


    //ui->scrollArea->setWidget(inbox_frame);

    ui->inbox_label->setStyleSheet("* { margin: 10px; color: #707070; }");
    ui->draft_label->setStyleSheet("* { margin: 10px; color: #707070; }");
    ui->sent_label->setStyleSheet("* { margin: 10px; color: #707070; }");
    ui->tags_label->setStyleSheet("* { margin: 10px; color: #707070; }");
    ui->junk_label->setStyleSheet("* { margin: 10px; color: #707070; }");
    ui->trash_label->setStyleSheet("* { margin: 10px; color: #707070; }");

    connect(ui->writeMailButton, &QAbstractButton::clicked, this, &MainWindow::writeClicked);
    connect(inbox_disp, &InboxDisplay::mailSelected, this, &MainWindow::itemClicked);
    connect(this, &MainWindow::newMailContent, mail_frame, &MailFrame::updateContent);
    connect(mail_frame, &MailFrame::mailTrashed, this, &MainWindow::trashMail);
    connect(mail_frame, &MailFrame::replyTo, this, &MainWindow::openReplyDialog);
    connect(mail_frame, &MailFrame::forwardMail, this, &MainWindow::openForwardDialog);
}


MainWindow::~MainWindow()
{
    if (inbox != nullptr)
        delete inbox;

    delete ui;
}


void MainWindow::setInbox(Inbox *ib)
{
    inbox = ib;
    inbox_disp->setInbox(inbox->getInboxSummary());
}


void MainWindow::writeClicked()
{
    SendDialog* send_window = new SendDialog(this);

    send_window->exec();
}


void MainWindow::itemClicked(int id)
{
    const Inbox::MailData& data = inbox->getMailData(id);
    emit newMailContent(data);
}


void MainWindow::trashMail(int id)
{
    inbox->moveToTrash(id);
    inbox_disp->setInbox(inbox->getInboxSummary());
}


void MainWindow::openReplyDialog(int id)
{
    if (id < 0)
        return;

    SendDialog* send_window = new SendDialog(this);
    send_window->setReplyMode(inbox->getMailData(id));

    send_window->exec();
}

void MainWindow::openForwardDialog(int id)
{
    if (id < 0)
        return;

    ForwardDialog* for_window = new ForwardDialog(inbox->getMailData(id), this);

    for_window->exec();
}
