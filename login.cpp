#include "login.h"
#include "ui_login.h"
#include <QPushButton>
#include "viewaccount.h"
#include "datastore.h"
#include "user.h"

//patron is logged in screen

login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    connect(ui->logoutButton, &QPushButton::clicked, this, &login::logout);
    connect(ui->viewAccountButton, &QPushButton::clicked, this, &login::viewAccount);
    connect(ui->browseCatalogueButton, &QPushButton::clicked, this, &login::browseCatalogue);
}

void login::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    // Update username when page is shown
    User* user = DataStore::instance().getCurrentUser();
    if (user) {
        ui->userLabel->setText("Logged in as: " + user->username());
    }

}

login::~login()
{
    delete ui;
}


