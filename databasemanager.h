#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QList>
#include "catalogueitem.h"
#include "user.h"
#include "patron.h"

class DatabaseManager
{
public:
    static DatabaseManager& instance();
    
    bool initializeDatabase(const QString& dbPath);
    bool createTables();
    bool populateDefaultData();
    
    // Catalogue operations
    QList<CatalogueItem*> loadAllItems();
    CatalogueItem* loadItem(int id);
    bool saveItem(CatalogueItem* item);
    bool addItem(CatalogueItem* item);
    bool removeItem(int id);
    
    // User operations
    QList<User*> loadAllUsers();
    User* loadUser(const QString& username);
    bool saveUser(User* user);
    
    // Loan operations
    bool saveLoan(const QString& patronUsername, int itemId, const QDate& dueDate);
    bool removeLoan(const QString& patronUsername, int itemId);
    QList<Loan> loadPatronLoans(const QString& patronUsername);
    
    // Hold operations
    bool saveHold(const QString& patronUsername, int itemId);
    bool removeHold(const QString& patronUsername, int itemId);
    QList<int> loadPatronHolds(const QString& patronUsername);
    
    bool closeDatabase();
    
private:
    DatabaseManager();
    ~DatabaseManager();
    
    QSqlDatabase m_db;
    bool m_initialized;
};

#endif // DATABASEMANAGER_H
