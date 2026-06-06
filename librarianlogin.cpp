#include "librarianlogin.h"
#include "ui_librarianlogin.h"
#include <QPushButton>

LibrarianLogin::LibrarianLogin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LibrarianLogin)
{
    ui->setupUi(this);
    connect(ui->logoutButton, &QPushButton::clicked, this, &LibrarianLogin::logout);
    connect(ui->viewCatalogueButton, &QPushButton::clicked, this, &LibrarianLogin::viewCatalogue);
    connect(ui->managePatronsButton, &QPushButton::clicked, this, &LibrarianLogin::managePatrons);
    connect(ui->restartSystemButton, &QPushButton::clicked, this, &LibrarianLogin::restartSystem);
}

LibrarianLogin::~LibrarianLogin()
{
    delete ui;
}
