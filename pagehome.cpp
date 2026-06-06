#include "pagehome.h"
#include "ui_pagehome.h"
#include "datastore.h"
#include "user.h"
#include <QPushButton>
#include <QLineEdit>
#include "mainwindow.h"
#include <QDebug>

//This is the log in page

pagehome::pagehome(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::pagehome)
{
    ui->setupUi(this);
    ui->titleLabel->setText("HinLIBS\nHintonville Library - Information and Borrowing System");
    ui->titleLabel->setStyleSheet("font-size: 12pt;");
    ui->titleLabel->setGeometry(QRect(180,10,700,200));

    //when login button pressed, call the checkName function
    connect(ui->loginButton, &QPushButton::clicked, this, &pagehome::checkName);
    connect(ui->usernameText, &QLineEdit::returnPressed, this, &pagehome::checkName);
}

pagehome::~pagehome()
{
    delete ui;
}

void pagehome::checkName() {
    QString name = ui->usernameText->text().trimmed();
    User* user = DataStore::instance().authenticateUser(name);
    if (user && user->type() == User::UserType::Patron) {
        // Safe to cast or use as Patron
        DataStore::instance().setCurrentUser(dynamic_cast<Patron*>(user));
    } else {
        DataStore::instance().setCurrentUser(user);
    }

    //if it actually updated current user
    if (user) {
        emit loginSuccess(user->type());
    } else {
        ui->loginLabel->setText("Incorrect user, try again");
    }
}
