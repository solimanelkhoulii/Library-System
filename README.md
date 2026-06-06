# HinLIBS - Library Information System 

## Overview

The core focus of this deliverable is the implementation of **persistent data storage using SQLite** and the addition of **Librarian-specific functionality**. The system is a C++ application with a Graphical User Interface (GUI) built using the **Qt framework**, designed to run on the course's Linux Ubuntu VM environment.

## Team Members

| Name | Student Number |
| :--- | :--- |
| Soliman Elkhouli | 101244211 |
| Jules Carson | 101019276 |
| Sean Xie | 101272248 |
| Joshua Wang | 101307312 |

## Key Features

### 1. Data Persistence 
All application data (Catalogue, Users, Loans, Holds) is now managed by an **SQLite database**. The system ensures data integrity by loading all data from the database on startup and persisting changes throughout the application's runtime.

### 2. Librarian Functionality 
The Librarian role provides administrative tools for managing the library's resources and patron accounts.
*   **Add/Remove Item:** Manage the library catalogue by adding or removing items.
*   **Process Returns:** Return items on behalf of any patron, including searching for a patron and viewing their active loans.

### 3. Patron Functionality 
The core patron features remain fully functional and are now backed by the SQLite database.
*   **Catalogue:** Search and browse available items.
*   **Borrowing:** Borrow items (maximum of 3 active loans).
*   **Holds:** Place and cancel holds on checked-out items (FIFO order).
*   **Account:** View personal account details, including active loans and holds.

## Technology Stack

| Component | Technology | Details |
| :--- | :--- | :--- |
| **Language** | C++ | Core application logic. |
| **GUI** | Qt Widgets | Graphical User Interface framework. |
| **Build System** | QMake | Project compilation and configuration (`Deliverable1.pro`). |
| **Database** | SQLite | Persistent data storage via the Qt SQL module. |

## Setup and Build Instructions

The project is configured as a fully **turnkey submission** for the course VM environment.

1.  **Extract the Archive:**
    ```bash
    unzip team_49_D2.zip
    cd d2v22
    ```
2.  **Open in Qt Creator:**
    *   Launch Qt Creator.
    *   Go to `File` -> `Open File or Project...`
    *   Select the project file: `Deliverable1.pro`.
3.  **Build and Run:**
    *   Select the appropriate build kit (e.g., Desktop Qt 5.x.x GCC 64bit).
    *   Click the **Build** button (`Ctrl+B`) and then the **Run** button (`Ctrl+R`).

The application will compile and launch immediately without requiring any manual configuration, library installation, or path changes.

## Default User Accounts and Database

The application uses a pre-populated SQLite database (`hinlibs.db`) located in the project root.

| User Type | Username | Role | Login Method |
| :--- | :--- | :--- | :--- |
| **Patron** | `Patron1` to `Patron5` | Standard library user. | Enter username, click **Patron Login**. |
| **Librarian** | `Librarian` | Catalogue and loan management. | Enter username, click **Librarian Login**. |
| **Administrator** | `Administrator` | System-level access. | Enter username, click **Admin Login**. |

**Note:** No password is required for login; simply enter the username.

### Database File Location

The pre-populated database file is:
```
/home/ubuntu/d2v22_project/d2v22/hinlibs.db
```

## Project Structure

| File/Directory | Description |
| :--- | :--- |
| `Deliverable1.pro` | QMake project file. |
| `main.cpp` | Application entry point. |
| `mainwindow.cpp` | Main window logic, handles page navigation and database connection. |
| `databasemanager.cpp` | Handles all database operations. |
| `sql_create_db.sql` | SQL script for table creation. |
| `sql_populate_db.sql` | SQL script for populating default data (20 items, 7 users). |
| `hinlibs.db` | The pre-populated SQLite database file. |
| `*.cpp`/`.h`/`.ui` | Source files for UI pages and domain objects. |
