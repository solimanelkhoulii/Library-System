#ifndef ADMINLOGIN_H
#define ADMINLOGIN_H

#include <QWidget>

namespace Ui {
class AdminLogin;
}

class AdminLogin : public QWidget
{
    Q_OBJECT

public:
    explicit AdminLogin(QWidget *parent = nullptr);
    ~AdminLogin();

signals:
    void logout();

private:
    Ui::AdminLogin *ui;
};

#endif // ADMINLOGIN_H
