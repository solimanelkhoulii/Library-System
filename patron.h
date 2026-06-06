#ifndef PATRON_H
#define PATRON_H

#include "user.h"
#include "catalogueitem.h"
#include <QDate>

class Loan
{
public:
    Loan(int itemId, const QDate& dueDate) : m_itemId(itemId), m_dueDate(dueDate) {}
    int itemId() const { return m_itemId; }
    QDate dueDate() const { return m_dueDate; }
    int daysRemaining() const { return QDate::currentDate().daysTo(m_dueDate); }

private:
    int m_itemId;
    QDate m_dueDate;
};

class Patron : public User
{
public:
    Patron(const QString& username) : User(username, User::Patron) {}

    // Patron features
    bool canBorrow() const { return m_activeLoans.size() < 3; }
    bool borrowItem(CatalogueItem* item);
    bool returnItem(int itemId);
    bool placeHold(CatalogueItem* item);
    bool cancelHold(int itemId);
    void resetState();

    QList<Loan> activeLoans() const { return m_activeLoans; }
    QList<int> activeHolds() const { return m_activeHolds; }

    // Public access for DataStore to load loans and holds from database
    QList<Loan> m_activeLoans;
    QList<int> m_activeHolds; // List of item IDs the patron has a hold on
};

#endif // PATRON_H
