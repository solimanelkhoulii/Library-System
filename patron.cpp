#include "patron.h"
#include "datastore.h"
#include <QDebug>

bool Patron::borrowItem(CatalogueItem* item)
{
    if (!canBorrow() || !item->isAvailable()) {
        return false;
    }

    // Check if patron already has an active loan for this item (D1 Feedback Fix)
    for (const Loan& loan : m_activeLoans) {
        if (loan.itemId() == item->id()) {
            return false; // Patron already has this item borrowed
        }
    }

    // Check if item has a hold queue and if the current patron is the next in line (D1 Feedback Fix)
    if (!item->holdQueue().isEmpty()) {
        if (item->holdQueue().first() != username()) {
            return false; // Cannot borrow, must be next in hold queue
        }
    }

    if (item->borrow()) {
        QDate dueDate = QDate::currentDate().addDays(14);
        m_activeLoans.append(Loan(item->id(), dueDate));
        
        // Persist the loan to the database
        DataStore::instance().saveLoanToDatabase(username(), item->id(), dueDate);
        
        return true;
    }
    return false;
}

bool Patron::returnItem(int itemId)
{
    // 1. Find and remove the loan from the patron's list
    int index = -1;
    for (int i = 0; i < m_activeLoans.size(); ++i) {
        if (m_activeLoans.at(i).itemId() == itemId) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        return false; // Loan not found
    }

    m_activeLoans.removeAt(index);

    // 2. Update the item status in the catalogue
    CatalogueItem* item = DataStore::instance().getItem(itemId);
    if (item) {
        item->returnItem();
        
        // Persist the loan removal to the database
        DataStore::instance().deleteLoanFromDatabase(username(), itemId);
        
        return true;
    }

    return false;
}

bool Patron::placeHold(CatalogueItem* item)
{
    if (item->isAvailable()) {
        return false; // Can only place hold on checked-out items
    }

    // Check if patron already has an active loan for this item (D1 Feedback Fix)
    for (const Loan& loan : m_activeLoans) {
        if (loan.itemId() == item->id()) {
            return false; // Cannot place a hold on an item you have checked out
        }
    }

    if (!m_activeHolds.contains(item->id())) {
        item->placeHold(username());
        m_activeHolds.append(item->id());
        
        // Persist the hold to the database
        int position = item->holdQueuePosition(username());
        DataStore::instance().saveHoldToDatabase(username(), item->id(), position);
        
        return true;
    }
    return false;
}

bool Patron::cancelHold(int itemId)
{
    if (m_activeHolds.contains(itemId)) {
        m_activeHolds.removeAll(itemId);
        CatalogueItem* item = DataStore::instance().getItem(itemId);
        if (item) {
            item->cancelHold(username());
            
            // Persist the hold removal to the database
            DataStore::instance().deleteHoldFromDatabase(username(), itemId);
        }
        return true;
    }
    return false;
}

void Patron::resetState()
{
    m_activeLoans.clear();
    m_activeHolds.clear();
}
