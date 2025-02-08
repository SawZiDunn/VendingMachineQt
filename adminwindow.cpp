#include "adminwindow.h"
#include <QMessageBox>
#include "ui_adminwindow.h"
#include "addstock.h"
#include "refillstock.h"
#include "db_controller.h"
#include <string>

AdminWindow::AdminWindow(QMainWindow *parent)
    : QMainWindow(parent)
    , ui(new Ui::AdminWindow), addStock(nullptr), refillStock(nullptr)
{
    ui->setupUi(this); // Corrected UI initialization
    displayItems();
}

AdminWindow::~AdminWindow()
{
    delete ui;
    delete addStock;
}

void AdminWindow::on_addStockBtn_clicked()
{
    if (!addStock) {  // Create window only if it doesn't exist
        addStock = new AddStock(this);
    }

    addStock->show();
}

void AdminWindow::displayItems()
{
    // Clear any existing data
    ui->tableWidget->clear();

    // Set table headers
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels({"ID", "Name", "Price", "Quantity"});

    // Remove default row numbers
    ui->tableWidget->verticalHeader()->setVisible(false);

    // Make all columns take equal width
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Fetch all items from database
    vector<Item> items = db.get_all_items("stocks_67011653");

    ui->tableWidget->setRowCount(0); // Reset table

    int row = 0;
    for (const Item &item : items) {
        ui->tableWidget->insertRow(row); // Add new row

        // Create table items with centered alignment
        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(item.item_id));
        idItem->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *nameItem = new QTableWidgetItem(QString::fromStdString(item.name));
        nameItem->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *priceItem = new QTableWidgetItem(QString::number(item.price));
        priceItem->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *quantityItem = new QTableWidgetItem(QString::number(item.quantity));
        quantityItem->setTextAlignment(Qt::AlignCenter);

        // Populate table
        ui->tableWidget->setItem(row, 0, idItem);
        ui->tableWidget->setItem(row, 1, nameItem);
        ui->tableWidget->setItem(row, 2, priceItem);
        ui->tableWidget->setItem(row, 3, quantityItem);

        row++;
    }
}


void AdminWindow::on_refreshBtn_clicked()
{
    displayItems();
}


void AdminWindow::on_collectColBoxBtn_clicked()
{
    int total = db.check_money_box("collection_box");
    db.collect_collection_box();
    string msg = "You have collected " + std::to_string(total) + " Baht from the collection box!";
    QMessageBox::information(this, "Success", QString::fromStdString(msg));
}

void AdminWindow::on_checkColBox_clicked()
{
    int total = db.check_money_box("collection_box");
    string msg = "You have " + std::to_string(total) + " Baht in the collection box!";
    QMessageBox::information(this, "Success", QString::fromStdString(msg));
}

void AdminWindow::on_refillChangeBox_clicked()
{
    db.refill_change_box();
    QMessageBox::information(this, "Success", "Change Box successfully refilled!");
}

void AdminWindow::on_checkChgBox_clicked()
{
    int total = db.check_money_box("change_box");
    string msg = "You have " + std::to_string(total) + " Baht in the change box!";
    QMessageBox::information(this, "Success", QString::fromStdString(msg));
}


void AdminWindow::on_refillStockBtn_clicked()
{
    if (!refillStock) {  // Create window only if it doesn't exist

        refillStock = new RefillStock(this);  // Pass 'this' as the parent
    }

    refillStock->show();
}

void AdminWindow::on_backToroleSelection_clicked()
{
    this->close();
}

