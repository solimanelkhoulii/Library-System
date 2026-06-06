#ifndef BROWSECATALOGUE_H
#define BROWSECATALOGUE_H

#include <QWidget>
#include <QObject>
#include <QMessageBox>



namespace Ui {
class BrowseCatalogue;
}

class BrowseCatalogue : public QWidget
{
    Q_OBJECT

public:
    explicit BrowseCatalogue(QWidget *parent = nullptr);
    void populateCatalogue();
    ~BrowseCatalogue();

signals:
    void backToLogin();
    void itemBorrowed();
    void borrowItemRequest(int itemId);
    void returnItemRequest(int itemId);
    void placeHoldRequest(int itemId);
    void backToMenu();

private:
    Ui::BrowseCatalogue *ui;

private slots:
    void on_borrowButton_clicked();
    void on_returnButton_clicked();
    void on_holdButton_clicked();
    void on_detailsButton_clicked();
    void on_backButton_clicked();



};

#endif // BROWSECATALOGUE_H
