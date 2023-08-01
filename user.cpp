#include <fstream>
#include <iostream>
#include "user.h"


// ------------------------------------------------------------------------------------------------------------------------------
// Методы класса user
// ------------------------------------------------------------------------------------------------------------------------------



// Конструкторы ---------------------------------------------------------------------------------------------

user::user()
{
	login = "";
	password = "";
}

user::user(std::string l, std::string p) : login(l), password(p)
{}



// get-методы -----------------------------------------------------------------------------------------------

std::string user::getLogin() const
{
	/*
	  Возвращает логин пользователя
	*/

	return this->login;
}

std::string user::getPassword() const
{
	/*
	  Возвращает пароль пользователя
	*/

	return this->password;
}



// set-методы -----------------------------------------------------------------------------------------------

void user::setLogin(std::string newLogin)
{
	this->login = newLogin;
}

void user::setPassword(std::string newPassword)
{
	this->password = newPassword;
}



// Методы работы с файлом -----------------------------------------------------------------------------------

bool user::checkUserExists(user toCheck)
{
	/*
	  Проверяет, есть ли уже такой пользователь в базе
	*/

	std::vector<user*> allUsers = user::readAll(USERFILENAME);

	for (size_t i = 0; i < allUsers.size(); i++)
	{
		if (*(allUsers[i]) == toCheck) {
			return true;
		}
	}
	return false;
}

bool user::checkLoginData(user toCheck)
{
	std::vector<user*> allUsers = user::readAll(USERFILENAME);

	for (size_t i = 0; i < allUsers.size(); i++)
	{
		if (*(allUsers[i]) == toCheck && allUsers[i]->getPassword() == toCheck.getPassword())
		{
			return true;
		}
	}
	return false;
}

void user::write(const char* filename)
{
	std::fstream file;
	file.open(filename, std::ios::out | std::ios::app | std::ios::binary);

	size_t strSize;
	this->writeLogin(filename);

	strSize = this->password.size();
	file.write(reinterpret_cast<char*>(&strSize), sizeof(strSize));
	file.write(reinterpret_cast<char*>(&this->password[0]), strSize);

}

void user::writeLogin(const char* filename)
{
	std::ofstream file;
	file.open(filename, std::ios::binary | std::ios::app);

	size_t strSize;
	strSize = this->login.size();
	file.write(reinterpret_cast<char*>(&strSize), sizeof(strSize));
	file.write(reinterpret_cast<char*>(&this->login[0]), strSize);
}

std::vector<user*> user::readAll(const char* filename)
{
	std::vector<user*> users; // Возвращаемый массив указателей на всех пользователей
	user* readUser; // Указатель, который будет указывать на новые объекты в соответствии с читаемыми данными
	std::string login, password; // Строки для чтения из файла
	size_t size = 0;

	std::ifstream file;
	file.open(filename, std::ios::binary);

	while (file)
	{
		file.read(reinterpret_cast<char*>(&size), sizeof(size));
		login.resize(size);
		file.read(reinterpret_cast<char*>(&login[0]), size);

		file.read(reinterpret_cast<char*>(&size), sizeof(size));
		password.resize(size);
		file.read(reinterpret_cast<char*>(&password[0]), size);

		readUser = new user(login, password);
		users.push_back(readUser);
	}
	return users;
}

// auth-методы ----------------------------------------------------------------------------------------------

void user::signUp()
{
	std::string login, password;
	std::cout << "Введите логин: ";
	std::cin >> login;
	std::cout << "Введите пароль: ";
	std::cin >> password;
	user newUser(login, password);
	if (user::checkUserExists(newUser)) {
		std::cout << "Такая учетная запись уже имеется в базе!" << std::endl;
	}
	else {
		newUser.write(USERFILENAME);
		std::cout << "Регистрация прошла успешно!" << std::endl;
	}
}

user user::signIn()
{
	std::string login, password;
	std::cout << "Введите логин: ";
	std::cin >> login;
	std::cout << "Введите пароль: ";
	std::cin >> password;

	user loginData(login, password);
	if (user::checkLoginData(loginData))
	{
		std::cout << "Вход успешно выполнен!" << std::endl << std::endl;
		loginData.records = loginData.filterRecords(UserData::readAll());
		return loginData;
	}
	else {
		std::cout << "Неверные данные аутентификации!" << std::endl;
	}
	return user();
}



// Методы работы с записями ----------------------------------------------------------------------------------

void user::createNewRecord()
{
	short userCommand;
	UserData* newRecord;
	std::cout << "Создать новую публикацию - 1" << std::endl;
	std::cout << "Ваш выбор? - ";
	std::cin >> userCommand;

	switch (userCommand)
	{
		case 1:
			newRecord = new UserPublication(this->login);
			newRecord->create();
			this->records.push_back(newRecord);
			break;
		//case 2: ...
	}
}

// Перегруженные методы -------------------------------------------------------------------------------------

bool user::operator== (user second)
{
	if (this->login == second.login)
	{
		return true;
	}
	return false;
}



// Фильтрация записей по пользователю

std::vector<UserData*> user::filterRecords(std::vector<UserData*> allRecords) const
{
	std::vector<UserData*> userRecords;

	for (size_t i = 0; i < allRecords.size(); i++)
	{
		if (allRecords[i]->getOwner() == this->login)
		{
			userRecords.push_back(allRecords[i]);
		}
	}

	return userRecords;
}


// ------------------------------------------------------------------------------------------------------------------------------
//  Методы класса UserData
// ------------------------------------------------------------------------------------------------------------------------------



//  Конструкторы --------------------------------------------------------------------------------------------

UserData::UserData()
{
	ownerLogin = "";
}

UserData::UserData(std::string setOwner)
{
	ownerLogin = setOwner;
}



//  get-методы ----------------------------------------------------------------------------------------------

std::string UserData::getOwner() const
{
	return ownerLogin;
}



//  Методы работы с файлом

void UserData::write()
{  
	std::ofstream file;
	file.open(DATAFILENAME, std::ios::binary | std::ios::app);

	size_t strSize;
	strSize = this->ownerLogin.size();
	file.write(reinterpret_cast<char*>(&strSize), sizeof(strSize));
	file.write(reinterpret_cast<char*>(&this->ownerLogin[0]), strSize);
}

std::string UserData::read(std::ifstream& file)
{
	size_t strSize = 0;
	std::string owner;
	file.read(reinterpret_cast<char*>(&strSize), sizeof(strSize));
	owner.resize(strSize);
	file.read(reinterpret_cast<char*>(&owner[0]), strSize);
	return owner;
}

std::vector<UserData*> UserData::readAll()
{
	std::vector<UserData*> records;
	UserData* readedRecord;
	std::ifstream file;
	size_t strSize = 0;
	std::string owner, type;

	file.open(DATAFILENAME, std::ios::binary);
	file.seekg(0, std::ios::end);
	int eof = file.tellg();

	file.seekg(0, std::ios::beg);
	while (file.tellg() < eof)
	{
		file.read(reinterpret_cast<char*>(&strSize), sizeof(strSize));
		type.resize(strSize);
		file.read(reinterpret_cast<char*>(&type[0]), strSize);
		if (type == "Pub")
		{
			readedRecord = UserPublication::read(file);
		}
		records.push_back(readedRecord);
	}

	return records;
}



// ------------------------------------------------------------------------------------------------------------------------------
//  Методы класса UserPublucation : UserData
// ------------------------------------------------------------------------------------------------------------------------------



//  Конструкторы --------------------------------------------------------------------------------------------

UserPublication::UserPublication() : UserData::UserData()
{
	title = "";
}

UserPublication::UserPublication(std::string ownerLogin) : UserData::UserData(ownerLogin)
{}

UserPublication::UserPublication(std::string ownerLogin, std::string setTitle) : UserData::UserData(ownerLogin)
{
	title = setTitle;
}



//  get-методы ----------------------------------------------------------------------------------------------

std::string UserPublication::getTitle() const
{
	return this->title;
}



//  Методы работы с файлом

void UserPublication::write()
{
	std::ofstream file;
	file.open(DATAFILENAME, std::ios::binary | std::ios::app);

	this->writeType();							 // Запись типа записи
	UserData::write();                           // Запись имени пользователя

	size_t strSize;
	strSize = this->title.size();
	file.write(reinterpret_cast<char*>(&strSize), sizeof(strSize));
	file.write(reinterpret_cast<char*>(&this->title[0]), strSize); // Запись заголовка публикации
}

void UserPublication::writeType()
{
	std::ofstream file;
	std::string type = "Pub";

	file.open(DATAFILENAME, std::ios::binary | std::ios::app);
	size_t strSize;
	strSize = type.size();
	file.write(reinterpret_cast<char*>(&strSize), sizeof(strSize));
	file.write(reinterpret_cast<char*>(&type[0]), strSize);
}

std::string UserPublication::readTitle(std::ifstream& file)
{
	std::string title;

	size_t strSize = 0;
	file.read(reinterpret_cast<char*>(&strSize), sizeof(strSize));
	title.resize(strSize);
	file.read(reinterpret_cast<char*>(&title[0]), strSize);

	return title;
}

UserData* UserPublication::read(std::ifstream& file)
{
	std::string owner, title;

	owner = UserData::read(file);
	title = UserPublication::readTitle(file);

	return new UserPublication(owner, title);
}



//  Методы взаимодействия с публикацией ---------------------------------------------------------------------

void UserPublication::create()
{
	std::cout << "Создание публикации" << std::endl << std::endl;

	std::cout << "Введите название публикации: ";
	std::cin >> this->title;

	this->write();
}