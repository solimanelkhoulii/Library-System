#include "databasemanager.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>
#include <QDate>

/**
 * @brief DatabaseManager constructor.
 */
DatabaseManager::DatabaseManager() : m_initialized(false)
{
}

/**
 * @brief DatabaseManager destructor. Closes the database connection.
 */
DatabaseManager::~DatabaseManager()
{
    closeDatabase();
}

/**
 * @brief Provides the singleton instance of the DatabaseManager.
 * @return Reference to the singleton instance.
 */
DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

/**
 * @brief Initializes and opens the SQLite database connection.
 * @param dbPath The file path to the SQLite database.
 * @return True if initialization is successful, false otherwise.
 */
bool DatabaseManager::initializeDatabase(const QString& dbPath)
{
    // D2 Requirement: Use SQLite for data persistence
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);
    
    if (!m_db.open()) {
        qDebug() << "Failed to open database:" << m_db.lastError().text();
        return false;
    }
    
    // Ensure all required tables exist
    if (!createTables()) {
        qDebug() << "Failed to create tables";
        return false;
    }
    
    m_initialized = true;
    return true;
}

/**
 * @brief Creates the necessary database tables if they do not already exist.
 * @return True if all tables are created successfully, false otherwise.
 */
bool DatabaseManager::createTables()
{
    QSqlQuery query;
    
    // Create Catalogue table (stores all library items)
    if (!query.exec("CREATE TABLE IF NOT EXISTS Catalogue ("
                    "id INTEGER PRIMARY KEY,"
                    "type INTEGER NOT NULL,"
                    "title TEXT NOT NULL,"
                    "creator TEXT NOT NULL,"
                    "format TEXT NOT NULL,"
                    "totalCopies INTEGER NOT NULL,"
                    "availableCopies INTEGER NOT NULL,"
                    "deweyDecimal TEXT,"
                    "issueNumber INTEGER,"
                    "publicationDate TEXT,"
                    "genre TEXT,"
                    "rating TEXT"
                    ")")) {
        qDebug() << "Failed to create Catalogue table:" << query.lastError().text();
        return false;
    }
    
    // Create Users table (stores all patrons, librarians, and admins)
    if (!query.exec("CREATE TABLE IF NOT EXISTS Users ("
                    "username TEXT PRIMARY KEY,"
                    "userType INTEGER NOT NULL"
                    ")")) {
        qDebug() << "Failed to create Users table:" << query.lastError().text();
        return false;
    }
    
    // Create Loans table (stores active borrowing records)
    if (!query.exec("CREATE TABLE IF NOT EXISTS Loans ("
                    "patronUsername TEXT NOT NULL,"
                    "itemId INTEGER NOT NULL,"
                    "dueDate TEXT NOT NULL,"
                    "PRIMARY KEY (patronUsername, itemId),"
                    "FOREIGN KEY (patronUsername) REFERENCES Users(username),"
                    "FOREIGN KEY (itemId) REFERENCES Catalogue(id)"
                    ")")) {
        qDebug() << "Failed to create Loans table:" << query.lastError().text();
        return false;
    }
    
    // Create Holds table (stores hold queue records)
    if (!query.exec("CREATE TABLE IF NOT EXISTS Holds ("
                    "patronUsername TEXT NOT NULL,"
                    "itemId INTEGER NOT NULL,"
                    "position INTEGER NOT NULL,"
                    "PRIMARY KEY (patronUsername, itemId),"
                    "FOREIGN KEY (patronUsername) REFERENCES Users(username),"
                    "FOREIGN KEY (itemId) REFERENCES Catalogue(id)"
                    ")")) {
        qDebug() << "Failed to create Holds table:" << query.lastError().text();
        return false;
    }
    
    return true;
}

/**
 * @brief Populates the database with the default catalogue items and users as required by D1/D2.
 * @return True if population is successful, false otherwise.
 */
bool DatabaseManager::populateDefaultData()
{
    QSqlQuery query;
    
    // Clear existing data to ensure a clean slate for default population
    query.exec("DELETE FROM Holds");
    query.exec("DELETE FROM Loans");
    query.exec("DELETE FROM Catalogue");
    query.exec("DELETE FROM Users");
    
    // Insert users (7 unique users: 5 patrons, 1 librarian, 1 system administrator)
    query.prepare("INSERT INTO Users (username, userType) VALUES (?, ?)");
    
    // Patrons (type 0)
    for (int i = 1; i <= 5; ++i) {
        query.addBindValue(QString("Patron%1").arg(i));
        query.addBindValue(0);
        if (!query.exec()) {
            qDebug() << "Failed to insert patron:" << query.lastError().text();
            return false;
        }
    }
    
    // Librarian (type 1)
    query.addBindValue("Librarian");
    query.addBindValue(1);
    if (!query.exec()) {
        qDebug() << "Failed to insert librarian:" << query.lastError().text();
        return false;
    }
    
    // Admin (type 2)
    query.addBindValue("Admin");
    query.addBindValue(2);
    if (!query.exec()) {
        qDebug() << "Failed to insert admin:" << query.lastError().text();
        return false;
    }
    
    // Insert catalogue items (20 unique items)
    query.prepare("INSERT INTO Catalogue (id, type, title, creator, format, totalCopies, availableCopies, "
                  "deweyDecimal, issueNumber, publicationDate, genre, rating) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    
    int id = 1;
    
    // 5 fiction books
    query.addBindValue(id++);
    query.addBindValue(0);
    query.addBindValue("The Great Gatsby");
    query.addBindValue("F. Scott Fitzgerald");
    query.addBindValue("Hardcover");
    query.addBindValue(2);
    query.addBindValue(2);
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    if (!query.exec()) qDebug() << "Failed to insert item:" << query.lastError().text();
    
    query.addBindValue(id++);
    query.addBindValue(0);
    query.addBindValue("1984");
    query.addBindValue("George Orwell");
    query.addBindValue("Paperback");
    query.addBindValue(1);
    query.addBindValue(1);
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    if (!query.exec()) qDebug() << "Failed to insert item:" << query.lastError().text();
    
    query.addBindValue(id++);
    query.addBindValue(0);
    query.addBindValue("Pride and Prejudice");
    query.addBindValue("Jane Austen");
    query.addBindValue("Ebook");
    query.addBindValue(3);
    query.addBindValue(3);
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    if (!query.exec()) qDebug() << "Failed to insert item:" << query.lastError().text();
    
    query.addBindValue(id++);
    query.addBindValue(0);
    query.addBindValue("To Kill a Mockingbird");
    query.addBindValue("Harper Lee");
    query.addBindValue("Hardcover");
    query.addBindValue(1);
    query.addBindValue(1);
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    if (!query.exec()) qDebug() << "Failed to insert item:" << query.lastError().text();
    
    query.addBindValue(id++);
    query.addBindValue(0);
    query.addBindValue("The Catcher in the Rye");
    query.addBindValue("J.D. Salinger");
    query.addBindValue("Paperback");
    query.addBindValue(2);
    query.addBindValue(2);
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    if (!query.exec()) qDebug() << "Failed to insert item:" << query.lastError().text();
    
    // 5 non-fiction books
    query.addBindValue(id++);
    query.addBindValue(1);
    query.addBindValue("Sapiens: A Brief History of Humankind");
    query.addBindValue("Yuval Noah Harari");
    query.addBindValue("Hardcover");
    query.addBindValue(1);
    query.addBindValue(1);
    query.addBindValue("909.82");
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    if (!query.exec()) qDebug() << "Failed to insert item:" << query.lastError().text();
    
    query.addBindValue(id++);
    query.addBindValue(1);
    query.addBindValue("Cosmos");
    query.addBindValue("Carl Sagan");
    query.addBindValue("Paperback");
    query.addBindValue(2);
    query.addBindValue(2);
    query.addBindValue("520");
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    if (!query.exec()) qDebug() << "Failed to insert item:" << query.lastError().text();
    
    query.addBindValue(id++);
    query.addBindValue(1);
    query.addBindValue("The Selfish Gene");
    query.addBindValue("Richard Dawkins");
    query.addBindValue("Hardcover");
    query.addBindValue(1);
    query.addBindValue(1);
    query.addBindValue("576.5");
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    if (!query.exec()) qDebug() << "Failed to insert item:" << query.lastError().text();
    
    query.addBindValue(id++);
    query.addBindValue(1);
    query.addBindValue("A Brief History of Time");
    query.addBindValue("Stephen Hawking");
    query.addBindValue("Ebook");
    query.addBindValue(3);
    query.addBindValue(3);
    query.addBindValue("523.1");
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    if (!query.exec()) qDebug() << "Failed to insert item:" << query.lastError().text();
    
    query.addBindValue(id++);
    query.addBindValue(1);
    query.addBindValue("Quiet: The Power of Introverts");
    query.addBindValue("Susan Cain");
    query.addBindValue("Paperback");
    query.addBindValue(1);
    query.addBindValue(1);
    query.addBindValue("155.232");
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    if (!query.exec()) qDebug() << "Failed to insert item:" << query.lastError().text();
    
    // 3 magazines
    query.addBindValue(id++);
    query.addBindValue(2);
    query.addBindValue("National Geographic");
    query.addBindValue("Various");
    query.addBindValue("Print");
    query.addBindValue(1);
    query.addBindValue(1);
    query.addBindValue(QVariant());
    query.addBindValue(12);
    query.addBindValue("2025-10-01");
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    if (!query.exec()) qDebug() << "Failed to insert item:" << query.lastError().text();
    
    query.addBindValue(id++);
    query.addBindValue(2);
    query.addBindValue("The Economist");
    query.addBindValue("Various");
    query.addBindValue("Print");
    query.addBindValue(2);
    query.addBindValue(2);
    query.addBindValue(QVariant());
    query.addBindValue(45);
    query.addBindValue("2025-11-05");
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    if (!query.exec()) qDebug() << "Failed to insert item:" << query.lastError().text();
    
    query.addBindValue(id++);
    query.addBindValue(2);
    query.addBindValue("Wired");
    query.addBindValue("Various");
    query.addBindValue("Digital");
    query.addBindValue(1);
    query.addBindValue(1);
    query.addBindValue(QVariant());
    query.addBindValue(9);
    query.addBindValue("2025-09-15");
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    if (!query.exec()) qDebug() << "Failed to insert item:" << query.lastError().text();
    
    // 3 movies
    query.addBindValue(id++);
    query.addBindValue(3);
    query.addBindValue("Inception");
    query.addBindValue("Christopher Nolan");
    query.addBindValue("DVD");
    query.addBindValue(1);
    query.addBindValue(1);
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue("Sci-Fi");
    query.addBindValue("PG-13");
    if (!query.exec()) qDebug() << "Failed to insert item:" << query.lastError().text();
    
    query.addBindValue(id++);
    query.addBindValue(3);
    query.addBindValue("Parasite");
    query.addBindValue("Bong Joon-ho");
    query.addBindValue("Blu-ray");
    query.addBindValue(2);
    query.addBindValue(2);
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue("Thriller");
    query.addBindValue("R");
    if (!query.exec()) qDebug() << "Failed to insert item:" << query.lastError().text();
    
    query.addBindValue(id++);
    query.addBindValue(3);
    query.addBindValue("Toy Story");
    query.addBindValue("John Lasseter");
    query.addBindValue("DVD");
    query.addBindValue(1);
    query.addBindValue(1);
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue("Animation");
    query.addBindValue("G");
    if (!query.exec()) qDebug() << "Failed to insert item:" << query.lastError().text();
    
    // 4 video games
    query.addBindValue(id++);
    query.addBindValue(4);
    query.addBindValue("The Legend of Zelda: Breath of the Wild");
    query.addBindValue("Nintendo");
    query.addBindValue("Switch Cartridge");
    query.addBindValue(1);
    query.addBindValue(1);
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue("Action-Adventure");
    query.addBindValue("E10+");
    if (!query.exec()) qDebug() << "Failed to insert item:" << query.lastError().text();
    
    query.addBindValue(id++);
    query.addBindValue(4);
    query.addBindValue("Red Dead Redemption 2");
    query.addBindValue("Rockstar Games");
    query.addBindValue("PS4 Disc");
    query.addBindValue(2);
    query.addBindValue(2);
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue("Action-Adventure");
    query.addBindValue("M");
    if (!query.exec()) qDebug() << "Failed to insert item:" << query.lastError().text();
    
    query.addBindValue(id++);
    query.addBindValue(4);
    query.addBindValue("Stardew Valley");
    query.addBindValue("ConcernedApe");
    query.addBindValue("PC");
    query.addBindValue(1);
    query.addBindValue(1);
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue("Simulation");
    query.addBindValue("E");
    if (!query.exec()) qDebug() << "Failed to insert item:" << query.lastError().text();
    
    query.addBindValue(id++);
    query.addBindValue(4);
    query.addBindValue("Overwatch 2");
    query.addBindValue("Blizzard Entertainment");
    query.addBindValue("PC");
    query.addBindValue(1);
    query.addBindValue(1);
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue(QVariant());
    query.addBindValue("First-Person Shooter");
    query.addBindValue("T");
    if (!query.exec()) qDebug() << "Failed to insert item:" << query.lastError().text();
    
    return true;
}

QList<CatalogueItem*> DatabaseManager::loadAllItems()
{
    QList<CatalogueItem*> items;
    QSqlQuery query("SELECT * FROM Catalogue");
    
    while (query.next()) {
        int id = query.value("id").toInt();
        int type = query.value("type").toInt();
        QString title = query.value("title").toString();
        QString creator = query.value("creator").toString();
        QString format = query.value("format").toString();
        int totalCopies = query.value("totalCopies").toInt();
        
        CatalogueItem* item = new CatalogueItem(id, (CatalogueItem::ItemType)type, title, creator, format, totalCopies);
        
        // Set available copies
        int availableCopies = query.value("availableCopies").toInt();
        for (int i = 0; i < totalCopies - availableCopies; ++i) {
            item->borrow();
        }
        
        // Set specific fields
        if (!query.value("deweyDecimal").isNull()) {
            item->setDeweyDecimal(query.value("deweyDecimal").toString());
        }
        if (!query.value("issueNumber").isNull()) {
            item->setIssueNumber(query.value("issueNumber").toInt());
        }
        if (!query.value("publicationDate").isNull()) {
            item->setPublicationDate(QDate::fromString(query.value("publicationDate").toString(), Qt::ISODate));
        }
        if (!query.value("genre").isNull()) {
            item->setGenre(query.value("genre").toString());
        }
        if (!query.value("rating").isNull()) {
            item->setRating(query.value("rating").toString());
        }
        
        items.append(item);
    }
    
    return items;
}

CatalogueItem* DatabaseManager::loadItem(int id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM Catalogue WHERE id = ?");
    query.addBindValue(id);
    
    if (!query.exec() || !query.next()) {
        return nullptr;
    }
    
    int type = query.value("type").toInt();
    QString title = query.value("title").toString();
    QString creator = query.value("creator").toString();
    QString format = query.value("format").toString();
    int totalCopies = query.value("totalCopies").toInt();
    
    CatalogueItem* item = new CatalogueItem(id, (CatalogueItem::ItemType)type, title, creator, format, totalCopies);
    
    int availableCopies = query.value("availableCopies").toInt();
    for (int i = 0; i < totalCopies - availableCopies; ++i) {
        item->borrow();
    }
    
    if (!query.value("deweyDecimal").isNull()) {
        item->setDeweyDecimal(query.value("deweyDecimal").toString());
    }
    if (!query.value("issueNumber").isNull()) {
        item->setIssueNumber(query.value("issueNumber").toInt());
    }
    if (!query.value("publicationDate").isNull()) {
        item->setPublicationDate(QDate::fromString(query.value("publicationDate").toString(), Qt::ISODate));
    }
    if (!query.value("genre").isNull()) {
        item->setGenre(query.value("genre").toString());
    }
    if (!query.value("rating").isNull()) {
        item->setRating(query.value("rating").toString());
    }
    
    return item;
}

bool DatabaseManager::saveItem(CatalogueItem* item)
{
    if (!item) return false;
    
    QSqlQuery query;
    query.prepare("UPDATE Catalogue SET type = ?, title = ?, creator = ?, format = ?, "
                  "totalCopies = ?, availableCopies = ?, deweyDecimal = ?, issueNumber = ?, "
                  "publicationDate = ?, genre = ?, rating = ? WHERE id = ?");
    
    query.addBindValue((int)item->type());
    query.addBindValue(item->title());
    query.addBindValue(item->creator());
    query.addBindValue(item->format());
    query.addBindValue(item->totalCopies());
    query.addBindValue(item->availableCopies());
    query.addBindValue(item->deweyDecimal().isEmpty() ? QVariant() : item->deweyDecimal());
    query.addBindValue(item->issueNumber() == 0 ? QVariant() : item->issueNumber());
    query.addBindValue(item->publicationDate().isNull() ? QVariant() : item->publicationDate().toString(Qt::ISODate));
    query.addBindValue(item->genre().isEmpty() ? QVariant() : item->genre());
    query.addBindValue(item->rating().isEmpty() ? QVariant() : item->rating());
    query.addBindValue(item->id());
    
    return query.exec();
}

bool DatabaseManager::addItem(CatalogueItem* item)
{
    if (!item) return false;
    
    QSqlQuery query;
    query.prepare("INSERT INTO Catalogue (id, type, title, creator, format, totalCopies, availableCopies, "
                  "deweyDecimal, issueNumber, publicationDate, genre, rating) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    
    query.addBindValue(item->id());
    query.addBindValue((int)item->type());
    query.addBindValue(item->title());
    query.addBindValue(item->creator());
    query.addBindValue(item->format());
    query.addBindValue(item->totalCopies());
    query.addBindValue(item->availableCopies());
    query.addBindValue(item->deweyDecimal().isEmpty() ? QVariant() : item->deweyDecimal());
    query.addBindValue(item->issueNumber() == 0 ? QVariant() : item->issueNumber());
    query.addBindValue(item->publicationDate().isNull() ? QVariant() : item->publicationDate().toString(Qt::ISODate));
    query.addBindValue(item->genre().isEmpty() ? QVariant() : item->genre());
    query.addBindValue(item->rating().isEmpty() ? QVariant() : item->rating());
    
    return query.exec();
}

bool DatabaseManager::removeItem(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Catalogue WHERE id = ?");
    query.addBindValue(id);
    return query.exec();
}

QList<User*> DatabaseManager::loadAllUsers()
{
    QList<User*> users;
    QSqlQuery query("SELECT * FROM Users");
    
    while (query.next()) {
        QString username = query.value("username").toString();
        int userType = query.value("userType").toInt();
        
        User* user;
        if (userType == 0) {
            user = new Patron(username);
        } else {
            user = new User(username, (User::UserType)userType);
        }
        users.append(user);
    }
    
    return users;
}

User* DatabaseManager::loadUser(const QString& username)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM Users WHERE username = ?");
    query.addBindValue(username);
    
    if (!query.exec() || !query.next()) {
        return nullptr;
    }
    
    int userType = query.value("userType").toInt();
    User* user;
    if (userType == 0) {
        user = new Patron(username);
    } else {
        user = new User(username, (User::UserType)userType);
    }
    
    return user;
}

bool DatabaseManager::saveUser(User* user)
{
    if (!user) return false;
    
    QSqlQuery query;
    query.prepare("UPDATE Users SET userType = ? WHERE username = ?");
    query.addBindValue((int)user->type());
    query.addBindValue(user->username());
    
    return query.exec();
}

bool DatabaseManager::saveLoan(const QString& patronUsername, int itemId, const QDate& dueDate)
{
    QSqlQuery query;
    query.prepare("INSERT OR REPLACE INTO Loans (patronUsername, itemId, dueDate) VALUES (?, ?, ?)");
    query.addBindValue(patronUsername);
    query.addBindValue(itemId);
    query.addBindValue(dueDate.toString(Qt::ISODate));
    
    return query.exec();
}

bool DatabaseManager::removeLoan(const QString& patronUsername, int itemId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Loans WHERE patronUsername = ? AND itemId = ?");
    query.addBindValue(patronUsername);
    query.addBindValue(itemId);
    
    return query.exec();
}

QList<Loan> DatabaseManager::loadPatronLoans(const QString& patronUsername)
{
    QList<Loan> loans;
    QSqlQuery query;
    query.prepare("SELECT itemId, dueDate FROM Loans WHERE patronUsername = ?");
    query.addBindValue(patronUsername);
    
    if (!query.exec()) {
        return loans;
    }
    
    while (query.next()) {
        int itemId = query.value("itemId").toInt();
        QDate dueDate = QDate::fromString(query.value("dueDate").toString(), Qt::ISODate);
        loans.append(Loan(itemId, dueDate));
    }
    
    return loans;
}

bool DatabaseManager::saveHold(const QString& patronUsername, int itemId)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Holds (patronUsername, itemId, position) "
                  "SELECT ?, ?, COALESCE(MAX(position), 0) + 1 FROM Holds WHERE itemId = ?");
    query.addBindValue(patronUsername);
    query.addBindValue(itemId);
    query.addBindValue(itemId);
    
    return query.exec();
}

bool DatabaseManager::removeHold(const QString& patronUsername, int itemId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Holds WHERE patronUsername = ? AND itemId = ?");
    query.addBindValue(patronUsername);
    query.addBindValue(itemId);
    
    return query.exec();
}

QList<int> DatabaseManager::loadPatronHolds(const QString& patronUsername)
{
    QList<int> holds;
    QSqlQuery query;
    query.prepare("SELECT itemId FROM Holds WHERE patronUsername = ? ORDER BY position");
    query.addBindValue(patronUsername);
    
    if (!query.exec()) {
        return holds;
    }
    
    while (query.next()) {
        holds.append(query.value("itemId").toInt());
    }
    
    return holds;
}

bool DatabaseManager::closeDatabase()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
    return true;
}
