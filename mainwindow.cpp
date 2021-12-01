#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

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

    QString filterStyle = "* { background-color: #BED5F1; font-size: 14px; color:#707070; border-radius: 5; }\n";
    filterStyle += "*:hover { background-color: #A1C6DF; }";
    ui->filterButton->setStyleSheet(filterStyle);
    ui->filterButton->setMinimumHeight(30);
    ui->filterButton->setMinimumWidth(80);


    /*Set up the inbox frame*/
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


    /*Set up the mail frame*/
    mail_frame = new MailFrame();
    mail_frame->setObjectName("mail-frame");
    mail_frame->setFrameShape(QFrame::Panel);
    mail_frame->setStyleSheet("QFrame#mail-frame { background-color: #A1C6DF; }");
    mail_frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidget(inbox_frame);
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);


    /*Set up the splitter*/
    splitter = new QSplitter(this);
    splitter->addWidget(scrollArea);
    splitter->addWidget(mail_frame);
    splitter->setHandleWidth(1);
    splitter->setStyleSheet("QSplitter::handle { background-color: white; border: 1px solid #707070;}");
    splitter->setMinimumHeight(650);
    splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ui->layoutInbox->addWidget(splitter);


    //Setup mail box selection styles
    ui->inbox_button->setStyleSheet(inactiveBoxStyle);
    ui->draft_button->setStyleSheet(inactiveBoxStyle);
    ui->sent_button->setStyleSheet(inactiveBoxStyle);
    ui->tags_button->setStyleSheet(inactiveBoxStyle);
    ui->junk_button->setStyleSheet(inactiveBoxStyle);
    ui->trash_button->setStyleSheet(inactiveBoxStyle);

    //Line edit action icons
    clear_search = new QAction(QPixmap(":/images/clear.png"), "Clear");
    make_search = new QAction(QPixmap(":/images/search.png"), "Search");
    clear_search->setWhatsThis("Clear search query and results");
    ui->lineEdit->addAction(clear_search, QLineEdit::TrailingPosition);
    ui->lineEdit->addAction(make_search, QLineEdit::LeadingPosition);

    //misc
    ui->settings->setMaximumHeight(30);
    ui->settings->setMinimumHeight(30);
    ui->settings->setMaximumWidth(30);
    ui->settings->setMinimumWidth(30);

    connect(ui->writeMailButton, &QAbstractButton::clicked, this, &MainWindow::writeClicked);
    connect(inbox_disp, &InboxDisplay::mailSelected, this, &MainWindow::itemClicked);
    connect(this, &MainWindow::newMailContent, mail_frame, &MailFrame::updateContent);

    connect(mail_frame, &MailFrame::mailTrashed, this, &MainWindow::trashMail);
    connect(mail_frame, &MailFrame::replyTo, this, &MainWindow::openReplyDialog);
    connect(mail_frame, &MailFrame::forwardMail, this, &MainWindow::openForwardDialog);
    connect(mail_frame, &MailFrame::nextMail, this, &MainWindow::nextMail);
    connect(mail_frame, &MailFrame::prevMail, this, &MainWindow::prevMail);
    connect(mail_frame, &MailFrame::tagClicked, this, &MainWindow::openTagDialog);

    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &MainWindow::searchEntered);
    connect(clear_search, &QAction::triggered, this, &MainWindow::removeSearch);
    connect(make_search, &QAction::triggered, this, &MainWindow::searchEntered);
    connect(ui->filterButton, &QAbstractButton::clicked, this, &MainWindow::searchFilterOpen);

    connect(ui->inbox_button, &QAbstractButton::clicked, this, &MainWindow::inboxSelected);
    connect(ui->sent_button, &QAbstractButton::clicked, this, &MainWindow::sentSelected);
    connect(ui->draft_button, &QAbstractButton::clicked, this, &MainWindow::draftSelected);
    connect(ui->trash_button, &QAbstractButton::clicked, this, &MainWindow::trashSelected);
    connect(ui->junk_button, &QAbstractButton::clicked, this, &MainWindow::junkSelected);
    connect(ui->tags_button, &QAbstractButton::clicked, this, &MainWindow::tagSelected);

    connect(ui->actionExit, &QAction::triggered, qApp, &QApplication::exit);
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
    BoxType type = ib->getActiveBox();

    switch (type) {
    case BoxType::INBOX:
        ui->inbox_button->setStyleSheet(activeBoxStyle);
        break;
    case BoxType::TRASH:
        ui->trash_button->setStyleSheet(activeBoxStyle);
        break;
    case BoxType::JUNK:
        ui->junk_button->setStyleSheet(activeBoxStyle);
        break;
    case BoxType::SENT:
        ui->sent_button->setStyleSheet(activeBoxStyle);
        break;
    case BoxType::DRAFT:
        ui->draft_button->setStyleSheet(activeBoxStyle);
        break;
    }
}


void MainWindow::resetButtonStyles()
{
    ui->inbox_button->setStyleSheet(inactiveBoxStyle);
    ui->draft_button->setStyleSheet(inactiveBoxStyle);
    ui->sent_button->setStyleSheet(inactiveBoxStyle);
    ui->tags_button->setStyleSheet(inactiveBoxStyle);
    ui->junk_button->setStyleSheet(inactiveBoxStyle);
    ui->trash_button->setStyleSheet(inactiveBoxStyle);
}

void MainWindow::changeBox(BoxType type)
{
    inbox->setActiveBox(type);
    inbox_disp->setInbox(inbox->getInboxSummary());
    mail_frame->clearContent();
    mail_frame->hide();
}


void MainWindow::writeClicked()
{
    Email* m = nullptr;
    SendDialog* send_window = new SendDialog(&m, inbox->getLocalAddr(), this);

    int state = send_window->exec();

    if (state == QDialog::Accepted) {
        m->setFromAddress(inbox->getLocalAddr().toStdString());
        inbox->addEmail(m, BoxType::SENT);
    } else {
        m->setFromAddress(inbox->getLocalAddr().toStdString());
        inbox->addEmail(m, BoxType::DRAFT);
    }

    send_window->deleteLater();
}


void MainWindow::itemClicked(int id)
{
    const Inbox::MailData& data = inbox->getMailData(id);
    if (inbox->getActiveBox() != BoxType::DRAFT && data.id != -1) {
        mail_frame->show();
        emit newMailContent(data);
    } else if (data.id != -1) {
        Email* m = nullptr;
        SendDialog* send_window = new SendDialog(&m, inbox->getLocalAddr(), this);
        send_window->setDraftMode(inbox->getMailData(id));

        int state = send_window->exec();

        if (state == QDialog::Accepted) {
            inbox->addEmail(m, BoxType::SENT);
            inbox->removeMail(id);
            inbox_disp->setInbox(inbox->getInboxSummary());
        }

        send_window->deleteLater();
    }
}


void MainWindow::trashMail(int id)
{
    inbox->moveToTrash(id);
    inbox_disp->setInbox(inbox->getInboxSummary());
    mail_frame->hide();
}


void MainWindow::openReplyDialog(int id)
{
    if (id < 0)
        return;

    Email* m = nullptr;
    SendDialog* send_window = new SendDialog(&m, inbox->getLocalAddr(), this);
    send_window->setReplyMode(inbox->getMailData(id));

    int state = send_window->exec();

    if (state == QDialog::Accepted) {
        inbox->addEmail(m, BoxType::SENT);
    }

    send_window->deleteLater();
}


void MainWindow::openForwardDialog(int id)
{
    if (id < 0)
        return;

    ForwardDialog* for_window = new ForwardDialog(inbox->getMailData(id), this);

    for_window->exec();
    for_window->deleteLater();
}


void MainWindow::openTagDialog(int id)
{
    if (id < 0)
        return;

    TagManageDialog* td = new TagManageDialog(&inbox->getMailTags(id), &inbox->getTags(), this);
    td->exec();
    td->deleteLater();
}


void MainWindow::nextMail(int id)
{
    const Inbox::MailData& data = inbox->getMailDataOffset(id, 1);
    if (data.id != -1) {
        mail_frame->show();
        emit newMailContent(data);
        inbox_disp->updateRead(data.id);
    }
}


void MainWindow::prevMail(int id)
{
    const Inbox::MailData& data = inbox->getMailDataOffset(id, -1);
    if (data.id != -1) {
        mail_frame->show();
        emit newMailContent(data);
        inbox_disp->updateRead(data.id);
    }
}


void MainWindow::searchEntered()
{
    if (!ui->lineEdit)
        return;

    filter.setQuery(ui->lineEdit->text().trimmed());
    inbox_disp->setInbox(inbox->search(filter));
}


void MainWindow::removeSearch()
{
    ui->lineEdit->setText("");
    inbox_disp->setInbox(inbox->getInboxSummary());
}


void MainWindow::searchFilterOpen()
{
    FilterDialog* dialog = new FilterDialog(&filter, this);
    int state = dialog->exec();
    if (state == QDialog::Rejected) {
        filter.reset();
        filter.setQuery(ui->lineEdit->text().trimmed());
        inbox_disp->setInbox(inbox->search(filter));
    }
}


void MainWindow::inboxSelected()
{
    resetButtonStyles();
    changeBox(BoxType::INBOX);
    ui->inbox_button->setStyleSheet(activeBoxStyle);
}

void MainWindow::sentSelected()
{
    resetButtonStyles();
    changeBox(BoxType::SENT);
    ui->sent_button->setStyleSheet(activeBoxStyle);
}

void MainWindow::draftSelected()
{
    resetButtonStyles();
    changeBox(BoxType::DRAFT);
    ui->draft_button->setStyleSheet(activeBoxStyle);
}

void MainWindow::trashSelected()
{
    resetButtonStyles();
    changeBox(BoxType::TRASH);
    ui->trash_button->setStyleSheet(activeBoxStyle);
}

void MainWindow::junkSelected()
{
    resetButtonStyles();
    changeBox(BoxType::JUNK);
    ui->junk_button->setStyleSheet(activeBoxStyle);
}

void MainWindow::tagSelected()
{
    ui->tags_button->setStyleSheet(activeBoxStyle);
    GlobalTagDialog* gd = new GlobalTagDialog(&inbox->getTags());
    gd->exec();
    ui->tags_button->setStyleSheet(inactiveBoxStyle);
    gd->deleteLater();
}
