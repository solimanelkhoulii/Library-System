#include "adminlogin.h"
#include "ui_adminlogin.h"
#include <QPushButton>

AdminLogin::AdminLogin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminLogin)
{
    ui->setupUi(this);
    ui->titleLabel->setGeometry(QRect(220,100,400,50));
    connect(ui->logoutButton, &QPushButton::clicked, this, &AdminLogin::logout);
}

AdminLogin::~AdminLogin()
{
    delete ui;
}
