#include <iostream>
#include "user.h"

int main() {
    setlocale(LC_ALL, "Russian");

    short userCommand;
    std::cout << "����� ���������� � ������������ ������������������ ������!" << std::endl;
    std::cout << "��� ������ ��� ���������� ����� � �������, ��� ������� ���." << std::endl;
    std::cout << "����������� - 1\n���� - 2" << std::endl;
    std::cout << "��� �����? - ";
    std::cin >> userCommand;

    if (userCommand == 1)
    {
        user::signUp();
    }
    user activeUser;
    activeUser = user::signIn();
    std::cout << "�������� ��������� ��������: " << std::endl;
    std::cout << "������� ����� ������� - 1" << std::endl;
    std::cout << "��� �����? - ";
    std::cin >> userCommand;

    switch (userCommand)
    {
        case 1:
            activeUser.createNewRecord();
            break;
    }
}