#include <iostream>
#include "user.h"

int main() {
    setlocale(LC_ALL, "Russian");

    short userCommand;
    std::cout << "Добро пожаловать в терминальную аутентификационную службу!" << std::endl;
    std::cout << "Для начала вам необходимо войти в аккаунт, или создать его." << std::endl;
    std::cout << "Регистрация - 1\nВход - 2" << std::endl;
    std::cout << "Ваш выбор? - ";
    std::cin >> userCommand;

    if (userCommand == 1)
    {
        user::signUp();
    }
    user activeUser;
    activeUser = user::signIn();
    std::cout << "Перечень доступных действий: " << std::endl;
    std::cout << "Создать новый элемент - 1" << std::endl;
    std::cout << "Ваш выбор? - ";
    std::cin >> userCommand;

    switch (userCommand)
    {
        case 1:
            activeUser.createNewRecord();
            break;
    }
}