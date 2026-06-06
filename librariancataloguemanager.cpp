#include "librariancataloguemanager.h"
#include "ui_librariancataloguemanager.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QHeaderView>
#include <QDebug>
#include <QFormLayout>
#include <QComboBox>
#include <QDialogButtonBox>
#include  <QSpinBox>
#include <QRadioButton>

LibrarianCatalogueManager::LibrarianCatalogueManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LibrarianCatalogueManager)
{
    ui->setupUi(this);
    connect(ui->backButton, &QPushButton::clicked, this, &LibrarianCatalogueManager::backToLibrarianMenu);
    setupCatalogueTable();

}

LibrarianCatalogueManager::~LibrarianCatalogueManager()
{
    delete ui;
}

void LibrarianCatalogueManager::setupCatalogueTable()
{
    ui->catalogueTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->catalogueTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->catalogueTable->verticalHeader()->setVisible(false);
    ui->catalogueTable->setColumnCount(6);
    ui->catalogueTable->setHorizontalHeaderLabels(QStringList() << "ID" << "Type" << "Title" << "Creator" << "Format" << "Status");
    ui->catalogueTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void LibrarianCatalogueManager::populateCatalogue()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;

    if (!query.exec("SELECT item_id, item_type, title, author, format, is_available FROM CATALOGUE")) {
        qDebug() << "Failed to query catalogue:" << query.lastError().text();
        return;
    }

    // clear table and datastore first
    ui->catalogueTable->setRowCount(0);
    DataStore::instance().clear();

    int row = 0;
    while (query.next()) {
        int id = query.value("item_id").toInt();
        QString typeStr = query.value("item_type").toString();
        QString title = query.value("title").toString();
        QString author = query.value("author").toString();
        QString format = query.value("format").toString();
        bool available = query.value("is_available").toBool();

        //get the type
        CatalogueItem::ItemType itemType;
        if (typeStr == "Fiction") { itemType = CatalogueItem::FictionBook; }
        else if (typeStr == "Non-Fiction") {itemType = CatalogueItem::NonFictionBook;}
        else if (typeStr == "Magazine") {itemType = CatalogueItem::Magazine;}
        else if (typeStr == "Movie") {itemType = CatalogueItem::Movie;}
        else if (typeStr == "VideoGame") {itemType = CatalogueItem::VideoGame;}
        else { return; }

        //create catalogue item and add it to the in memory data-store
        CatalogueItem* itm = new CatalogueItem(id, itemType, title, author, format, 2);
        DataStore::instance().m_items.insert(id, itm);;


        // add to table
        ui->catalogueTable->insertRow(row);
        ui->catalogueTable->setItem(row, 0, new QTableWidgetItem(QString::number(id)));
        ui->catalogueTable->setItem(row, 1, new QTableWidgetItem(typeStr));
        ui->catalogueTable->setItem(row, 2, new QTableWidgetItem(title));
        ui->catalogueTable->setItem(row, 3, new QTableWidgetItem(author));
        ui->catalogueTable->setItem(row, 4, new QTableWidgetItem(format));
        ui->catalogueTable->setItem(row, 5, new QTableWidgetItem(available ? "Available" : "Checked Out"));

        ++row;
    }
}





//void LibrarianCatalogueManager::populateCatalogue()
//{
//    QSqlDatabase db = QSqlDatabase::database(); // default connection
//    QSqlQuery query;

 //    // select all items from your catalogue table
//    if (!query.exec("SELECT item_id, item_type, title, author, format, is_available FROM CATALOGUE")) {
//        qDebug() << "Failed to query catalogue:" << query.lastError().text();
//        return;
//    }

//    // clear the table first
//    ui->catalogueTable->setRowCount(0);

//    int row = 0;
//    while (query.next()) {
//        ui->catalogueTable->insertRow(row);
//        ui->catalogueTable->setItem(row, 0, new QTableWidgetItem(QString::number(query.value("item_id").toInt())));
//        ui->catalogueTable->setItem(row, 1, new QTableWidgetItem(query.value("item_type").toString()));
//        ui->catalogueTable->setItem(row, 2, new QTableWidgetItem(query.value("title").toString()));
//        ui->catalogueTable->setItem(row, 3, new QTableWidgetItem(query.value("author").toString()));
//        ui->catalogueTable->setItem(row, 4, new QTableWidgetItem(query.value("format").toString()));

//        // convert availability to status string
//        bool available = query.value("is_available").toBool();
//        ui->catalogueTable->setItem(row, 5, new QTableWidgetItem(available ? "Available" : "Checked Out"));

//        ++row;
//    }
//}

//void LibrarianCatalogueManager::populateCatalogue()
//{
//    QList<CatalogueItem*> items = DataStore::instance().allItems();
//    ui->catalogueTable->setRowCount(items.size());
    
//    for (int i = 0; i < items.size(); ++i) {
//        CatalogueItem* item = items.at(i);
        
//        ui->catalogueTable->setItem(i, 0, new QTableWidgetItem(QString::number(item->id())));
//        ui->catalogueTable->setItem(i, 1, new QTableWidgetItem(item->typeString()));
//        ui->catalogueTable->setItem(i, 2, new QTableWidgetItem(item->title()));
//        ui->catalogueTable->setItem(i, 3, new QTableWidgetItem(item->creator()));
//        ui->catalogueTable->setItem(i, 4, new QTableWidgetItem(item->format()));
//        ui->catalogueTable->setItem(i, 5, new QTableWidgetItem(item->status()));
//    }
//}

//This function brings up the Addd Item Dialog box
void LibrarianCatalogueManager::on_addButton_clicked()
{
    // set the window title
    QDialog dlg(this);
    dlg.setWindowTitle("Add New Item");

    //use form layout
    QFormLayout form(&dlg);

    // select the item type with radio buttons
    QRadioButton *fictionBtn = new QRadioButton("Fiction", &dlg);
    QRadioButton *nonFictionBtn = new QRadioButton("Non-Fiction", &dlg);
    QRadioButton *magazineBtn = new QRadioButton("Magazine", &dlg);
    QRadioButton *movieBtn = new QRadioButton("Movie", &dlg);
    QRadioButton *videoGameBtn = new QRadioButton("Video Game", &dlg);

    //set fiction to the default checked button
    fictionBtn->setChecked(true);

    //lay the radio buttons out  side by side
    QHBoxLayout *typeLayout = new QHBoxLayout;
    typeLayout->addWidget(fictionBtn);
    typeLayout->addWidget(nonFictionBtn);
    typeLayout->addWidget(magazineBtn);
    typeLayout->addWidget(movieBtn);
    typeLayout->addWidget(videoGameBtn);

    //add the horizontal layout of radio buttons to the form
    form.addRow("Type:", typeLayout);

    // == Item Fields  ==
    //title
    QLineEdit *titleEdit = new QLineEdit(&dlg);
    form.addRow("Title:", titleEdit);

    //author
    QLineEdit *authorEdit = new QLineEdit(&dlg);
    form.addRow("Author:", authorEdit);

    //dewey decimal
    QLineEdit *deweyEdit = new QLineEdit(&dlg);
    form.addRow("Dewey-Decimal:", deweyEdit);

    //ISBN
    QLineEdit *isbnEdit = new QLineEdit(&dlg);
    form.addRow("ISBN:", isbnEdit);

    //publication date
    QLineEdit *publicationDateEdit = new QLineEdit(&dlg);
    form.addRow("Publication Date:", publicationDateEdit);

    //issue number
    QLineEdit *issueEdit = new QLineEdit(&dlg);
    form.addRow("Issue Number:", issueEdit);

    //genre
    QLineEdit *genreEdit = new QLineEdit(&dlg);
    form.addRow("Genre:", genreEdit);

    //rating
    QLineEdit *ratingEdit = new QLineEdit(&dlg);
    form.addRow("Rating:", ratingEdit);

    //format
    QLineEdit *formatEdit = new QLineEdit(&dlg);
    form.addRow("Format:", formatEdit);

    //ficiton iis default selection, so initialize fields for correct values
    //toggle fields
    deweyEdit->setVisible(false);
    issueEdit->setVisible(false);
    genreEdit->setVisible(false);
    ratingEdit->setVisible(false);

    //toggle labels
    form.labelForField(deweyEdit)->setVisible(false);
    form.labelForField(issueEdit)->setVisible(false);
    form.labelForField(genreEdit)->setVisible(false);
    form.labelForField(ratingEdit)->setVisible(false);

    // fiction selected
    QObject::connect(fictionBtn, &QRadioButton::toggled, [&](bool checked){

        //toggle fields
        deweyEdit->setVisible(!checked);
        issueEdit->setVisible(!checked);
        genreEdit->setVisible(!checked);
        ratingEdit->setVisible(!checked);

        //toggle labels
        form.labelForField(deweyEdit)->setVisible(!checked);
        form.labelForField(issueEdit)->setVisible(!checked);
        form.labelForField(genreEdit)->setVisible(!checked);
        form.labelForField(ratingEdit)->setVisible(!checked);


    });

    // non-fiction selected
    QObject::connect(nonFictionBtn, &QRadioButton::toggled, [&](bool checked){
        //toggle fields
        deweyEdit->setVisible(checked);
        issueEdit->setVisible(!checked);
        genreEdit->setVisible(!checked);
        ratingEdit->setVisible(!checked);
        //toggle labels
        form.labelForField(deweyEdit)->setVisible(checked);
        form.labelForField(issueEdit)->setVisible(!checked);
        form.labelForField(genreEdit)->setVisible(!checked);
        form.labelForField(ratingEdit)->setVisible(!checked);

    });

    // magazine selected
    QObject::connect(magazineBtn, &QRadioButton::toggled, [&](bool checked){
        //toggle fields
        issueEdit->setVisible(checked);
        deweyEdit->setVisible(!checked);
        genreEdit->setVisible(!checked);
        ratingEdit->setVisible(!checked);

        //toggle labels
        form.labelForField(issueEdit)->setVisible(checked);
        form.labelForField(deweyEdit)->setVisible(!checked);
        form.labelForField(genreEdit)->setVisible(!checked);
        form.labelForField(ratingEdit)->setVisible(!checked);

    });

    // movie selected
    QObject::connect(movieBtn, &QRadioButton::toggled, [&](bool checked){
        //toggle fields
        genreEdit->setVisible(checked);
        ratingEdit->setVisible(checked);
        issueEdit->setVisible(!checked);
        deweyEdit->setVisible(!checked);
        //toggle labels
        form.labelForField(genreEdit)->setVisible(checked);
        form.labelForField(ratingEdit)->setVisible(checked);
        form.labelForField(issueEdit)->setVisible(!checked);
        form.labelForField(deweyEdit)->setVisible(!checked);
    });

    // video game selected
    QObject::connect(videoGameBtn, &QRadioButton::toggled, [&](bool checked){
        //toggle fields
        genreEdit->setVisible(checked);
        ratingEdit->setVisible(checked);
        issueEdit->setVisible(!checked);
        deweyEdit->setVisible(!checked);
        //toggle labels
        form.labelForField(genreEdit)->setVisible(checked);
        form.labelForField(ratingEdit)->setVisible(checked);
        form.labelForField(issueEdit)->setVisible(!checked);
        form.labelForField(deweyEdit)->setVisible(!checked);
    });

    // OK or Cancel button
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dlg);
    form.addRow(&buttonBox);

    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);


    //if user accepts to add the new item

    if (dlg.exec() == QDialog::Accepted) {

        // check selected type
        int selectedType;
        if (fictionBtn->isChecked()) selectedType = 1;
        if (nonFictionBtn->isChecked()) selectedType = 2;
        if (magazineBtn->isChecked()) selectedType = 3;
        if (movieBtn->isChecked()) selectedType = 4;
        if (videoGameBtn->isChecked()) selectedType = 5;


        //store the populated fields
        QVariantMap fields;
        fields["title"] = titleEdit->text();
        fields["author"] = authorEdit->text();
        fields["isbn"] = isbnEdit->text();
        fields["publicationDate"] = publicationDateEdit->text();
        fields["deweyDecimal"] = deweyEdit->text();
        fields["issue"] = issueEdit->text();
        fields["genre"] = genreEdit->text();
        fields["rating"] = ratingEdit->text();
        fields["format"] = formatEdit->text();

        //grab latest id  in datastore
        int newId = DataStore::instance().lastIndex() + 1;
        bool added = true;

        //check for correct field inputs
        switch (selectedType) {

            //fiction
            case 1: {
                //check valid fields
                if (fields.value("title").toString().length() <= 0) {
                    qDebug() << "Fiction add error: check author, title, publication date";
                    return;
                }

                //create the catalogue item, add it to the datstore, pass field values
                CatalogueItem* newItem = new CatalogueItem(newId, CatalogueItem::FictionBook, fields["title"].toString(), fields["author"].toString(), fields["format"].toString(), 1);
                DataStore::instance().addItem(newItem, fields);
                break;
            }

            //non-ficiton
        case 2: {
            //check valid fields
                if (fields.value("title").toString().length() <= 0
                        || fields.value("deweyDecimal").toString().length() <= 0
                        ) {
                    qDebug() << "Non-Fiction add error";
                    return;
                }

                //create the catalogue item, add it to the datstore, pass field values
                CatalogueItem* newItem = new CatalogueItem(newId, CatalogueItem::NonFictionBook, fields["title"].toString(), fields["author"].toString(), fields["format"].toString(), 1);
                DataStore::instance().addItem(newItem, fields);
                break;
            }

            //magazine
            case 3:{
            //check valid fields
                if (fields.value("title").toString().length() <= 0
                        || fields.value("issue").toString().length() <= 0
                        ) {
                    qDebug() << "magazine add error";
                    return;
                }

                //create the catalogue item, add it to the datstore, pass field values
                CatalogueItem* newItem = new CatalogueItem(newId, CatalogueItem::Magazine, fields["title"].toString(), fields["author"].toString(), fields["format"].toString(), 1);
                DataStore::instance().addItem(newItem, fields);
                break;
            }

            //movie
            case 4: {
            //check valid fields
                if (fields.value("title").toString().length() <= 0
                        || fields.value("genre").toString().length() <= 0
                        || fields.value("rating").toString().length() <= 0
                        ) {
                    qDebug() << "Non-Fiction add error";
                    return;
                }

                //create the catalogue item, add it to the datstore, pass field values
                CatalogueItem* newItem = new CatalogueItem(newId, CatalogueItem::Movie, fields["title"].toString(), fields["author"].toString(), fields["format"].toString(), 1);
                DataStore::instance().addItem(newItem, fields);
                break;
            }

            //video game
            case 5:
        {
                    //check valid fields
                        if (fields.value("title").toString().length() <= 0
                                || fields.value("genre").toString().length() <= 0
                                || fields.value("rating").toString().length() <= 0
                                ) {
                            qDebug() << "Non-Fiction add error";
                            return;
                        }

                        //create the catalogue item, add it to the datstore, pass field values
                        CatalogueItem* newItem = new CatalogueItem(newId, CatalogueItem::VideoGame, fields["title"].toString(), fields["author"].toString(), fields["format"].toString(), 1);
                        DataStore::instance().addItem(newItem, fields);
                        break;
                    }
                break;
            default:
                added = false;
                break;

        }

        //re-display catalogue after item updated
        if (added) {
            QMessageBox::information(this, "Item Added", QString("Item '%1' added with ID %2.").arg(fields.value("title").toString()).arg(QString::number(newId)));
            populateCatalogue();
        }


    }

}

//void LibrarianCatalogueManager::on_addButton_clicked_original()
//{
//    // Simplified add for D1. Only adding a generic fiction book.
//    bool ok;
//    QString title = QInputDialog::getText(this, "Add New Item", "Enter Title:", QLineEdit::Normal, "", &ok);
    
//    if (ok && !title.isEmpty()) {
//        // Find the next available ID (simple approach for D1)
//        int newId = 1;
//        while (DataStore::instance().getItem(newId) != nullptr) {
//            newId++;
//        }
        
//        CatalogueItem* newItem = new CatalogueItem(newId, CatalogueItem::FictionBook, title, "Librarian Added", "Digital", 1);
//        DataStore::instance().addItem(newItem);
//        QMessageBox::information(this, "Item Added", QString("Item '%1' added with ID %2.").arg(title).arg(newId));
//        populateCatalogue();
//    }
//    qDebug() << "ADD CLICK from:" << sender(); //test button press for num times
//}

void LibrarianCatalogueManager::on_removeButton_clicked()
{
    int currentRow = ui->catalogueTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Action Failed", "Please select an item to remove.");
        return;
    }
    
    int itemId = ui->catalogueTable->item(currentRow, 0)->text().toInt();
    
    DataStore::instance().removeItem(itemId);
    QMessageBox::information(this, "Item Removed", QString("Item ID %1 removed.").arg(itemId));
    populateCatalogue();
}
