# HinLIBS — Library Management System

A desktop library management application built with **C++**, **Qt Widgets**, and **SQLite**. Supports patron self-service and librarian administration through a role-based GUI.

Built as part of COMP 3004 (Object-Oriented Software Engineering) at Carleton University.

---

## Features

### Patron
- Search and browse the catalogue
- Borrow items (max 3 active loans, 14-day loan period)
- Place and cancel holds (FIFO queue)
- View account details — active loans, holds, due dates

### Librarian
- Add and remove items from the catalogue
- Process returns on behalf of any patron
- Search patrons by name and view their active loans

### System
- Full SQLite persistence — all data survives restarts
- Role-based login (Patron / Librarian / Admin)
- Pre-populated database with 20 catalogue items and 7 user accounts

---

## Tech Stack

| Layer | Technology |
|---|---|
| Language | C++17 |
| GUI | Qt Widgets (Qt 5) |
| Build System | QMake (`.pro` file) |
| Database | SQLite via Qt SQL module |
| Platform | Linux Ubuntu (tested on course VM) |

---

## Getting Started

### Prerequisites
- Qt Creator with Qt 5.x and GCC
- SQLite (bundled with Qt — no separate install needed)

### Build & Run

1. Clone the repository:
   ```bash
   git clone https://github.com/<your-username>/hinlibs.git
   cd hinlibs
   ```

2. Open the project in Qt Creator:
   - `File → Open File or Project...`
   - Select `Deliverable1.pro`

3. Build and run:
   - Select your Desktop Qt kit
   - Press `Ctrl+B` to build, then `Ctrl+R` to run

The app launches immediately using the pre-populated `hinlibs.db` database in the project root.

---

## Default Accounts

No password required — just enter the username and click the appropriate login button.

| Username | Role |
|---|---|
| `Patron1` – `Patron5` | Patron |
| `Librarian` | Librarian |
| `Admin` | System Administrator |

---

## Project Structure

```
hinlibs/
├── main.cpp                         # Entry point
├── mainwindow.cpp / .h / .ui        # Main window and navigation
├── login.cpp / .h / .ui             # Login screen
├── pagehome.cpp / .h / .ui          # Patron home page
├── browsecatalogue.cpp / .h / .ui   # Catalogue browse/search
├── viewaccount.cpp / .h / .ui       # Patron account view
├── librarianlogin.cpp / .h / .ui    # Librarian login
├── librariancataloguemanager.cpp    # Add/remove catalogue items
├── librarianpatronmanager.cpp       # Return items for patrons
├── adminlogin.cpp / .h / .ui        # Admin login
├── datastore.cpp / .h               # Singleton in-memory store + DB sync
├── databasemanager.cpp / .h         # Database connection setup
├── catalogueitem.cpp / .h           # Domain model: catalogue items
├── patron.cpp / .h                  # Domain model: patron (loans, holds)
├── user.cpp / .h                    # Domain model: base user class
├── sql_create_db.sql                # Schema creation script
├── sql_populate_db.sql              # Seed data script
├── hinlibs.db                       # Pre-populated SQLite database
└── Deliverable1.pro                 # QMake project file
```

---

## Database Schema

The SQLite database (`hinlibs.db`) contains the following tables:

- **CATALOGUE** — All library items (books, magazines, movies, video games)
- **USERS** — Patron, librarian, and admin accounts
- **LOANS** — Active borrowing records (patron, item, borrow date, due date)
- **HOLDS** — Hold queue records (patron, item, queue position)

See `sql_create_db.sql` for the full schema and `sql_populate_db.sql` for seed data.

---

## Bug Fix (Post-Submission)

A memory corruption crash (`malloc_consolidate(): unaligned fastbin chunk detected`) that occurred after adding/removing catalogue items was fixed after grading.

**Root cause:** `DataStore::clear()` called `qDeleteAll(m_items)` to free all heap-allocated `CatalogueItem` pointers, but did not call `m_items.clear()` afterward. The map retained dangling pointers. On the next call to `populateCatalogue()` → `clear()`, `qDeleteAll` attempted to free already-freed memory, causing undefined behaviour and a heap corruption crash.

**Fix:** One line added to `datastore.cpp`:
```cpp
void DataStore::clear() {
    qDeleteAll(m_items);
    m_items.clear(); // prevent double-free on re-population
}
```

---

## Team

| Name | Student Number |
|---|---|
| Soliman Elkhouli | 101244211 |
| Jules Carson | 101019276 |
| Sean Xie | 101272248 |
| Joshua Wang | 101307312 |
