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
