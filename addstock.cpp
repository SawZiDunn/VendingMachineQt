#include "addstock.h"
#include "ui_addstock.h"

AddStock::AddStock(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AddStock)
{
    ui->setupUi(this);
}

AddStock::~AddStock()
{
    delete ui;
}

void AddStock::on_pushButton_clicked()
{
    QString name = ui->productName->text();  // Get product name from QLineEdit
    int price = ui->unitPrice->text().toInt();  // Get price from QLineEdit
    int quantity = ui->quantity->text().toInt();  // Get quantity from QLineEdit

    // Input validation
    if (name.isEmpty() || price <= 0 || quantity <= 0) {
        QMessageBox::warning(this, "Input Error", "Please enter valid data for all fields please.");
    } else {
        // Call the database function to add stock
        if (db.add_stock(name.toStdString(), price, quantity)) {
            // Provide user feedback
            QMessageBox::information(this, "Success", "A new item was added successfully!");
            this->close();

        } else {
            QMessageBox::information(this, "Failed!", "Invalid Stock ID!");
        }


    }

}

