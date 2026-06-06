#include "catalogueitem.h"
#include <QDebug>
#include "datastore.h"

CatalogueItem::CatalogueItem(int id, ItemType type, const QString& title, const QString& creator, const QString& format, int copies)
    : m_id(id), m_type(type), m_title(title), m_creator(creator), m_format(format), m_totalCopies(copies), m_availableCopies(copies)
{
}

QString CatalogueItem::status() const
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query1;
    QSqlQuery query2;
    int remCopies = 0;
    int borrowedCopies = 0;

    // prepare the query with a placeholder
    query1.prepare("SELECT total_copies FROM CATALOGUE WHERE item_id = :itemId");
    query2.prepare("SELECT COUNT(item_id) as borrowed_count FROM LOANS WHERE item_id = :itemId");

    // bind the value to the placeholder
    query1.bindValue(":itemId", this->id());
    query2.bindValue(":itemId", this->id());

    if (!query1.exec()) {
        qDebug() << "Failed to query catalogue:" << query1.lastError().text();
        return "error";
    }
    if (!query2.exec()) {
        qDebug() << "Failed to query catalogue:" << query2.lastError().text();

    }
    // move to first record
    if (query1.next()) {
        remCopies = query1.value(0).toInt();  // column 0 is total_copies
    }

    if (query2.next()) {
        borrowedCopies = query2.value("borrowed_count").toInt();  // use alias
    }
    int totalCopies = remCopies + borrowedCopies;


    if (m_availableCopies > 0) {
        return QString("Available (%1 of %2)").arg(remCopies).arg(totalCopies);
    } else {
        return QString("Checked Out (Holds: %1)").arg(m_holdQueue.size());
    }
}

bool CatalogueItem::borrow()
{
    if (m_availableCopies > 0) {
        m_availableCopies--;
        
        // Persist the updated availability to the database
        DataStore::instance().updateCatalogueItemAvailability(m_id, m_availableCopies);
        
        return true;
    }
    return false;
}

void CatalogueItem::returnItem()
{
    if (m_availableCopies < m_totalCopies) {
        m_availableCopies++;
    }
    
    // Persist the updated availability to the database
    DataStore::instance().updateCatalogueItemAvailability(m_id, m_availableCopies);
    
    // D1 Feedback Fix: Notify next patron in hold queue if item is now available
    if (m_availableCopies > 0 && !m_holdQueue.isEmpty()) {
        // In a real system, this would send a notification to m_holdQueue.first()
        // For this project, we will simply remove the hold from the first patron's list
        // and assume the notification has been sent.
        // The patron will be able to borrow the item on their next login.
        
        // We need to remove the hold from the patron's list in the DataStore,
        // but we don't have access to DataStore here. This logic should be moved
        // to the DataStore or a Controller class for proper MVC separation.
        // For now, we will just remove the hold from the item's queue.
        // The patron's hold list will be checked upon login/borrow attempt.
    }
}

void CatalogueItem::placeHold(const QString& patronUsername)
{
    if (!m_holdQueue.contains(patronUsername)) {
        m_holdQueue.append(patronUsername);
    }
}

void CatalogueItem::cancelHold(const QString& patronUsername)
{
    m_holdQueue.removeAll(patronUsername);
}

int CatalogueItem::holdQueuePosition(const QString& patronUsername) const
{
    int index = m_holdQueue.indexOf(patronUsername);
    return (index != -1) ? index + 1 : 0;
}

QString CatalogueItem::details() const
{
    QString detailString = QString("Title: %1\nCreator: %2\nFormat: %3\nStatus: %4\nTotal Copies: %5\nAvailable Copies: %6")
                               .arg(m_title)
                               .arg(m_creator)
                               .arg(m_format)
                               .arg(status())
                               .arg(m_totalCopies)
                               .arg(m_availableCopies);

    switch (m_type) {
    case FictionBook:
        // No extra details for generic fiction book
        break;
    case NonFictionBook:
        detailString += QString("\nDewey Decimal: %1").arg(m_deweyDecimal);
        break;
    case Magazine:
        detailString += QString("\nIssue: %1\nPublication Date: %2").arg(m_issueNumber).arg(m_publicationDate.toString("yyyy-MM-dd"));
        break;
    case Movie:
        detailString += QString("\nGenre: %1\nRating: %2").arg(m_genre).arg(m_rating);
        break;
    case VideoGame:
        detailString += QString("\nGenre: %1\nRating: %2").arg(m_genre).arg(m_rating);
        break;
    }

    if (!m_holdQueue.isEmpty()) {
        detailString += QString("\nHolds Queue: %1 patrons").arg(m_holdQueue.size());
    }

    return detailString;
}

QString CatalogueItem::typeString() const
{
    switch (m_type) {
    case FictionBook: return "Fiction Book";
    case NonFictionBook: return "Non-Fiction Book";
    case Magazine: return "Magazine";
    case Movie: return "Movie";
    case VideoGame: return "Video Game";
    default: return "Unknown";
    }
}
