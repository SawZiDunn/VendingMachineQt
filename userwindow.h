#pragma once

#include <QWidget>
#include "db_controller.h"
#include "ui_userwindow.h" // Include the UI file
#include "moneyInstance.h"

class UserWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserWindow(QMainWindow *parent = nullptr);
    ~UserWindow(); // Destructor to clean up

private slots:

    void on_purchaseBtn_clicked();

    void on_backToRoleSelection_clicked();

    void on_one_baht_valueChanged(int arg1);

    void on_twenty_baht_valueChanged(int arg1);

    void on_five_baht_valueChanged(int arg1);

    void on_ten_baht_valueChanged(int arg1);

    void on_hundred_baht_valueChanged(int arg1);

private:
    Ui::UserWindow *ui;   // Pointer to UI
    DatabaseController db; // Database controller instance
    void displayItems();
    int getTotalInsertedMoney(vector<MoneyInstance>& money_instance_collections);
    vector<MoneyInstance> getRefundableChange(int total_change);
    void addMoneyInstance(int amount, int count, vector<MoneyInstance> &money_instance_collections);
    void addMoneyInstance_single(int amount, vector<MoneyInstance> &money_instance_collections);
    void sort_money_instances(vector<MoneyInstance> &money_instance_collection);
    bool willCollectBoxFull(const vector<MoneyInstance> &money_collections);
    void insertMoneyToCollectionBox(vector<MoneyInstance> money_collections);
    bool decreaseMoneyFromChangeBox(vector<MoneyInstance> refundableChange);
    bool isCollectBoxFull();
    bool isNoChange();
    bool isOutOfStock();
    void updateTotalLabel();


};
