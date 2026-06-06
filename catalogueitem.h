#ifndef CATALOGUEITEM_H
#define CATALOGUEITEM_H

#include <QString>
#include <QDate>

class CatalogueItem
{
public:
    enum ItemType { FictionBook, NonFictionBook, Magazine, Movie, VideoGame };

    CatalogueItem(int id, ItemType type, const QString& title, const QString& creator, const QString& format, int copies = 1);

    int id() const { return m_id; }
    ItemType type() const { return m_type; }
    QString title() const { return m_title; }
    QString creator() const { return m_creator; }
    QString format() const { return m_format; }
    int totalCopies() const { return m_totalCopies; }
    int availableCopies() const { return m_availableCopies; }
    QString status() const;
    QString typeString() const;

    // Specific fields for item types
    QString deweyDecimal() const { return m_deweyDecimal; }
    int issueNumber() const { return m_issueNumber; }
    QDate publicationDate() const { return m_publicationDate; }
    QString genre() const { return m_genre; }
    QString rating() const { return m_rating; }
    QString details() const;

    // Mutators
    void setDeweyDecimal(const QString& dewey) { m_deweyDecimal = dewey; }
    void setAvailableCopies(int copies) { m_availableCopies = copies; }
    void setIssueNumber(int issue) { m_issueNumber = issue; }
    void setPublicationDate(const QDate& date) { m_publicationDate = date; }
    void setGenre(const QString& genre) { m_genre = genre; }
    void setRating(const QString& rating) { m_rating = rating; }

    // Availability and Holds
    bool isAvailable() const { return m_availableCopies > 0; }
    bool borrow();
    void returnItem();
    void placeHold(const QString& patronUsername);
    void cancelHold(const QString& patronUsername);
    QList<QString> holdQueue() const { return m_holdQueue; }
    int holdQueuePosition(const QString& patronUsername) const;

private:
    int m_id;
    ItemType m_type;
    QString m_title;
    QString m_creator;
    QString m_format;
    int m_totalCopies;
    int m_availableCopies;

    // Specific fields
    QString m_deweyDecimal;
    int m_issueNumber;
    QDate m_publicationDate;
    QString m_genre;
    QString m_rating;

    QList<QString> m_holdQueue;
};

#endif // CATALOGUEITEM_H
