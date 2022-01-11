#include "globaltagdialog.h"
#include "ui_globaltagdialog.h"

GlobalTagDialog::GlobalTagDialog(QSet<QString>* existing_tags, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GlobalTagDialog)
{
    ui->setupUi(this);
    this->tags = existing_tags;

    tag_list.setStringList(QStringList(this->tags->begin(), this->tags->end()));
    ui->listView->setModel(&tag_list);
    ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);

    QString AddStyle = "*:hover { background-color: #00BC4B; }\n";
    AddStyle += "* { border-radius: 3; background-color: #1FF2A5; color: white; font-weight: bold; font-size: 14px; border: 1px solid #00BC4B;}";
    ui->addNew->setStyleSheet(AddStyle);

    QPushButton* ok = ui->buttonBox->button(QDialogButtonBox::Ok);
    ok->setMinimumHeight(30);
    QString OKStyle = "* { border-radius: 3; font-size: 14px; padding: 5px 20px 5px 20px;";
    OKStyle += "background-color: white; font-weight: bold; border: 1px solid #707070; } *:hover { background-color: #C0C0C0; }";
    ok->setStyleSheet(OKStyle);
    ok->setText("Back");

    connect(ui->addNew, &QPushButton::clicked, this, &GlobalTagDialog::addTags);
}

GlobalTagDialog::~GlobalTagDialog()
{
    delete ui;
}

QString GlobalTagDialog::toLower(QString s)
{
    std::string str = s.toStdString();
    std::transform(str.begin(), str.end(), str.begin(), [](char c){ return std::tolower(c); });
    return QString::fromStdString(str);
}

void GlobalTagDialog::addTags()
{
    QString tag = toLower(ui->lineEdit->text().trimmed());
    if (tag.size() == 0) return;
    tags->insert(tag);
    tags->insert(tag);
    tag_list.setStringList(QStringList(this->tags->begin(), this->tags->end()));
}
