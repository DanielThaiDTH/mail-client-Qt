#include "filterdialog.h"
#include "ui_filterdialog.h"
#include <QPushButton>
#include <QPixmap>

FilterDialog::FilterDialog(SearchFilter* filter, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterDialog)
{
    ui->setupUi(this);
    this->filter = filter;

    //Initializing button styles
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
    cancel->setText(" Clear ");

    connect(cancel, &QPushButton::clicked, this, &QDialog::reject);


    //Setting text
    ui->toLine->setText(filter->getToFilter());
    ui->fromLine->setText(filter->getFromFilter());
    ui->subjectLine->setText(filter->getSubjectFilter());
    ui->tagsLine->setText(filter->getTagFilter());
    ui->textLine->setText(filter->getTextFilter());
    ui->attachCheckBox->setChecked(filter->getAttachmentCheck());
    ui->attachFilterLine->setText(filter->getAttachmentFilter());

    //Date
    openFrom = new QAction(QPixmap(":/images/cal.png"), "Open calender");
    openTo = new QAction(QPixmap(":/images/cal.png"), "Open calender");
    ui->dateFrom->addAction(openFrom, QLineEdit::TrailingPosition);
    ui->dateTo->addAction(openTo, QLineEdit::TrailingPosition);

    connect(ui->toLine, &QLineEdit::textEdited, this, &FilterDialog::toChange);
    connect(ui->fromLine, &QLineEdit::textEdited, this, &FilterDialog::fromChange);
    connect(ui->subjectLine, &QLineEdit::textEdited, this, &FilterDialog::subjectChange);
    connect(ui->tagsLine, &QLineEdit::textEdited, this, &FilterDialog::tagsChange);
    connect(ui->textLine, &QLineEdit::textEdited, this, &FilterDialog::textChange);
    connect(ui->attachCheckBox, &QCheckBox::toggled, this, &FilterDialog::attachmentCheckChange);
    connect(ui->attachFilterLine, &QLineEdit::textEdited, this, &FilterDialog::attachmentChange);
}

FilterDialog::~FilterDialog()
{
    delete ui;
}


void FilterDialog::queryChange(const QString& text)
{
    filter->setQuery(text);
}

void FilterDialog::toChange(const QString& text)
{
    filter->setToFilter(text);
}

void FilterDialog::fromChange(const QString& text)
{
    filter->setFromFilter(text);
}

void FilterDialog::subjectChange(const QString& text)
{
    filter->setSubjectFilter(text);
}

void FilterDialog::tagsChange(const QString& text)
{
    filter->setTagFilter(text);
}

void FilterDialog::textChange(const QString& text)
{
    filter->setTextFilter(text);
}

void FilterDialog::attachmentCheckChange(bool val)
{
    filter->setAttachmentCheck(val);
}

void FilterDialog::attachmentChange(const QString& text)
{
    filter->setAttachmentFilter(text);
}
