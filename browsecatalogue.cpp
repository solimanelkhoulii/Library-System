#include "browsecatalogue.h"
#include "ui_browsecatalogue.h"
#include "datastore.h"
#include "catalogueitem.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QDebug>




BrowseCatalogue::BrowseCatalogue(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BrowseCatalogue)
{
    ui->setupUi(this);
    populateCatalogue();
}

BrowseCatalogue::~BrowseCatalogue()
{
    delete ui;
}


void BrowseCatalogue::on_backButton_clicked() {
    emit backToMenu();
}

void BrowseCatalogue::on_borrowButton_clicked()
{
    QModelIndexList selectedRows = ui->catalogueTable->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Selection Error", "Please select an item to borrow.");
        return;
    }

    // get the first selected row
    int row = selectedRows.first().row();

    // column 0 is where your ID should be
    QTableWidgetItem* idItem = ui->catalogueTable->item(row, 0);
    if (!idItem) {
        QMessageBox::warning(this, "Error", "Failed to read selected item ID.");
        return;
    }

    int itemId = idItem->text().toInt();

    emit borrowItemRequest(itemId);

    // repopulate the table to show updated availability
    populateCatalogue();
}


//void BrowseCatalogue::on_borrowButton_clicked()
//{
//    QList<QTableWidgetItem*> selected = ui->catalogueTable->selectedItems();
//    if (selected.isEmpty()) {
//        QMessageBox::warning(this, "Selection Error", "Please select an item to borrow.");
//        return;
//    }
//    int itemId = selected.first()->text().toInt();
//    emit borrowItemRequest(itemId);
//    populateCatalogue();
//}

void BrowseCatalogue::on_returnButton_clicked()
{
    QList<QTableWidgetItem*> selected = ui->catalogueTable->selectedItems();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Selection Error", "Please select an item to return.");
        return;
    }
    int itemId = selected.first()->text().toInt();
    emit returnItemRequest(itemId);
    populateCatalogue();
}

void BrowseCatalogue::on_holdButton_clicked()
{
    QList<QTableWidgetItem*> selected = ui->catalogueTable->selectedItems();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Selection Error", "Please select an item to place a hold on.");
        return;
    }
    int itemId = selected.first()->text().toInt();
    emit placeHoldRequest(itemId);
    populateCatalogue();
}

void BrowseCatalogue::on_detailsButton_clicked()
{
    QList<QTableWidgetItem*> selected = ui->catalogueTable->selectedItems();
    if (selected.isEmpty()) {
        QMessageBox::information(this, "Item Details", "Please select an item to view its details.");
        return;
    }
    int itemId = selected.first()->text().toInt();
    CatalogueItem* item = DataStore::instance().getItem(itemId);
    if (item) {
        QMessageBox::information(this, "Item Details", item->details());
    }
}



void BrowseCatalogue::populateCatalogue()
{
    // --- step 1: load from DB into DataStore ---
    QSqlQuery query;
    if (!query.exec("SELECT item_id, item_type, title, author, format, is_available, total_copies FROM CATALOGUE")) {
        qDebug() << "Failed to query catalogue:" << query.lastError().text();
        return;
    }

    // clear the datastore first
    DataStore::instance().clear();

    while (query.next()) {
        int id = query.value("item_id").toInt();
        QString typeStr = query.value("item_type").toString();
        QString title = query.value("title").toString();
        QString author = query.value("author").toString(); // mapped to creator
        QString format = query.value("format").toString();
//        bool available = query.value("is_available").toBool();
        int copies = query.value("total_copies").toInt();

        // map DB type -> enum (skip unknown)
        CatalogueItem::ItemType itemType;
        if      (typeStr == "Fiction")     itemType = CatalogueItem::FictionBook;
        else if (typeStr == "Non-Fiction") itemType = CatalogueItem::NonFictionBook;
        else if (typeStr == "Magazine")    itemType = CatalogueItem::Magazine;
        else if (typeStr == "Movie")       itemType = CatalogueItem::Movie;
        else if (typeStr == "VideoGame")   itemType = CatalogueItem::VideoGame;
        else continue;

        // convert availability into copies (constructor expects copies)
//        int copies = available ? 1 : 0;

        // construct and insert into datastore
        CatalogueItem* itm = new CatalogueItem(id, itemType, title, author, format, copies);
        DataStore::instance().m_items.insert(id, itm);
    }

    // --- step 2: populate table from DataStore (match commented layout) ---
    QList<CatalogueItem*> items = DataStore::instance().allItems();

    // configure table to match your commented version
    ui->catalogueTable->setRowCount(items.size());
    ui->catalogueTable->setColumnCount(5);
    ui->catalogueTable->setHorizontalHeaderLabels({"ID", "Title", "Creator", "Format", "Status"});
    ui->catalogueTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->catalogueTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->catalogueTable->setSelectionMode(QAbstractItemView::SingleSelection);

    for (int i = 0; i < items.size(); ++i) {
        CatalogueItem* item = items.at(i);
        if (!item) continue;

        ui->catalogueTable->setItem(i, 0, new QTableWidgetItem(QString::number(item->id())));
        ui->catalogueTable->setItem(i, 1, new QTableWidgetItem(item->title()));
        ui->catalogueTable->setItem(i, 2, new QTableWidgetItem(item->creator())); // constructor stored author -> creator
        ui->catalogueTable->setItem(i, 3, new QTableWidgetItem(item->format()));
        ui->catalogueTable->setItem(i, 4, new QTableWidgetItem(item->status())); // assumes status() returns "Available"/"Checked Out"
    }

    // resize first 5 columns to contents (like your commented code)
    for (int c = 0; c < 5; ++c) {
        ui->catalogueTable->resizeColumnToContents(c);
    }

    ui->catalogueTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}


