#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QDebug>

void executeSQL(QSqlDatabase &db, const QString &sql);
bool checkDatabaseExists(const QString &dbName);

int setup_db()
{
    QString database_name = "my_database.db";

    // Check if the database already exists
    if (checkDatabaseExists(database_name))
    {
        qDebug() << "Database already exists, skipping creation and data population.";
        return 0; // No need to create the database again
    }

    // Create and open the database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(database_name);

    if (!db.open())
    {
        qDebug() << "Can't open database:" << db.lastError().text();
        return 1;
    }
    else
    {
        qDebug() << "Opened database successfully:" << database_name;
    }

    // Create tables - one at a time
    QString createTableSQL1 = "CREATE TABLE IF NOT EXISTS stocks_67011653 (item_id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL UNIQUE, price INTEGER NOT NULL CHECK (price > 0), quantity INTEGER NOT NULL);";
    executeSQL(db, createTableSQL1);

    QString createTableSQL2 = "CREATE TABLE IF NOT EXISTS change_box (id INTEGER PRIMARY KEY AUTOINCREMENT, denomination INTEGER NOT NULL UNIQUE, quantity INTEGER NOT NULL CHECK(quantity >= 0 AND quantity <= 30));";
    executeSQL(db, createTableSQL2);

    QString createTableSQL3 = "CREATE TABLE IF NOT EXISTS collection_box (id INTEGER PRIMARY KEY AUTOINCREMENT, denomination INTEGER NOT NULL UNIQUE, quantity INTEGER NOT NULL CHECK(quantity >= 0 AND quantity <= 30));";
    executeSQL(db, createTableSQL3);

    // Populate the tables - one insert at a time
    QString populate_items1 = "INSERT OR IGNORE INTO stocks_67011653 (name, price, quantity) VALUES ('Coke', 30, 0);";
    executeSQL(db, populate_items1);

    QString populate_items2 = "INSERT OR IGNORE INTO stocks_67011653 (name, price, quantity) VALUES ('Pepsi', 23, 0);";
    executeSQL(db, populate_items2);

    QString populate_items3 = "INSERT OR IGNORE INTO stocks_67011653 (name, price, quantity) VALUES ('Snickers', 30, 30);";
    executeSQL(db, populate_items3);

    QString populate_collection_box1 = "INSERT OR IGNORE INTO collection_box (denomination, quantity) VALUES (1, 0);";
    executeSQL(db, populate_collection_box1);

    QString populate_collection_box2 = "INSERT OR IGNORE INTO collection_box (denomination, quantity) VALUES (5, 0);";
    executeSQL(db, populate_collection_box2);

    QString populate_collection_box3 = "INSERT OR IGNORE INTO collection_box (denomination, quantity) VALUES (10, 0);";
    executeSQL(db, populate_collection_box3);

    QString populate_collection_box4 = "INSERT OR IGNORE INTO collection_box (denomination, quantity) VALUES (20, 0);";
    executeSQL(db, populate_collection_box4);

    QString populate_collection_box5 = "INSERT OR IGNORE INTO collection_box (denomination, quantity) VALUES (100, 0);";
    executeSQL(db, populate_collection_box5);

    QString populate_change_box1 = "INSERT OR IGNORE INTO change_box (denomination, quantity) VALUES (1, 30);";
    executeSQL(db, populate_change_box1);

    QString populate_change_box2 = "INSERT OR IGNORE INTO change_box (denomination, quantity) VALUES (5, 1);";
    executeSQL(db, populate_change_box2);

    QString populate_change_box3 = "INSERT OR IGNORE INTO change_box (denomination, quantity) VALUES (10, 2);";
    executeSQL(db, populate_change_box3);

    QString populate_change_box4 = "INSERT OR IGNORE INTO change_box (denomination, quantity) VALUES (20, 1);";
    executeSQL(db, populate_change_box4);

    QString populate_change_box5 = "INSERT OR IGNORE INTO change_box (denomination, quantity) VALUES (100, 30);";
    executeSQL(db, populate_change_box5);

    // Close the database
    db.close();
    qDebug() << "Database closed.";

    return 0;
}

void executeSQL(QSqlDatabase &db, const QString &sql)
{
    QSqlQuery query(db);
    if (!query.exec(sql))
    {
        qDebug() << "SQL error:" << query.lastError().text();
    }
    else
    {
        qDebug() << "SQL executed successfully.";
    }
}

bool checkDatabaseExists(const QString &dbName)
{
    // Check if the database file exists in the root folder
    return QFile::exists(dbName);
}
