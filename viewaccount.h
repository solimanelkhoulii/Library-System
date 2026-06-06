#ifndef VIEWACCOUNT_H
#define VIEWACCOUNT_H

#include <QWidget>


namespace Ui {
class ViewAccount;
}

class ViewAccount : public QWidget
{
    Q_OBJECT

public:
    explicit ViewAccount(QWidget *parent = nullptr);
    void populateAccountStatus();
    ~ViewAccount();

signals:
    void backToLogin();
    void cancelHold(int itemId);
    void itemReturned();
    void returnItem(int itemId);

private:
    Ui::ViewAccount *ui;

private slots:
    void on_returnButton_clicked();
    void on_cancelHoldButton_clicked();

public slots:
    void updateAccountStatus();
};

#endif // VIEWACCOUNT_H
