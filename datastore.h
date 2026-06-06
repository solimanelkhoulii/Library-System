#ifndef DATASTORE_H
#define DATASTORE_H

#include <QObject>
#include <QMap>
#include <QList>
#include "user.h"
#include "patron.h"
#include "catalogueitem.h"
#include "mainwindow.h"

class DataStore : public QObject
{
    Q_OBJECT
public:
    static DataStore& instance();

    User* authenticateUser(const QString& username);
    Patron* currentPatron() const;
    void setCurrentUser(User* user);
    User* getCurrentUser();
    QList<CatalogueItem*> allItems() const { return m_items.values(); }
    QList<Patron*> allPatrons() const;
    CatalogueItem* getItem(int id);
    void addItem(CatalogueItem* item, const QVariantMap& fields);
    void removeItem(int id);
    void resetData();
    int lastIndex();
    void clear();
    QMap<int, CatalogueItem*> m_items;
    
    // Database persistence methods for Patron loans and holds
    void saveLoanToDatabase(const QString& patronUsername, int itemId, const QDate& dueDate);
    void deleteLoanFromDatabase(const QString& patronUsername, int itemId);
    void saveHoldToDatabase(const QString& patronUsername, int itemId, int position);
    void deleteHoldFromDatabase(const QString& patronUsername, int itemId);
    void loadPatronLoansAndHolds(Patron* patron);
    void updateCatalogueItemAvailability(int itemId, int availableCopies);

private:
    explicit DataStore(QObject *parent = nullptr);
    void initializeUsers();
    void initializeCatalogue();
    void initializePatronLoansAndHolds();
    void loadCatalogueFromDatabase();
    void loadCatalogueAvailabilityFromDatabase();



    QMap<QString, User*> m_users;

    User* m_currentUser = nullptr;
};

#endif // DATASTORE_H
