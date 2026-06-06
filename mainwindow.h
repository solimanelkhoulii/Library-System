#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "user.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class login;
class pagehome;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void setCurrentUser(User *user) {currentUser = user; }
    User* getCurrentUser() const {return currentUser;}
    ~MainWindow();

private:
    User *currentUser = nullptr; // keep track of current user
    Ui::MainWindow *ui;
    pagehome *pageHome;
    login *pageLogin;
    //other page pointers
    QSqlDatabase db;

};
#endif // MAINWINDOW_H
