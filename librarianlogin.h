#ifndef LIBRARIANLOGIN_H
#define LIBRARIANLOGIN_H

#include <QWidget>

namespace Ui {
class LibrarianLogin;
}

class LibrarianLogin : public QWidget
{
    Q_OBJECT

public:
    explicit LibrarianLogin(QWidget *parent = nullptr);
    ~LibrarianLogin();

signals:
    void logout();
    void viewCatalogue();
    void managePatrons();
    void restartSystem();

private:
    Ui::LibrarianLogin *ui;
};

#endif // LIBRARIANLOGIN_H
