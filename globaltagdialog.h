#ifndef GLOBALTAGDIALOG_H
#define GLOBALTAGDIALOG_H

#include <QDialog>
#include <QSet>
#include <QString>
#include <QStringList>
#include <QStringListModel>
#include <string>

namespace Ui {
class GlobalTagDialog;
}

class GlobalTagDialog : public QDialog
{
    Q_OBJECT
    QString toLower(QString s);
    QSet<QString>* tags;
    QStringListModel tag_list;

public:
    explicit GlobalTagDialog(QSet<QString>* existing_tags, QWidget *parent = nullptr);
    ~GlobalTagDialog();

public slots:
     void addTags();

private:
    Ui::GlobalTagDialog *ui;
};

#endif // GLOBALTAGDIALOG_H
