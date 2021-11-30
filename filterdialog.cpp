#include "filterdialog.h"
#include "ui_filterdialog.h"
#include <QPushButton>

FilterDialog::FilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterDialog)
{
    ui->setupUi(this);
    QString searchButtonStyle = "*:hover { background-color: #437BEC; }\n";
    searchButtonStyle += "* { border-radius: 3; background-color: #21089F; ";
    searchButtonStyle += "color: white; font-weight: bold; font-size: 14px; border: 1px solid #707070; ";
    searchButtonStyle += "padding: 2px 5px 2px 5px; margin-right: 10px;}";

    QPushButton* ok = ui->buttonBox->button(QDialogButtonBox::Ok);
    ok->setStyleSheet(searchButtonStyle);
    ok->setMinimumHeight(30);
    ok->setText(" Set ");
    QPushButton* cancel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    QString cancelStyle = "* { border-radius: 3; font-size: 14px; background-color: white;";
    cancelStyle += "font-weight: bold; border: 1px solid #707070; margin-left: 10px; padding: 2px 5px 2px 5px; } ";
    cancelStyle += "*:hover { background-color: #C0C0C0; }";
    cancel->setStyleSheet(cancelStyle);
    cancel->setMinimumHeight(30);
    cancel->setText(" Cancel ");
}

FilterDialog::~FilterDialog()
{
    delete ui;
}
