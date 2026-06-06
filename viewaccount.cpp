#include "viewaccount.h"
#include "ui_viewaccount.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include "datastore.h"
#include "patron.h"
#include "catalogueitem.h"
#include <QMessageBox>
#include <QDebug>

ViewAccount::ViewAccount(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViewAccount)
{
    ui->setupUi(this);
    QString userName = "Patron";
//    QString userNamed = DataStore::instance().getCurrentUser()->username();
//    if (userNamed.size() <= 0) return;
    ui->userStatusLabel->setText(userName + " Account Status");

    populateAccountStatus();

    // Assuming a QPushButton named 'backButton' exists in viewaccount.ui
    connect(ui->backButton, &QPushButton::clicked, this, &ViewAccount::backToLogin);
}

void ViewAccount::updateAccountStatus(){
    populateAccountStatus();
}

void ViewAccount::populateAccountStatus()
{




    Patron* patron = dynamic_cast<Patron*>(DataStore::instance().currentPatron());

    if (!patron) {
        ui->userStatusLabel->setText("Error: No Patron Logged In");
        return;
    }


    ui->userStatusLabel->setText(patron->username() + " Account Status");

    // --- Active Loans ---
    QList<Loan> loans = patron->activeLoans();
    ui->loansTable->setRowCount(loans.size());
    ui->loansTable->setColumnCount(3);
    ui->loansTable->setHorizontalHeaderLabels({"Item Title", "Due Date", "Days Remaining"});
    ui->loansTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->loansTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->loansTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->loansTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    // D1 Feedback Fix: Clear selection in holds table when a loan is selected
    connect(ui->loansTable, &QTableWidget::itemSelectionChanged, [this](){
        if (!ui->loansTable->selectedItems().isEmpty()) {
            ui->holdsTable->clearSelection();
        }
    });

    for (int i = 0; i < loans.size(); ++i) {
        const Loan& loan = loans.at(i);
        CatalogueItem* item = DataStore::instance().getItem(loan.itemId());
        QString title = item ? item->title() : "Unknown Item";

        ui->loansTable->setItem(i, 0, new QTableWidgetItem(title));
        ui->loansTable->setItem(i, 1, new QTableWidgetItem(loan.dueDate().toString("yyyy-MM-dd")));
        ui->loansTable->setItem(i, 2, new QTableWidgetItem(QString::number(loan.daysRemaining())));
    }

    // --- Active Holds ---
    QList<int> holds = patron->activeHolds();
    ui->holdsTable->setRowCount(holds.size());
    ui->holdsTable->setColumnCount(3);
    ui->holdsTable->setHorizontalHeaderLabels({"Item Title", "Queue Position", "Item ID"});
    ui->holdsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->holdsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->holdsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->holdsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    // D1 Feedback Fix: Clear selection in loans table when a hold is selected
    connect(ui->holdsTable, &QTableWidget::itemSelectionChanged, [this](){
        if (!ui->holdsTable->selectedItems().isEmpty()) {
            ui->loansTable->clearSelection();
        }
    });

    for (int i = 0; i < holds.size(); ++i) {
        int itemId = holds.at(i);
        CatalogueItem* item = DataStore::instance().getItem(itemId);
        QString title = item ? item->title() : "Unknown Item";
        int position = item ? item->holdQueuePosition(patron->username()) : 0;

        ui->holdsTable->setItem(i, 0, new QTableWidgetItem(title));
        ui->holdsTable->setItem(i, 1, new QTableWidgetItem(QString::number(position)));
        ui->holdsTable->setItem(i, 2, new QTableWidgetItem(QString::number(itemId))); // Hidden column for ID
        ui->holdsTable->hideColumn(2);
    }

}

void ViewAccount::on_returnButton_clicked()
{
    QList<QTableWidgetItem*> selected = ui->loansTable->selectedItems();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Selection Error", "Please select a loan to return.");
        return;
    }

    // The item ID is in the first column (index 0) of the selected row.
    int row = selected.first()->row();
    // The item ID is not in the table, only title, due date, and days remaining.
    // I need to get the item ID from the Patron's active loans list.
    // Since the table is populated in the same order as the activeLoans list,
    // I can use the row index to get the item ID.

    Patron* patron = DataStore::instance().currentPatron();
    if (!patron) return;

    int itemId = patron->activeLoans().at(row).itemId();

    // Emit a signal to mainwindow to handle the return logic and UI updates
    emit returnItem(itemId);
    // The mainwindow slot will handle the success message and refresh.
}

void ViewAccount::on_cancelHoldButton_clicked()
{
    QList<QTableWidgetItem*> selected = ui->holdsTable->selectedItems();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Selection Error", "Please select a hold to cancel.");
        return;
    }

    // The item ID is in the third column (index 2) of the selected row.
    int row = selected.first()->row();
    int itemId = ui->holdsTable->item(row, 2)->text().toInt();

    Patron* patron = DataStore::instance().currentPatron();
    if (!patron) return;

    if (patron->cancelHold(itemId)) {
        QMessageBox::information(this, "Hold Cancelled", "Hold successfully cancelled.");
        populateAccountStatus(); // Refresh the view
    } else {
        QMessageBox::warning(this, "Cancellation Failed", "Cancellation failed: Hold not found.");
    }
}

ViewAccount::~ViewAccount()
{
    delete ui;
}
