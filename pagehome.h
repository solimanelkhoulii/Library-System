#ifndef PAGEHOME_H
#define PAGEHOME_H

#include <QWidget>
#include "user.h"

namespace Ui {
class pagehome;
}

class pagehome : public QWidget
{
    Q_OBJECT

public:
    explicit pagehome(QWidget *parent = nullptr);
    ~pagehome();

private:
    Ui::pagehome *ui;

signals:
    void loginSuccess(User::UserType type);

private slots:
    void checkName();

};

#endif // PAGEHOME_H
