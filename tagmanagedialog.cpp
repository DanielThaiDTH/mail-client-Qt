#include "tagmanagedialog.h"
#include "ui_tagmanagedialog.h"
#include <QModelIndexList>
#include <iostream>

TagManageDialog::TagManageDialog(QSet<QString>* tags, QSet<QString>* existing_tags, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TagManageDialog)
{
    ui->setupUi(this);
    this->tags = tags;
    this->existing_tags = existing_tags;

    ui->comboBox->addItems(QStringList(this->existing_tags->begin(), this->existing_tags->end()));
    tag_list.setStringList(QStringList(this->tags->begin(), this->tags->end()));
    ui->listView->setModel(&tag_list);
    ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(ui->remove, &QPushButton::clicked, this, &TagManageDialog::removeTags);
    connect(ui->addExisting, &QPushButton::clicked, this, &TagManageDialog::addTags);
    connect(ui->addNew, &QPushButton::clicked, this, &TagManageDialog::addTags);
}

TagManageDialog::~TagManageDialog()
{
    delete ui;
}


QString TagManageDialog::toLower(QString s)
{
    std::string str = s.toStdString();
    std::transform(str.begin(), str.end(), str.begin(), [](char c){ return std::tolower(c); });
    return QString::fromStdString(str);
}


void TagManageDialog::removeTags()
{
    auto idxs = ui->listView->selectionModel()->selectedIndexes();

    for (auto index : idxs) {
        if (!index.isValid() || index.row() >= tag_list.stringList().size())
            continue;
        tags->remove(tag_list.data(index).toString());
        tag_list.removeRows(index.row(), 1);
    }
}


void TagManageDialog::addTags()
{
    QString name = dynamic_cast<QPushButton*>(sender())->objectName();

    if (name == "addExisting") {
        QString tag = ui->comboBox->currentText();
        if (tag.size() == 0) return;
        tags->insert(tag);
        tag_list.setStringList(QStringList(this->tags->begin(), this->tags->end()));
    } else if (name == "addNew") {
        QString tag = toLower(ui->lineEdit->text().trimmed());
        if (tag.size() == 0) return;
        tags->insert(tag);
        existing_tags->insert(tag);
        ui->comboBox->clear();
        ui->comboBox->addItems(QStringList(this->existing_tags->begin(), this->existing_tags->end()));
        tag_list.setStringList(QStringList(this->tags->begin(), this->tags->end()));
    } else {
        std::cout << "Unknown button selected.";
    }
}
