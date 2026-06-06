#ifndef USER_H
#define USER_H

#include <QString>

class User
{
public:
    enum UserType { Patron, Librarian, Admin };


    User(const QString& username, UserType type)
        : m_username(username), m_type(type) {}

    virtual ~User();

    QString username() const { return m_username; }
    UserType type() const { return m_type; }
    QString typeString() const;

private:
    QString m_username;
    UserType m_type;
};

#endif // USER_H
