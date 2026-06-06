#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QShowEvent>

namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

protected:
    void showEvent(QShowEvent *event) override;

signals:
    void logout();
    void viewAccount();
    void browseCatalogue();

private:
    Ui::login *ui;
};

#endif // LOGIN_H
