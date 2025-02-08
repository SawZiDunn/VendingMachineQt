#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include "adminwindow.h"
#include "userwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_adminBtn_clicked();

    void on_userBtn_clicked();

private:
    Ui::MainWindow *ui;
    AdminWindow *adminWindow;
    UserWindow *userWindow;

};
