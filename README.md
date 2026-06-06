# HinLIBS — Library Management System

A desktop library management application built with **C++**, **Qt Widgets**, and **SQLite**. HinLIBS delivers a clean, role-based interface where patrons can manage their borrowing activity and librarians can oversee the full catalogue and user base — all backed by persistent local storage.

---

## 🚀 Key Features

### 👤 Patron Portal
- **Smart Browsing** — Search and filter the library catalogue in real-time
- **Loan Management** — Borrow up to 3 items with automated due-date tracking
- **Hold System** — Place FIFO-based holds on checked-out items
- **Personal Dashboard** — View active loans, holds, and account status at a glance

### 🔑 Librarian & Admin Tools
- **Catalogue Control** — Add, update, or remove items (Books, Magazines, Movies, Games)
- **Patron Oversight** — Search patrons by name and process returns on their behalf
- **System Maintenance** — Integrated system restart and data persistence verification

---

## 🛠 Tech Stack

| Component | Technology |
|---|---|
| Language | C++17 |
| Framework | Qt 5 (Widgets, SQL, Core) |
| Database | SQLite 3 |
| Build System | QMake |

---

## 📂 Project Architecture

- **DataStore** — Centralized manager for in-memory caching and database synchronization
- **DatabaseManager** — Handles low-level SQLite connections and schema initialization
- **Domain Models** — Specialized classes for `Patron`, `Librarian`, and `CatalogueItem`

---

## ⚙️ Getting Started

### Prerequisites
- Qt Creator (Qt 5.15+ recommended)
- GCC/G++ Compiler

### Installation

1. Clone the repository:
```bash
   git clone https://github.com/solimanelkhoulii/Library-System.git
```
2. Open `Deliverable1.pro` in Qt Creator
3. Select your build kit and press `Ctrl+R` to build and run

The app launches immediately using the pre-populated `hinlibs.db` database — no setup required.

---

## 🔐 Default Accounts

| Username | Role |
|---|---|
| `Patron1` – `Patron5` | Patron |
| `Librarian` | Librarian |
| `Admin` | System Administrator |

No password required — just enter the username.

---

## 🐛 Bug Fix

A memory corruption crash (`malloc_consolidate(): unaligned fastbin chunk detected`) occurring after catalogue modifications was identified and fixed.

**Root cause:** `DataStore::clear()` called `qDeleteAll(m_items)` without following up with `m_items.clear()`, leaving dangling pointers in the map. On the next repopulation, `qDeleteAll` attempted to free already-freed memory.

**Fix:**
```cpp
void DataStore::clear() {
    qDeleteAll(m_items);
    m_items.clear(); // prevent double-free on re-population
}
```

---

## 👤 Author
**Soliman Elkhouli**