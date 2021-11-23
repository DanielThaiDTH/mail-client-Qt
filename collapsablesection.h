#ifndef COLLAPSABLE_SECTION_H
#define COLLAPSABLE_SECTION_H

#include <QFrame>
#include <QGridLayout>
#include <QLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QWidget>
#include <QLabel>

class CollapsableSection : public QWidget
{
    Q_OBJECT
private:
    QVBoxLayout* mainLayout;
    QPushButton* toggleButton;
    QFrame* content;
    QScrollArea* contentArea;

public slots:
    void toggle(bool collapsed);

public:
    explicit CollapsableSection(const QString & title = "", QWidget* parent = 0);
    void setContentLayout(QLayout* contentLayout);
};

#endif // SECTION_H

