#pragma once

#include "moneyInstance.h"
#include <iostream>
#include <string>
#include <vector>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

using namespace std;

// represents each item
struct Item
{
    int item_id;
    string name;
    float price;
    int quantity;
};

class DatabaseController
{
public:
    DatabaseController()
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("my_database.db");

        if (!db.open())
        {
            cerr << "Can't open database: " << db.lastError().text().toStdString() << endl;
        }
    }

    ~DatabaseController()
    {
        cout << "db is closed" << endl;
        db.close(); // Close the database when done
    }


    // add a new item
    bool add_stock(const string& name, int price, int quantity)
    {

        QSqlQuery query;
        query.prepare("INSERT INTO stocks_67011653 (name, price, quantity) VALUES (:name, :price, :quantity)");
        query.bindValue(":name", QString::fromStdString(name));
        query.bindValue(":price", price);
        query.bindValue(":quantity", quantity);

        if (!query.exec())
        {
            cerr << "SQL error: " << query.lastError().text().toStdString() << endl;
            return false;
        }
        else
        {
            cout << "Stock added successfully!" << endl;
            return true;
        }

        return true;
    }

    vector<Item> get_all_items(const string& tableName)
    {
        vector<Item> items;

        QSqlQuery query;

        if (!query.exec(QString::fromStdString("SELECT * FROM " + tableName))) {
            cerr << "SQL Error: " << query.lastError().text().toStdString() << endl;
            return items;
        }

        while (query.next())
        {
            Item item;
            item.item_id = query.value(0).toInt();
            item.name = query.value(1).toString().toStdString();
            item.price = query.value(2).toFloat();
            item.quantity = query.value(3).toInt();

            items.push_back(item);
        }

        return items;

    }

    Item get_single_item(const string& item_id)
    {
        Item item;
        QSqlQuery query;
        query.prepare("SELECT * FROM stocks_67011653 WHERE item_id = :item_id");
        query.bindValue(":item_id", QString::fromStdString(item_id));

        if (query.exec() && query.next())
        {
            item.item_id = query.value(0).toInt();
            item.name = query.value(1).toString().toStdString();
            item.price = query.value(2).toFloat();
            item.quantity = query.value(3).toInt();
        }
        else
        {
            item.item_id = -1; // Product not found
        }

        return item;
    }

    bool refill_stock(const string& id, int quantity)
    {
        QSqlQuery query;
        query.prepare("SELECT quantity FROM stocks_67011653 WHERE item_id = :item_id");
        query.bindValue(":item_id", QString::fromStdString(id));

        int currentQuantity = 0;
        if (query.exec() && query.next())
        {
            currentQuantity = query.value(0).toInt();
        }
        else
        {
            cout << "Product not found!" << endl;
            return false;
        }

        int newQuantity = currentQuantity + quantity;

        query.prepare("UPDATE stocks_67011653 SET quantity = :quantity WHERE item_id = :item_id");
        query.bindValue(":quantity", newQuantity);
        query.bindValue(":item_id", QString::fromStdString(id));

        if (!query.exec())
        {
            cerr << "SQL error: " << query.lastError().text().toStdString() << endl;
            return false;
        }
        return true;
    }

    void decrease_item_quantity(const string& item_id)
    {
        QSqlQuery query;
        query.prepare("SELECT quantity FROM stocks_67011653 WHERE item_id = :item_id");
        query.bindValue(":item_id", QString::fromStdString(item_id));

        int currentQuantity = 0;
        if (query.exec() && query.next())
        {
            currentQuantity = query.value(0).toInt();
        }
        else
        {
            cout << "Product not found!" << endl;
            return;
        }

        int newQuantity = currentQuantity - 1;

        query.prepare("UPDATE stocks_67011653 SET quantity = :quantity WHERE item_id = :item_id");
        query.bindValue(":quantity", newQuantity);
        query.bindValue(":item_id", QString::fromStdString(item_id));

        if (!query.exec())
        {
            cerr << "SQL error: " << query.lastError().text().toStdString() << endl;
        }
    }

    void insert_money(int denomination, int quantity)
    {
        QSqlQuery query;
        query.prepare("SELECT quantity FROM collection_box WHERE denomination = :denomination");
        query.bindValue(":denomination", denomination);

        int currentQuantity = 0;
        if (query.exec() && query.next())
        {
            currentQuantity = query.value(0).toInt();
        }

        int newQuantity = currentQuantity + quantity;

        query.prepare("UPDATE collection_box SET quantity = :quantity WHERE denomination = :denomination");
        query.bindValue(":quantity", newQuantity);
        query.bindValue(":denomination", denomination);

        if (!query.exec())
        {
            cerr << "SQL error: " << query.lastError().text().toStdString() << endl;
        }
    }

    void decrease_money(int denomination, int quantity)
    {
        QSqlQuery query;
        query.prepare("SELECT quantity FROM change_box WHERE denomination = :denomination");
        query.bindValue(":denomination", denomination);

        int currentQuantity = 0;
        if (query.exec() && query.next())
        {
            currentQuantity = query.value(0).toInt();
        }

        int newQuantity = currentQuantity - quantity;

        query.prepare("UPDATE change_box SET quantity = :quantity WHERE denomination = :denomination");
        query.bindValue(":quantity", newQuantity);
        query.bindValue(":denomination", denomination);

        if (!query.exec())
        {
            cerr << "SQL error: " << query.lastError().text().toStdString() << endl;
        }
    }

    vector<MoneyInstance> get_money_instances(const string& table_name)
    {
        vector<MoneyInstance> money_instances;
        QSqlQuery query;
        query.exec(QString::fromStdString("SELECT denomination, quantity FROM " + table_name));

        while (query.next())
        {
            MoneyInstance moneyInstance;
            moneyInstance.denomination = query.value(0).toInt();
            moneyInstance.quantity = query.value(1).toInt();
            money_instances.push_back(moneyInstance);
        }

        return money_instances;
    }

    void refill_change_box()
    {
        QSqlQuery query;
        if (!query.exec("UPDATE change_box SET quantity = 30;"))
        {
            cerr << "SQL error: " << query.lastError().text().toStdString() << endl;
        }
    }

    void collect_collection_box()
    {
        QSqlQuery query;
        if (!query.exec("UPDATE collection_box SET quantity = 0;"))
        {
            cerr << "SQL error: " << query.lastError().text().toStdString() << endl;
        }
    }

    int check_money_box(const string& table_name)
    {
        int total = 0;

        QSqlQuery query;
        query.exec(QString::fromStdString("SELECT denomination, quantity FROM " + table_name));

        while (query.next())
        {
            int denomination = query.value(0).toInt();
            int quantity = query.value(1).toInt();
            total += denomination * quantity;
        }

        return total;
    }

    bool reopenDatabase(){
        db.close();
        if (!db.open()) {
            cerr << "Failed to reopen the database: " << db.lastError().text().toStdString() << endl;
            return false;  // Return false if the database could not be opened
        }

        cout << "Database reopened successfully!" << endl;
        return true;  // Return true if the database is reopened successfully
        return true;  // Database was already open

    }



private:
    QSqlDatabase db;
};
