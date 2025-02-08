#pragma once

#include <QMainWindow>
#include "db_controller.h"

namespace Ui {
class RefillStock;
}

class RefillStock : public QMainWindow
{
    Q_OBJECT

public:
    explicit RefillStock(QWidget *parent = nullptr);
    ~RefillStock();

private slots:
    void on_refillBtn_clicked();

private:
    Ui::RefillStock *ui;
    DatabaseController db;

};
