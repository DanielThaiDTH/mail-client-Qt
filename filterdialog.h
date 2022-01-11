#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include <QDate>
#include <QAction>
#include "searchfilter.h"

namespace Ui {
class FilterDialog;
}

class FilterDialog : public QDialog
{
    Q_OBJECT
    SearchFilter* filter;
    QAction* openFrom;
    QAction* openTo;

public:
    explicit FilterDialog(SearchFilter* filter, QWidget *parent = nullptr);
    ~FilterDialog();

public slots:
    void queryChange(const QString& text);
    void toChange(const QString& text);
    void fromChange(const QString& text);
    void subjectChange(const QString& text);
    void tagsChange(const QString& text);
    void textChange(const QString& text);
    void attachmentCheckChange(bool val);
    void attachmentChange(const QString& text);

private:
    Ui::FilterDialog *ui;
};

#endif // FILTERDIALOG_H
