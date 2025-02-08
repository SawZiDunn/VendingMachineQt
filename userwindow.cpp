#include "userwindow.h"
#include "ui_userwindow.h"
#include "moneyInstance.h"
#include <QMessageBox>
#include <QTimer>
#include <string>

UserWindow::UserWindow(QMainWindow *parent)
    : QMainWindow(parent)
    , ui(new Ui::UserWindow)
{

    ui->setupUi(this);
    displayItems();

    // Check conditions before doing anything else
    if (isOutOfStock())
    {
        QMessageBox::information(this, "OUT OF STOCK!", "\nThe Vending Machine is OUT OF STOCK!");
        QTimer::singleShot(0, this, &UserWindow::close);
        return;
    }
    else if (isNoChange())
    {
        QMessageBox::information(this, "Empty Change Box!", "\nChange Box is empty! You cannot make any purchase!");
        QTimer::singleShot(0, this, &UserWindow::close);
        return;
    }
    else if (isCollectBoxFull())
    {
        QMessageBox::information(this, "Full Collection Box!", "Collection Box is full! You cannot make any purchase!");
        QTimer::singleShot(0, this, &UserWindow::close);
        return;
    }
}

UserWindow::~UserWindow()
{
    delete ui;
}

void UserWindow::displayItems()
{
    // Clear any existing data
    ui->tableWidget->clear();

    // Set table headers
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels({"ID", "Name", "Price"});

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

        // Populate table
        ui->tableWidget->setItem(row, 0, idItem);
        ui->tableWidget->setItem(row, 1, nameItem);
        ui->tableWidget->setItem(row, 2, priceItem);

        row++;
    }


}

void UserWindow::on_purchaseBtn_clicked()
{

    // Get the item ID from the item ID spin box
    int itemId = ui->itemId->value();
    auto item = db.get_single_item(std::to_string(itemId));

    if (item.item_id == -1)
    {
        // Provide user feedback
        QMessageBox::information(this, "Invalid ID", "Product does not exist!");
        return;
    }

    if (item.quantity == 0)
    {
        QMessageBox::information(this, "Out of Stock", "THIS ITEM IS OUT OF STOCK!");
        return;
    }

    vector<MoneyInstance> money_instance_collections{};
    int total_inserted_money = getTotalInsertedMoney(money_instance_collections);



    if (total_inserted_money < item.price) {
        QMessageBox::information(this, "Not Enough Money!", "Not Enough Money! Please insert more coins!");
        return;
    }

    int total_change = total_inserted_money - item.price;

    vector<MoneyInstance> refundableChange = getRefundableChange(total_change);

    if (willCollectBoxFull(money_instance_collections))
    {
        QString message = "Sorry! The money collection box is full!\n"
                                                  "Here is your refund: " + QString::number(item.price) + " Baht.";
        QMessageBox::information(this, "Collection Box Full!" , message);
        return;
    }
    else if (refundableChange.empty() && total_change != 0)
    {
        QString message = "Change box does not have enough change to return!\n"
                          "You cannot make this purchase!\n"
                          "Please take back your money: " + QString::number(total_change + item.price) + " Baht.";
        QMessageBox::information(this, "Not Enough Change!", message);
        return;
    }

    // complete final transaction
    insertMoneyToCollectionBox(money_instance_collections);

    db.decrease_item_quantity(to_string(item.item_id));

    decreaseMoneyFromChangeBox(refundableChange);

    // Prepare the message
    QString msg = "You successfully purchased " + QString::fromStdString(item.name) +
                  "\nTotal Change: " + QString::number(total_change) + " Baht.";

    if (total_change > 0) {
        msg += "\n\nYou received:";
    }

    // Append each change detail to the message
    for (const MoneyInstance &box : refundableChange) {
        if (box.quantity > 0) {
            msg += "\n" + QString::number(box.quantity) + " notes of " +
                   QString::number(box.denomination) + " Baht";
        }
    }

    // Display in a single message box
    QMessageBox::information(this, "Successful Purchase!", msg);


}

void UserWindow::addMoneyInstance(int amount, int count, vector<MoneyInstance> &money_instance_collections)
{
    for (auto &i : money_instance_collections)
    {
        if (i.denomination == amount)
        {
            i.quantity += count; // Add 'count' instead of just 1
            return;
        }
    }

    // If denomination not found, create a new entry
    MoneyInstance moneybox;
    moneybox.denomination = amount;
    moneybox.quantity = count; // Set the initial quantity to 'count'
    money_instance_collections.push_back(moneybox);
}

// Function to calculate refundable change
vector<MoneyInstance> UserWindow::getRefundableChange(int total_change)
{
    int temp_total_change = 0;
    vector<MoneyInstance> db_change_instances = db.get_money_instances("change_box");
    vector<MoneyInstance> refundable_change;

    // sort instances in descending order
    sort_money_instances(db_change_instances);

    for (MoneyInstance &i : db_change_instances)
    {
        while (i.quantity > 0 && i.denomination + temp_total_change <= total_change)
        {
            temp_total_change += i.denomination;
            --i.quantity;
            addMoneyInstance_single(i.denomination, refundable_change);
        }

        // immediately return if total change is achieved
        if (temp_total_change == total_change)
        {
            return refundable_change;
        }
    }

    // empty vector if total change is not refundable

    return {};
}

int UserWindow::getTotalInsertedMoney(vector<MoneyInstance>& money_instance_collections) {
    // Get values from the five spin boxes
    int one_baht_count = ui->one_baht->value();
    int five_baht_count = ui->five_baht->value();
    int ten_baht_count = ui->ten_baht->value();
    int twenty_baht_count = ui->twenty_baht->value();
    int hundred_baht_count = ui->hundred_baht->value();

    addMoneyInstance(1, one_baht_count, money_instance_collections);
    addMoneyInstance(5, five_baht_count, money_instance_collections);
    addMoneyInstance(10, ten_baht_count, money_instance_collections);
    addMoneyInstance(20, twenty_baht_count, money_instance_collections);
    addMoneyInstance(100, hundred_baht_count, money_instance_collections);


    int total{0};
    for (auto moneyInstance : money_instance_collections) {
        total += moneyInstance.denomination * moneyInstance.quantity;
    }

    return total;
}

// create a money instance and push to the original vector
void UserWindow::addMoneyInstance_single(int amount, vector<MoneyInstance> &money_instance_collections)
{

    for (auto &i : money_instance_collections)
    {
        if (i.denomination == amount)
        {
            ++i.quantity;
            return;
        }
    }

    MoneyInstance moneybox;
    moneybox.denomination = amount;
    moneybox.quantity = 1;
    money_instance_collections.push_back(moneybox);
}

// sort money instances according to denomination in descending order
void UserWindow::sort_money_instances(vector<MoneyInstance> &money_instance_collection)
{
    for (int i = 0; i < money_instance_collection.size() - 1; ++i)
    {
        for (int j = 0; j < money_instance_collection.size() - i - 1; ++j)
        {
            if (money_instance_collection[j].denomination < money_instance_collection[j + 1].denomination)
            {
                // Swap adjacent elements
                MoneyInstance temp = money_instance_collection[j];
                money_instance_collection[j] = money_instance_collection[j + 1];
                money_instance_collection[j + 1] = temp;
            }
        }
    }
}

// check if the collection box will be full after user inserts coins
bool UserWindow::willCollectBoxFull(const vector<MoneyInstance> &money_collections)
{
    vector<MoneyInstance> money_instances = db.get_money_instances("collection_box");

    for (const auto &box : money_collections)
    {

        for (const auto &instance : money_instances)
        {
            if (box.denomination == instance.denomination && box.quantity + instance.quantity > 30)
            {
                return true;
            }
        }
    }
    return false;
}

void UserWindow::insertMoneyToCollectionBox(vector<MoneyInstance> money_collections)
{
    for (auto box : money_collections)
    {
        db.insert_money(box.denomination, box.quantity);
    }
}

bool UserWindow::decreaseMoneyFromChangeBox(vector<MoneyInstance> refundableChange)
{
    if (refundableChange.empty())
    {
        return false;
    }

    for (auto box : refundableChange)
    {
        db.decrease_money(box.denomination, box.quantity);
    }
    return true;
}

bool UserWindow::isOutOfStock()
{
    vector<Item> items = db.get_all_items("stocks_67011653");
    int emptyCount = 0;
    for (Item &item : items)
    {
        if (item.quantity == 0)
        {
            ++emptyCount;
        }
    }

    return static_cast<float>(emptyCount) / static_cast<float>(items.size()) >= 0.5;
}

bool UserWindow::isNoChange()
{
    const vector<MoneyInstance> moneyboxes = db.get_money_instances("change_box");
    for (const MoneyInstance &box : moneyboxes)
    {
        if (box.quantity == 0)
        {
            return true;
        }
    }

    return false;
}
bool UserWindow::isCollectBoxFull()
{
    const vector<MoneyInstance> moneyboxes = db.get_money_instances("collection_box");
    for (const MoneyInstance &box : moneyboxes)
    {
        if (box.quantity == 30)
        {
            return true;
        }
    }

    return false;
}

void UserWindow::updateTotalLabel() {

    vector<MoneyInstance> money_instance_collections{};
    int total_inserted_money = getTotalInsertedMoney(money_instance_collections);

    QString text = "Total: " + QString::number(total_inserted_money) + " Baht";
    ui->totalLabel->setText(text);

}


void UserWindow::on_backToRoleSelection_clicked()
{
    this->close();
}


void UserWindow::on_one_baht_valueChanged(int arg1)
{
    updateTotalLabel();
}

void UserWindow::on_five_baht_valueChanged(int arg1)
{
    updateTotalLabel();
}

void UserWindow::on_ten_baht_valueChanged(int arg1)
{
    updateTotalLabel();
}

void UserWindow::on_twenty_baht_valueChanged(int arg1)
{
    updateTotalLabel();
}


void UserWindow::on_hundred_baht_valueChanged(int arg1)
{
    updateTotalLabel();
}

