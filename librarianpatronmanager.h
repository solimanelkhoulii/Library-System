#ifndef LIBRARIANPATRONMANAGER_H
#define LIBRARIANPATRONMANAGER_H

#include <QWidget>
#include <QTableWidget>
#include "datastore.h"
#include "patron.h"

namespace Ui {
class LibrarianPatronManager;
}

class LibrarianPatronManager : public QWidget
{
    Q_OBJECT

public:
    explicit LibrarianPatronManager(QWidget *parent = nullptr);
    ~LibrarianPatronManager();

public slots:
    void populatePatronData();

signals:
    void backToLibrarianMenu();

private slots:
    void on_patronListTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_returnBookButton_clicked();
    void on_cancelHoldButton_clicked();

private:
    Ui::LibrarianPatronManager *ui;
    void populateLoansTable(Patron* patron);
    void populateHoldsTable(Patron* patron);
};

#endif // LIBRARIANPATRONMANAGER_H
