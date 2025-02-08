#pragma once

#include "db_controller.h"
#include "ui_adminwindow.h" // Include the UI file
#include "addstock.h"
#include "refillstock.h"

class AdminWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminWindow(QMainWindow *parent = nullptr);
    ~AdminWindow(); // Destructor to clean up



private slots:

    void on_addStockBtn_clicked();

    void on_refreshBtn_clicked();

    void on_collectColBoxBtn_clicked();

    void on_checkColBox_clicked();

    void on_refillChangeBox_clicked();

    void on_checkChgBox_clicked();

    void on_refillStockBtn_clicked();

    void on_backToroleSelection_clicked();

private:
    Ui::AdminWindow *ui;   // Pointer to UI
    AddStock *addStock;
    RefillStock *refillStock;
    void displayItems();

    DatabaseController db; // Database controller instance
};
