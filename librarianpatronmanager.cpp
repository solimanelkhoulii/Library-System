#include "librarianpatronmanager.h"
#include "ui_librarianpatronmanager.h"
#include <QMessageBox>
#include <QHeaderView>

LibrarianPatronManager::LibrarianPatronManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LibrarianPatronManager)
{
    ui->setupUi(this);
    connect(ui->backButton, &QPushButton::clicked, this, &LibrarianPatronManager::backToLibrarianMenu);
    
    // Make tables read-only
    ui->loansTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->holdsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->patronListTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    // Enable full row selection
    ui->loansTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->holdsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->patronListTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    // Hide row numbers
    ui->loansTable->verticalHeader()->setVisible(false);
    ui->holdsTable->verticalHeader()->setVisible(false);
    ui->patronListTable->verticalHeader()->setVisible(false);
    
    // Resize columns to fit content
    ui->loansTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->holdsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->patronListTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    // Set up Patron List Table
    ui->patronListTable->setColumnCount(1);
    ui->patronListTable->setHorizontalHeaderLabels(QStringList() << "Patron Username");
    
    // Disable action buttons initially
    ui->returnBookButton->setEnabled(false);
    ui->cancelHoldButton->setEnabled(false);
}

LibrarianPatronManager::~LibrarianPatronManager()
{
    delete ui;
}

void LibrarianPatronManager::populatePatronData()
{
    QList<Patron*> patrons = DataStore::instance().allPatrons();
    ui->patronListTable->setRowCount(patrons.size());
    
    for (int i = 0; i < patrons.size(); ++i) {
        QTableWidgetItem *item = new QTableWidgetItem(patrons.at(i)->username());
        ui->patronListTable->setItem(i, 0, item);
    }
    
    // Clear loans and holds tables
    ui->loansTable->setRowCount(0);
    ui->holdsTable->setRowCount(0);
    ui->patronDetailsLabel->setText("Select a patron to view their loans and holds.");
    
    // Disable action buttons
    ui->returnBookButton->setEnabled(false);
    ui->cancelHoldButton->setEnabled(false);
}

void LibrarianPatronManager::on_patronListTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentColumn);
    Q_UNUSED(previousRow);
    Q_UNUSED(previousColumn);
    
    if (currentRow < 0) {
        ui->loansTable->setRowCount(0);
        ui->holdsTable->setRowCount(0);
        ui->patronDetailsLabel->setText("Select a patron to view their loans and holds.");
        ui->returnBookButton->setEnabled(false);
        ui->cancelHoldButton->setEnabled(false);
        return;
    }
    
    QString username = ui->patronListTable->item(currentRow, 0)->text();
    Patron* patron = dynamic_cast<Patron*>(DataStore::instance().authenticateUser(username));
    
    if (patron) {
        ui->patronDetailsLabel->setText(QString("Account Status for: %1").arg(patron->username()));
        populateLoansTable(patron);
        populateHoldsTable(patron);
    }
}

void LibrarianPatronManager::populateLoansTable(Patron* patron)
{
    QList<Loan> loans = patron->activeLoans();
    ui->loansTable->setColumnCount(4);
    ui->loansTable->setHorizontalHeaderLabels(QStringList() << "Item ID" << "Title" << "Due Date" << "Days Left");
    ui->loansTable->setRowCount(loans.size());
    
    for (int i = 0; i < loans.size(); ++i) {
        Loan loan = loans.at(i);
        CatalogueItem* item = DataStore::instance().getItem(loan.itemId());
        
        ui->loansTable->setItem(i, 0, new QTableWidgetItem(QString::number(loan.itemId())));
        ui->loansTable->setItem(i, 1, new QTableWidgetItem(item ? item->title() : "Unknown Item"));
        ui->loansTable->setItem(i, 2, new QTableWidgetItem(loan.dueDate().toString("yyyy-MM-dd")));
        ui->loansTable->setItem(i, 3, new QTableWidgetItem(QString::number(loan.daysRemaining())));
    }
    
    ui->returnBookButton->setEnabled(loans.size() > 0);
}

void LibrarianPatronManager::populateHoldsTable(Patron* patron)
{
    QList<int> holds = patron->activeHolds();
    ui->holdsTable->setColumnCount(3);
    ui->holdsTable->setHorizontalHeaderLabels(QStringList() << "Item ID" << "Title" << "Queue Position");
    ui->holdsTable->setRowCount(holds.size());
    
    for (int i = 0; i < holds.size(); ++i) {
        int itemId = holds.at(i);
        CatalogueItem* item = DataStore::instance().getItem(itemId);
        
        ui->holdsTable->setItem(i, 0, new QTableWidgetItem(QString::number(itemId)));
        ui->holdsTable->setItem(i, 1, new QTableWidgetItem(item ? item->title() : "Unknown Item"));
        ui->holdsTable->setItem(i, 2, new QTableWidgetItem(QString::number(item ? item->holdQueuePosition(patron->username()) : 0)));
    }
    
    ui->cancelHoldButton->setEnabled(holds.size() > 0);
}

void LibrarianPatronManager::on_returnBookButton_clicked()
{
    int currentRow = ui->loansTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Action Failed", "Please select a book to return.");
        return;
    }
    
    QString username = ui->patronListTable->currentItem()->text();
    Patron* patron = dynamic_cast<Patron*>(DataStore::instance().authenticateUser(username));
    
    if (patron) {
        int itemId = ui->loansTable->item(currentRow, 0)->text().toInt();
        
        if (patron->returnItem(itemId)) {
            QMessageBox::information(this, "Success", QString("Item ID %1 returned successfully for %2.").arg(itemId).arg(username));
            populateLoansTable(patron); // Refresh loans table
            
            // CRITICAL FIX: Update the catalogue availability in the database
            CatalogueItem* item = DataStore::instance().getItem(itemId);
            if (item) {
                DataStore::instance().updateCatalogueItemAvailability(itemId, item->availableCopies());
            }
        } else {
            QMessageBox::critical(this, "Error", "Failed to return item. Item not found in patron's loans.");
        }
    }
}

void LibrarianPatronManager::on_cancelHoldButton_clicked()
{
    int currentRow = ui->holdsTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Action Failed", "Please select a hold to cancel.");
        return;
    }
    
    QString username = ui->patronListTable->currentItem()->text();
    Patron* patron = dynamic_cast<Patron*>(DataStore::instance().authenticateUser(username));
    
    if (patron) {
        int itemId = ui->holdsTable->item(currentRow, 0)->text().toInt();
        
        if (patron->cancelHold(itemId)) {
            QMessageBox::information(this, "Success", QString("Hold on Item ID %1 cancelled successfully for %2.").arg(itemId).arg(username));
            populateHoldsTable(patron); // Refresh holds table
        } else {
            QMessageBox::critical(this, "Error", "Failed to cancel hold. Hold not found for patron.");
        }
    }
}
