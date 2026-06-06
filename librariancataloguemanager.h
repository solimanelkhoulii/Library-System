#ifndef LIBRARIANCATALOGUEMANAGER_H
#define LIBRARIANCATALOGUEMANAGER_H

#include <QWidget>
#include <QTableWidget>
#include "datastore.h"
#include "catalogueitem.h"

namespace Ui {
class LibrarianCatalogueManager;
}

class LibrarianCatalogueManager : public QWidget
{
    Q_OBJECT

public:
    explicit LibrarianCatalogueManager(QWidget *parent = nullptr);
    ~LibrarianCatalogueManager();

public slots:
    void populateCatalogue();


signals:
    void backToLibrarianMenu();

private slots:
    void on_addButton_clicked();
    void on_removeButton_clicked();

private:
    Ui::LibrarianCatalogueManager *ui;
    void setupCatalogueTable();
};

#endif // LIBRARIANCATALOGUEMANAGER_H
