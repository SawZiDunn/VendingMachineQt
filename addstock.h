#pragma once

#include <QMainWindow>
#include <QMessageBox>
#include "db_controller.h"

namespace Ui {
class AddStock;
}

class AddStock : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddStock(QWidget *parent = nullptr);
    ~AddStock();

private slots:
    void on_pushButton_clicked();

signals:
    void stockAdded();

private:
    Ui::AddStock *ui;
    DatabaseController db; // Database controller instance
};
