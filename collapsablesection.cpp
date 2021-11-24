#include "collapsablesection.h"
#include <iostream>

CollapsableSection::CollapsableSection(const QString & title, QWidget* parent)
    : QWidget(parent)
{
    toggleButton = new QPushButton(title);
    content = new QFrame();
    contentArea = new QScrollArea();
    mainLayout = new QVBoxLayout();

    mainLayout->setAlignment(Qt::AlignLeft);
    content->setFrameShape(QFrame::Box);

    toggleButton->setStyleSheet("* { border: none; font-weight: bold; text-align: left; color: blue; }");
    toggleButton->setCheckable(true);
    toggleButton->setChecked(false);

    contentArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    content->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    content->setMinimumHeight(0);
    content->setStyleSheet("* { background-color: white; border-style: none; }");

    // start out collapsed
    contentArea->setMaximumHeight(0);
    contentArea->setMinimumHeight(0);
    contentArea->setMinimumWidth(250);
    contentArea->setWidget(content);
    this->setMaximumHeight(38);

    mainLayout->addWidget(toggleButton);
    mainLayout->addWidget(contentArea);
    setLayout(mainLayout);

    connect(toggleButton, &QPushButton::toggled, this, &CollapsableSection::toggle);
}


void CollapsableSection::toggle(bool collapsed) {
    if (collapsed) {
        this->setMaximumHeight(250);
        this->setMinimumHeight(150);
        contentArea->setMaximumHeight(250);
        contentArea->setMinimumHeight(250);
        content->setMinimumHeight(200);
        content->setMinimumWidth(300);
        contentArea->show();
    } else {
        this->setMaximumHeight(38);
        this->setMinimumHeight(38);
        contentArea->hide();
        //content->setMinimumHeight(0);
        //content->setMaximumHeight(0);
        //contentArea->setMinimumHeight(0);
        //contentArea->setMaximumHeight(0);
    }
}


void CollapsableSection::setContentLayout(QLayout* contentLayout)
{
    if (content->layout() != nullptr)
        delete content->layout();
    content->setLayout(contentLayout);

    //QLayout* layout = content->layout();
    //std::cout << dynamic_cast<QLabel*>(layout->itemAt(0)->widget())->text().toStdString() << std::endl;
}

