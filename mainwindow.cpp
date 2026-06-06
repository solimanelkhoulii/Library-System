#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pagehome.h"
#include "user.h"
#include "datastore.h"
#include "patron.h"
#include "catalogueitem.h"
#include "login.h"
#include "viewaccount.h"
#include "browsecatalogue.h"
#include "adminlogin.h"
#include "librarianlogin.h"
#include "librarianpatronmanager.h"
#include "librariancataloguemanager.h"
#include <QDebug>
#include <QMetaMethod>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("hinlibs.db");
    if (!db.open()) {
        qDebug() << "Error opening database";
    } else {
        qDebug() << "Connected to hinlibs.db";
    }

    ui->setupUi(this);
    setWindowTitle("HinLIBS");


    //different pages
    pagehome *pageHome = new pagehome(this);
    login *pageLogin = new login(this);
    ViewAccount *viewAccount = new ViewAccount(this);
    BrowseCatalogue *browseCatalogue = new BrowseCatalogue(this);
    AdminLogin *adminLogin = new AdminLogin(this);
    LibrarianLogin *librarianLogin = new LibrarianLogin(this);
    LibrarianPatronManager *patronManager = new LibrarianPatronManager(this);
    LibrarianCatalogueManager *catalogueManager = new LibrarianCatalogueManager(this);


    //add pages to the stacked widget
    ui->stackedWidget->addWidget(pageHome);
    ui->stackedWidget->addWidget(pageLogin);
    ui->stackedWidget->addWidget(viewAccount);
    ui->stackedWidget->addWidget(browseCatalogue);
    ui->stackedWidget->addWidget(adminLogin);
    ui->stackedWidget->addWidget(librarianLogin);
    ui->stackedWidget->addWidget(patronManager);
    ui->stackedWidget->addWidget(catalogueManager);


    // show initial home page
    ui->stackedWidget->setCurrentWidget(pageHome);

    //navigation
    connect(pageHome, &pagehome::loginSuccess, this, [=](User::UserType type) {
        //update the account details on successful login only for Patrons
        if (type == User::Patron) {
            viewAccount->populateAccountStatus();
        }
        switch (type) {
        case User::Patron:
            ui->stackedWidget->setCurrentWidget(pageLogin); // Assuming pageLogin is the Patron's main menu (yes)
            break;
        case User::Librarian:
            ui->stackedWidget->setCurrentWidget(librarianLogin);
            break;
        case User::Admin:
            ui->stackedWidget->setCurrentWidget(adminLogin);
            break;
        default:
            // Should not happen
            break;
        }
    });

    connect(pageLogin, &login::viewAccount, this, [=]() {
       ui->stackedWidget->setCurrentWidget(viewAccount);
    });

    // Navigation back from ViewAccount to the Patron's main menu (pageLogin)
    connect(viewAccount, &ViewAccount::backToLogin, this, [=]() {
       ui->stackedWidget->setCurrentWidget(pageLogin);
    });


    connect(pageLogin, &login::browseCatalogue, this, [=]() {
       ui->stackedWidget->setCurrentWidget(browseCatalogue);
    });

    // Navigation back from ViewAccount to the Patron's main menu (pageLogin)
    connect(viewAccount, &ViewAccount::backToLogin, this, [=]() {
       ui->stackedWidget->setCurrentWidget(pageLogin);
    });

    // Patron action from View Account Status
    connect(viewAccount, &ViewAccount::cancelHold, this, [=](int itemId) {
        Patron* patron = DataStore::instance().currentPatron();
        if (patron) {
            patron->cancelHold(itemId);
            // No need for success message, the view will refresh
        }
    });

    connect(viewAccount, &ViewAccount::returnItem, this, [=](int itemId) {
        Patron* patron = DataStore::instance().currentPatron();
        CatalogueItem* item = DataStore::instance().getItem(itemId);
        if (patron && item) {
            if (patron->returnItem(itemId)) {
                QMessageBox::information(this, "Return Success", "Item successfully returned.");
                viewAccount->populateAccountStatus(); // Refresh the view
                browseCatalogue->populateCatalogue(); // Signal to mainwindow to refresh catalogue

                // D1 Feedback Fix: Check for hold availability notification
                if (!item->holdQueue().isEmpty() && item->isAvailable()) {
                    QString nextPatron = item->holdQueue().first();
                    QMessageBox::information(this, "Hold Available",
                                             QString("The item '%1' is now available for the next patron in the queue: %2.")
                                             .arg(item->title())
                                             .arg(nextPatron));
                }

            } else {
                QMessageBox::warning(this, "Return Failed", "Return failed: Item not found in your active loans.");
            }
        }
    });



    // Navigation back from BrowseCatalogue to the Patron's main menu (pageLogin)
    connect(browseCatalogue, &BrowseCatalogue::backToLogin, this, [=]() {
       ui->stackedWidget->setCurrentWidget(pageLogin);
    });

    // Patron actions from Browse Catalogue
    connect(browseCatalogue, &BrowseCatalogue::borrowItemRequest, this, [=](int itemId) {
        Patron* patron = DataStore::instance().currentPatron();
        CatalogueItem* item = DataStore::instance().getItem(itemId);
        if (patron && item) {
            if (patron->borrowItem(item)) {
                QMessageBox::information(this, "Borrow Success", QString("Successfully borrowed: %1").arg(item->title()));
                browseCatalogue->populateCatalogue();
                viewAccount->populateAccountStatus();
            } else {
                QString errorMsg;
                if (!patron->canBorrow()) {
                    errorMsg = "Borrowing failed: You have reached the maximum limit of 3 active loans.";
                } else if (!item->isAvailable()) {
                    errorMsg = "Borrowing failed: The item is currently checked out.";
                } else {
                    errorMsg = "Borrowing failed: Unknown error.";
                }
                QMessageBox::warning(this, "Borrow Failed", errorMsg);
            }
        }
    });

    connect(browseCatalogue, &BrowseCatalogue::returnItemRequest, this, [=](int itemId) {
        Patron* patron = DataStore::instance().currentPatron();
        if (patron) {
            if (patron->returnItem(itemId)) {
                QMessageBox::information(this, "Return Success", "Item successfully returned.");
                browseCatalogue->populateCatalogue();
                viewAccount->populateAccountStatus();
            } else {
                QMessageBox::warning(this, "Return Failed", "Return failed: Item not found in your active loans.");
            }
        }
    });

    connect(browseCatalogue, &BrowseCatalogue::placeHoldRequest, this, [=](int itemId) {
        Patron* patron = DataStore::instance().currentPatron();
        CatalogueItem* item = DataStore::instance().getItem(itemId);
        if (patron && item) {
            if (patron->placeHold(item)) {
                QMessageBox::information(this, "Hold Placed", QString("Hold successfully placed on: %1. Your position is %2.").arg(item->title()).arg(item->holdQueuePosition(patron->username())));
                browseCatalogue->populateCatalogue();
                viewAccount->populateAccountStatus();
            } else {
                QString errorMsg;
                if (item->isAvailable()) {
                    errorMsg = "Hold failed: You can only place a hold on items that are currently checked out.";
                } else if (patron->activeHolds().contains(itemId)) {
                    errorMsg = "Hold failed: You already have an active hold on this item.";
                } else {
                    errorMsg = "Hold failed: Unknown error.";
                }
                QMessageBox::warning(this, "Hold Failed", errorMsg);
            }
        }
    });

    //update the patron's account status when borrowing an item
    connect(browseCatalogue, &BrowseCatalogue::itemBorrowed, this, [=]() {
        viewAccount->updateAccountStatus();
    });

    connect(browseCatalogue, &BrowseCatalogue::backToMenu, this, [=]() {
        ui->stackedWidget->setCurrentWidget(pageLogin);  // or setCurrentWidget(loginWidget);
    });

    // Logout connections - return to home page and clear current user
    connect(pageLogin, &login::logout, this, [=]() {
        DataStore::instance().setCurrentUser(nullptr);
        ui->stackedWidget->setCurrentWidget(pageHome);
    });

    connect(adminLogin, &AdminLogin::logout, this, [=]() {
        DataStore::instance().setCurrentUser(nullptr);
        ui->stackedWidget->setCurrentWidget(pageHome);
    });

    connect(librarianLogin, &LibrarianLogin::logout, this, [=]() {
        DataStore::instance().setCurrentUser(nullptr);
        ui->stackedWidget->setCurrentWidget(pageHome);
    });

    // Librarian System Restart
    connect(librarianLogin, &LibrarianLogin::restartSystem, this, [=]() {
        DataStore::instance().resetData();
        QMessageBox::information(this, "System Restart", "System data has been reset. All items are available and all patron loans/holds have been cleared.");
        ui->stackedWidget->setCurrentWidget(pageHome);
    });

    // Librarian Navigation
    connect(librarianLogin, &LibrarianLogin::viewCatalogue, this, [=]() {
        catalogueManager->populateCatalogue();
        ui->stackedWidget->setCurrentWidget(catalogueManager);
    });

    connect(librarianLogin, &LibrarianLogin::managePatrons, this, [=]() {
        patronManager->populatePatronData();
        ui->stackedWidget->setCurrentWidget(patronManager);
    });

    // Navigation back to Librarian Menu
    connect(patronManager, &LibrarianPatronManager::backToLibrarianMenu, this, [=]() {
        ui->stackedWidget->setCurrentWidget(librarianLogin);
    });

    connect(catalogueManager, &LibrarianCatalogueManager::backToLibrarianMenu, this, [=]() {
        ui->stackedWidget->setCurrentWidget(librarianLogin);
    });

    //===== testing registered signals in browse catalogue ===
    // qDebug() << "Meta-object signals for BrowseCatalogue:";
    //const QMetaObject* meta = browseCatalogue->metaObject();
    //for (int i = 0; i < meta->methodCount(); ++i) {
    //    QMetaMethod method = meta->method(i);
    //    if (method.methodType() == QMetaMethod::Signal)
    //        qDebug() << "Signal:" << method.methodSignature();
    //}



}

MainWindow::~MainWindow()
{
    delete ui;
}

