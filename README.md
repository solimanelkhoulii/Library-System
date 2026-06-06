# HinLIBS — Library Management System

A desktop library management application built with **C++**, **Qt Widgets**, and **SQLite**. Supports patron self-service and librarian administration through a role-based GUI.

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

## Tech Stack
- Language: C++17
- GUI: Qt Widgets (Qt 5)
- Build System: QMake
- Database: SQLite via Qt SQL module

## Getting Started

1. Open Qt Creator
2. File -> Open File or Project -> select Deliverable1.pro
3. Build and run (Ctrl+B, Ctrl+R)

The app launches immediately using the pre-populated hinlibs.db database.

## Default Accounts

No password required — just enter the username.

| Username | Role |
|---|---|
| Patron1 to Patron5 | Patron |
| Librarian | Librarian |
| Admin | System Administrator |
