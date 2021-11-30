#ifndef TAGMANAGEDIALOG_H
#define TAGMANAGEDIALOG_H

#include <QDialog>
#include <QSet>
#include <QString>
#include <QStringList>
#include <QStringListModel>
#include <string>

namespace Ui {
class TagManageDialog;
}

class TagManageDialog : public QDialog
{
    Q_OBJECT

    QSet<QString>* tags;
    QSet<QString>* existing_tags;
    QStringListModel tag_list;

    QString toLower(QString s);

public:
    explicit TagManageDialog(QSet<QString>* tags, QSet<QString>* existing_tags, QWidget *parent = nullptr);
    ~TagManageDialog();

public slots:
    void removeTags();
    void addTags();

private:
    Ui::TagManageDialog *ui;
};

#endif // TAGMANAGEDIALOG_H
