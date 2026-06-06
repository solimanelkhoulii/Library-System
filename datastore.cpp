#include "datastore.h"
#include <QDebug>
#include "patron.h"
#include <QMessageBox>

DataStore::DataStore(QObject *parent) : QObject(parent)
{
    initializeUsers();
    initializeCatalogue();
    initializePatronLoansAndHolds();
}

DataStore& DataStore::instance()
{
    static DataStore instance;
    return instance;
}

void DataStore::initializeUsers()
{
    // Clear existing users
    // Reset users is not required for D1, only items and patron loans
    // We keep the users in memory

    // 7 unique users: 5 patrons, 1 librarian, 1 system administrator
    // Patron users
    m_users.insert("Patron1", new Patron("Patron1"));
    m_users.insert("Patron2", new Patron("Patron2"));
    m_users.insert("Patron3", new Patron("Patron3"));
    m_users.insert("Patron4", new Patron("Patron4"));
    m_users.insert("Patron5", new Patron("Patron5"));

    // Librarian user
    m_users.insert("Librarian", new User("Librarian", User::Librarian));

    // System Administrator user
    m_users.insert("Admin", new User("Admin", User::Admin));
    // Check the users
    // qDebug() << "DataStore initialized with" << m_users.size() << "users.";
}

void DataStore::setCurrentUser(User* user)
{
    m_currentUser = user;
}

void DataStore::clear() {
    qDeleteAll(m_items);
    m_items.clear(); // Fix: prevent double-free on subsequent populateCatalogue() calls
}

User* DataStore::getCurrentUser() {
   return m_currentUser;
}

Patron* DataStore::currentPatron() const
{
    if (m_currentUser != nullptr) {
        return dynamic_cast<Patron*>(m_currentUser);
    }
    return nullptr;
}

QList<Patron*> DataStore::allPatrons() const
{
    QList<Patron*> patrons;
    for (User* user : m_users.values()) {
        if (user->type() == User::Patron) {
            patrons.append(dynamic_cast<Patron*>(user));
        }
    }
    return patrons;
}

void DataStore::initializeCatalogue()
{
    // Clear existing items
    qDeleteAll(m_items);
    m_items.clear();

    // 5 fiction books
    CatalogueItem* item1 = new CatalogueItem(1, CatalogueItem::FictionBook, "The Great Gatsby", "F. Scott Fitzgerald", "Hardcover", 2);
    m_items.insert(item1->id(), item1);
    CatalogueItem* item2 = new CatalogueItem(2, CatalogueItem::FictionBook, "1984", "George Orwell", "Paperback", 1);
    m_items.insert(item2->id(), item2);
    CatalogueItem* item3 = new CatalogueItem(3, CatalogueItem::FictionBook, "Pride and Prejudice", "Jane Austen", "Ebook", 3);
    m_items.insert(item3->id(), item3);
    CatalogueItem* item4 = new CatalogueItem(4, CatalogueItem::FictionBook, "To Kill a Mockingbird", "Harper Lee", "Hardcover", 1);
    m_items.insert(item4->id(), item4);
    CatalogueItem* item5 = new CatalogueItem(5, CatalogueItem::FictionBook, "The Catcher in the Rye", "J.D. Salinger", "Paperback", 2);
    m_items.insert(item5->id(), item5);

    // 5 non-fiction books (must include Dewey Decimal classification)
    CatalogueItem* item6 = new CatalogueItem(6, CatalogueItem::NonFictionBook, "Sapiens: A Brief History of Humankind", "Yuval Noah Harari", "Hardcover", 1);
    item6->setDeweyDecimal("909.82");
    m_items.insert(item6->id(), item6);
    CatalogueItem* item7 = new CatalogueItem(7, CatalogueItem::NonFictionBook, "Cosmos", "Carl Sagan", "Paperback", 2);
    item7->setDeweyDecimal("520");
    m_items.insert(item7->id(), item7);
    CatalogueItem* item8 = new CatalogueItem(8, CatalogueItem::NonFictionBook, "The Selfish Gene", "Richard Dawkins", "Hardcover", 1);
    item8->setDeweyDecimal("576.5");
    m_items.insert(item8->id(), item8);
    CatalogueItem* item9 = new CatalogueItem(9, CatalogueItem::NonFictionBook, "A Brief History of Time", "Stephen Hawking", "Ebook", 3);
    item9->setDeweyDecimal("523.1");
    m_items.insert(item9->id(), item9);
    CatalogueItem* item10 = new CatalogueItem(10, CatalogueItem::NonFictionBook, "Quiet: The Power of Introverts", "Susan Cain", "Paperback", 1);
    item10->setDeweyDecimal("155.232");
    m_items.insert(item10->id(), item10);

    // 3 magazines (must include issue numbers and publication dates)
    CatalogueItem* item11 = new CatalogueItem(11, CatalogueItem::Magazine, "National Geographic", "Various", "Print", 1);
    item11->setIssueNumber(12);
    item11->setPublicationDate(QDate(2025, 10, 1));
    m_items.insert(item11->id(), item11);
    CatalogueItem* item12 = new CatalogueItem(12, CatalogueItem::Magazine, "The Economist", "Various", "Print", 2);
    item12->setIssueNumber(45);
    item12->setPublicationDate(QDate(2025, 11, 5));
    m_items.insert(item12->id(), item12);
    CatalogueItem* item13 = new CatalogueItem(13, CatalogueItem::Magazine, "Wired", "Various", "Digital", 1);
    item13->setIssueNumber(9);
    item13->setPublicationDate(QDate(2025, 9, 15));
    m_items.insert(item13->id(), item13);

    // 3 movies (must include genre and rating)
    CatalogueItem* item14 = new CatalogueItem(14, CatalogueItem::Movie, "Inception", "Christopher Nolan", "DVD", 1);
    item14->setGenre("Sci-Fi");
    item14->setRating("PG-13");
    m_items.insert(item14->id(), item14);
    CatalogueItem* item15 = new CatalogueItem(15, CatalogueItem::Movie, "Parasite", "Bong Joon-ho", "Blu-ray", 2);
    m_items.insert(item15->id(), item15);
    CatalogueItem* item16 = new CatalogueItem(16, CatalogueItem::Movie, "Toy Story", "John Lasseter", "DVD", 1);
    item16->setGenre("Animation");
    item16->setRating("G");
    m_items.insert(item16->id(), item16);

    // 4 video games (must include genre and rating)
    CatalogueItem* item17 = new CatalogueItem(17, CatalogueItem::VideoGame, "The Legend of Zelda: Breath of the Wild", "Nintendo", "Switch Cartridge", 1);
    item17->setGenre("Action-Adventure");
    item17->setRating("E10+");
    m_items.insert(item17->id(), item17);
    CatalogueItem* item18 = new CatalogueItem(18, CatalogueItem::VideoGame, "Red Dead Redemption 2", "Rockstar Games", "PS4 Disc", 2);
    item18->setGenre("Action-Adventure");
    item18->setRating("M");
    m_items.insert(item18->id(), item18);
    CatalogueItem* item19 = new CatalogueItem(19, CatalogueItem::VideoGame, "Stardew Valley", "ConcernedApe", "PC", 1);
    item19->setGenre("Simulation");
    item19->setRating("E");
    m_items.insert(item19->id(), item19);
    CatalogueItem* item20 = new CatalogueItem(20, CatalogueItem::VideoGame, "Overwatch 2", "Blizzard Entertainment", "PC", 1);
    item20->setGenre("First-Person Shooter");
    item20->setRating("T");
    m_items.insert(item20->id(), item20);
    //check catalogue
    //qDebug() << "DataStore initialized with" << m_items.size() << "catalogue items.";

    // Load persisted availability status from database
    loadCatalogueAvailabilityFromDatabase();
}

void DataStore::loadCatalogueAvailabilityFromDatabase()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery q("SELECT item_id, total_copies FROM CATALOGUE");

    if (!q.exec()) {
        qDebug() << "Failed to load catalogue availability from database:" << q.lastError().text();
        return;
    }

    while (q.next()) {
        int id = q.value(0).toInt();
        int availableCopies = q.value(1).toInt();

        CatalogueItem* item = m_items.value(id, nullptr);
        if (item) {
            item->setAvailableCopies(availableCopies);
        }
    }
}

CatalogueItem* DataStore::getItem(int id)
{
    return m_items.value(id, nullptr);
}



void DataStore::resetData()
{
//    // Reset all items to available status and clear holds
//    for (CatalogueItem* item : m_items.values()) {
//        // Simple reset: re-initialize the catalogue
//        // We could also store initial state and restore it
//        // For D1, re-initializing is acceptable as per the requirement:
//        // "All items return to Available status."
//    }
    initializeCatalogue();
    
    // Reset patron accounts
    // "All patron accounts reset to zero borrowed items."
    // For now, we only re-initialize the catalogue.
    // Reset patron accounts (will be implemented later)
    // "All patron accounts reset to zero borrowed items."
    for (User* user : m_users.values()) {
        if (user->type() == User::Patron) {
            Patron* patron = dynamic_cast<Patron*>(user);
            if (patron) {
                patron->resetState();
            }
        }
    }
}

User* DataStore::authenticateUser(const QString& username)
{
    User* user = m_users.value(username, nullptr);
    if (user) {
        m_currentUser = user;
    }
    return user;
}

void DataStore::addItem(CatalogueItem* item, const QVariantMap& fields)
{
    if (item) {
        //add to datastore
        m_items.insert(item->id(), item);

        //connect to db
        QSqlDatabase db = QSqlDatabase::database();

        //add to db, based on item type
        if (!fields["title"].toString().isEmpty()) {
            qDebug() << "connecteed to db in DataStore::addItem()";

            //prepare the query, based on item type
            QSqlQuery q;
            switch (item->type()) {
                case CatalogueItem::FictionBook:
                    q.prepare("INSERT INTO CATALOGUE "
                                             "(item_id, item_type, title, author, is_available, queue_position) "
                                             "VALUES (:id, :type, :title, :author, 1, 0)");

                   q.bindValue(":id", item->id());
                   q.bindValue(":type", "Fiction");
                   q.bindValue(":title", fields.value("title").toString());
                   q.bindValue(":author", fields.value("author").toString());
                    break;
                case CatalogueItem::NonFictionBook:
                   q.prepare("INSERT INTO CATALOGUE "
                                             "(item_id, item_type, title, author, dewey_decimal, is_available, queue_position) "
                                             "VALUES (:id, :type, :title, :author, :dewey_decimal, 1, 0)");

                   q.bindValue(":id", item->id());
                   q.bindValue(":type", "Non-Fiction");
                   q.bindValue(":title", fields.value("title").toString());
                   q.bindValue(":author", fields.value("author").toString());
                   q.bindValue(":dewey_decimal", fields.value("deweyDecimal"));
                    break;
                case CatalogueItem::Magazine:
                    q.prepare("INSERT INTO CATALOGUE "
                                              "(item_id, item_type, title, author, issue_number, publication_date, is_available, queue_position) "
                                              "VALUES (:id, :type, :title, :author, :issue_number, :publication_date, 1, 0)");

                    q.bindValue(":id", item->id());
                    q.bindValue(":type", "Magazine");
                    q.bindValue(":title", fields.value("title").toString());
                    q.bindValue(":author", fields.value("author").toString());
                    q.bindValue(":dewey_decimal", fields.value("deweyDecimal"));
                    q.bindValue(":issue_number", fields.value("issue"));
                    q.bindValue("publication_date",fields.value("publicationDate"));
                        break;
                case CatalogueItem::Movie:
                    q.prepare("INSERT INTO CATALOGUE "
                                              "(item_id, item_type, title, author, issue_number, publication_date, genre, rating, is_available, queue_position) "
                                              "VALUES (:id, :type, :title, :author, :issue_number, :publication_date, :genre, :rating, 1, 0)");

                    q.bindValue(":id", item->id());
                    q.bindValue(":type", "Movie");
                    q.bindValue(":title", fields.value("title").toString());
                    q.bindValue(":author", fields.value("author").toString());
                    q.bindValue(":dewey_decimal", fields.value("deweyDecimal"));
                    q.bindValue(":issue_number", fields.value("issue"));
                    q.bindValue("publication_date",fields.value("publicationDate"));
                    q.bindValue(":genre", fields.value("genre"));
                    q.bindValue(":rating", fields.value("rating"));

                    break;
                case CatalogueItem::VideoGame:
                    q.prepare("INSERT INTO CATALOGUE "
                                              "(item_id, item_type, title, author, issue_number, publication_date, genre, rating, is_available, queue_position) "
                                              "VALUES (:id, :type, :title, :author, :issue_number, :publication_date, :genre, :rating, 1, 0)");

                    q.bindValue(":id", item->id());
                    q.bindValue(":type", "VideoGame");
                    q.bindValue(":title", fields.value("title").toString());
                    q.bindValue(":author", fields.value("author").toString());
                    q.bindValue(":dewey_decimal", fields.value("deweyDecimal"));
                    q.bindValue(":issue_number", fields.value("issue"));
                    q.bindValue("publication_date",fields.value("publicationDate"));
                    q.bindValue(":genre", fields.value("genre"));
                    q.bindValue(":rating", fields.value("rating"));
                    break;
                default:
                    break;

            }

            if (!q.exec()) {
                qDebug() << "Failed to add item to database:" << q.lastError().text() << item->typeString();

            } else {
                qDebug() << "Item added to database successfully.";

            }


        }
    }
}

void DataStore::removeItem(int id)
{
    CatalogueItem* item = m_items.take(id);

    if (item) {
        delete item;
        //remove from database -- test but it should work for just the item id also
        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery q;
        q.prepare("DELETE FROM CATALOGUE WHERE item_id = :id");
                q.bindValue(":id", id);

        if (!q.exec()) {
            qDebug() << "Failed to delete item from database:" << q.lastError().text();
        } else {
            qDebug() << "Item deleted from database successfully.";
        }
    }

}

int DataStore::lastIndex(){
    if (!m_items.isEmpty()) {
        return m_items.lastKey();
    }
    return 1;
}

// ============================================================================
// Database Persistence Methods for Patron Loans and Holds
// ============================================================================

void DataStore::initializePatronLoansAndHolds()
{
    // Load all loans and holds from the database for each patron
    for (User* user : m_users.values()) {
        if (user->type() == User::Patron) {
            Patron* patron = dynamic_cast<Patron*>(user);
            if (patron) {
                loadPatronLoansAndHolds(patron);
            }
        }
    }
}

void DataStore::loadPatronLoansAndHolds(Patron* patron)
{
    if (!patron) return;
    
    QSqlDatabase db = QSqlDatabase::database();
    
    // Load active loans for this patron
    QSqlQuery loansQuery;
    loansQuery.prepare("SELECT item_id, due_date FROM LOANS WHERE username = :username");
    loansQuery.bindValue(":username", patron->username());
    
    if (loansQuery.exec()) {
        while (loansQuery.next()) {
            int itemId = loansQuery.value(0).toInt();
            QDate dueDate = loansQuery.value(1).toDate();
            patron->m_activeLoans.append(Loan(itemId, dueDate));
        }
    } else {
        qDebug() << "Failed to load loans for patron" << patron->username() << ":" << loansQuery.lastError().text();
    }
    
    // Load active holds for this patron
    QSqlQuery holdsQuery;
    holdsQuery.prepare("SELECT item_id FROM HOLDS WHERE username = :username");
    holdsQuery.bindValue(":username", patron->username());
    
    if (holdsQuery.exec()) {
        while (holdsQuery.next()) {
            int itemId = holdsQuery.value(0).toInt();
            patron->m_activeHolds.append(itemId);
        }
    } else {
        qDebug() << "Failed to load holds for patron" << patron->username() << ":" << holdsQuery.lastError().text();
    }
}

void DataStore::saveLoanToDatabase(const QString& patronUsername, int itemId, const QDate& dueDate)
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery q;
    
    q.prepare("INSERT INTO LOANS (username, item_id, borrow_date, due_date) "
              "VALUES (:username, :item_id, :borrow_date, :due_date)");
    q.bindValue(":username", patronUsername);
    q.bindValue(":item_id", itemId);
    q.bindValue(":borrow_date", QDate::currentDate().toString("yyyy-MM-dd"));
    q.bindValue(":due_date", dueDate.toString("yyyy-MM-dd"));
    
    if (!q.exec()) {
        qDebug() << "Failed to save loan to database:" << q.lastError().text();
    } else {
        qDebug() << "Loan saved to database successfully for" << patronUsername << "on item" << itemId;
    }
}

void DataStore::deleteLoanFromDatabase(const QString& patronUsername, int itemId)
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery q;
    
    q.prepare("DELETE FROM LOANS WHERE username = :username AND item_id = :item_id");
    q.bindValue(":username", patronUsername);
    q.bindValue(":item_id", itemId);
    
    if (!q.exec()) {
        qDebug() << "Failed to delete loan from database:" << q.lastError().text();
    } else {
        qDebug() << "Loan deleted from database successfully for" << patronUsername << "on item" << itemId;
    }
}

void DataStore::saveHoldToDatabase(const QString& patronUsername, int itemId, int position)
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery q;
    
    q.prepare("INSERT INTO HOLDS (username, item_id, queue_position) "
              "VALUES (:username, :item_id, :queue_position)");
    q.bindValue(":username", patronUsername);
    q.bindValue(":item_id", itemId);
    q.bindValue(":queue_position", position);
    
    if (!q.exec()) {
        qDebug() << "Failed to save hold to database:" << q.lastError().text();
    } else {
        qDebug() << "Hold saved to database successfully for" << patronUsername << "on item" << itemId;
    }
}

void DataStore::deleteHoldFromDatabase(const QString& patronUsername, int itemId)
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery q;
    
    q.prepare("DELETE FROM HOLDS WHERE username = :username AND item_id = :item_id");
    q.bindValue(":username", patronUsername);
    q.bindValue(":item_id", itemId);
    
    if (!q.exec()) {
        qDebug() << "Failed to delete hold from database:" << q.lastError().text();
    } else {
        qDebug() << "Hold deleted from database successfully for" << patronUsername << "on item" << itemId;
    }
}


void DataStore::updateCatalogueItemAvailability(int itemId, int availableCopies)
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery q;
    
    q.prepare("UPDATE CATALOGUE SET total_copies = :total_copies WHERE item_id = :item_id");
    q.bindValue(":total_copies", availableCopies);
    q.bindValue(":item_id", itemId);
    
    if (!q.exec()) {
        qDebug() << "Failed to update catalogue item availability:" << q.lastError().text();
    } else {
        qDebug() << "Catalogue item availability updated successfully for item" << itemId << "available copies:" << availableCopies;
    }
}
