#include "mainwindow.h"
#include "MailParser.h"
#include "errorwindow.h"
#include "inbox.h"
#include <fstream>

#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    ErrorWindow ew;
    Inbox* ib = nullptr;

    std::ofstream log("log.txt");

    MailParser parser(":/mailbox.txt", log);
    parser.parse();

    if (parser.isParsed()) {
        ib = new Inbox(parser.getMails());
        w.setInbox(ib);
        w.show();
    } else {
        ew.show();
    }

    return a.exec();
}
