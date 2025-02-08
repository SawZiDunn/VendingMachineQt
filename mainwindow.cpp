#include "mainwindow.h"
#include "adminwindow.h"
#include "userwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), adminWindow(nullptr), userWindow(nullptr)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete adminWindow;
    delete userWindow;
}


void MainWindow::on_adminBtn_clicked()
{
    if (!adminWindow) {  // Create window only if it doesn't exist
        adminWindow = new AdminWindow(this);
    }

    // this -> close();
    adminWindow->show();
}


void MainWindow::on_userBtn_clicked()
{
    // Delete existing window if it exists
    if (userWindow) {
        delete userWindow;
        userWindow = nullptr;
    }

    // Create a new window (constructor will be called)
    userWindow = new UserWindow(this);
    userWindow->show();
}

