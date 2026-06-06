#include "user.h"

QString User::typeString() const
{
    switch (m_type) {
    case Patron: return "Patron";
    case Librarian: return "Librarian";
    case Admin: return "Admin";
    default: return "Unknown";
    }
}

User::~User() {}

