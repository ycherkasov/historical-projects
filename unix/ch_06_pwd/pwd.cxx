#include <iostream>
#include <pwd.h>

using std::string;
using std::cout;
using std::cerr;
using std::endl;

/*
 * Chapter 6 Stievens, Rago
 * POSIX security
 *
 * 1. Чтобы просмотреть информацию о конкретном юзере, используется getpwuid()/getpwnam()
 * 2. Для енумерации записей passwd используется setpwent()/getpwent()/endpwent()
 * 3. Чтобы просто получит имя залогиненнго пользователя, испльзуйте getlogin()
 */

/** @brief Show user information from passwd */
void print_passwd_info(struct passwd* pw) {
    cout << "Name: " << pw->pw_name << endl;
    cout << "Passwd: " << pw->pw_passwd << endl;
    cout << "Dir: " << pw->pw_dir << endl;
    cout << "Shell: " << pw->pw_shell << endl;
    cout << "UID: " << pw->pw_uid << endl;

}

/** @brief Show user information by UID or name */
void show_passwd() {

    // get user name
    cout << "Current user: " << getlogin() << endl;

    // get passwd struct of current user
    struct passwd* pw1 = getpwuid(getuid());
    print_passwd_info(pw1);

    // the same version by user name
    struct passwd* pw2 = getpwnam(pw1->pw_name);
    print_passwd_info(pw1);
}

/** @brief Show passwd enumeration */
void enumerate_passwd() {
    setpwent();
    struct passwd* pw = 0;
    while (NULL != (pw = getpwent())) {
        print_passwd_info(pw);
    }
    endpwent();
}

int main(int argc, char* argv[]) {

    show_passwd();
    enumerate_passwd();

    return 0;
}
