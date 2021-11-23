#ifndef ERRORWINDOW_H
#define ERRORWINDOW_H

#include <QObject>
#include <QWidget>
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class ErrorWindow; }
QT_END_NAMESPACE

class ErrorWindow final : public QDialog
{
    Q_OBJECT
public:
    ErrorWindow(QWidget* parent = nullptr);
    ~ErrorWindow();
    void closeEvent(QCloseEvent* e) override;
private:
    Ui::ErrorWindow *ui;
};

#endif // ERRORWINDOW_H
