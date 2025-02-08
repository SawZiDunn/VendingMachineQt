#include "refillstock.h"
#include "ui_refillstock.h"
#include <QMessageBox>

RefillStock::RefillStock(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RefillStock)
{
    ui->setupUi(this);
}

RefillStock::~RefillStock()
{
    delete ui;
}

void RefillStock::on_refillBtn_clicked()
{
    QString id = ui->productId->text();  // Get product name from QLineEdit
    int quantity = ui->quantity->text().toInt();  // Get quantity from QLineEdit

    // Input validation
    if (id.isEmpty() || quantity <= 0) {
        QMessageBox::warning(this, "Input Error", "Please enter valid data for all fields please.");
    } else {


        if (db.refill_stock(id.toStdString(), quantity)) {
            QMessageBox::information(this, "Success", "Stock Refilled successfully!");
            this->close();

        } else {
            QMessageBox::information(this, "Failed!", "Invalid Stock ID!");
        }




    }
}

