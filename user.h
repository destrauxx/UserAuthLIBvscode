#pragma once
#include <string>
#include <vector>

const char USERFILENAME[] = "users.dat";
const char DATAFILENAME[] = "data.dat";

class UserData;

class user
{
	private:
		std::string login;
		std::string password;
		std::vector<UserData*> records;


		// Методы работы с файлом
		static bool checkLoginData(user);
		static bool checkUserExists(user);

	public:

		// Конструкторы
		user();
		user(std::string, std::string);


		// get-методы
		std::string getLogin() const;
		std::string getPassword() const;
		void displayRecords() const;


		// set-методы
		void setLogin(std::string);
		void setPassword(std::string);


		// Методы работы с файлом
		void write(const char*);
		void writeLogin(const char*);
		static std::vector<user*> readAll(const char*);


		// auth-методы
		static void signUp();
		static user signIn();

		// Методы работы с записями
		void createNewRecord();

		// Перегруженные методы
		bool operator== (user);

		// Фильтрация записей по пользователю
		std::vector<UserData*> filterRecords(std::vector<UserData*>) const;

};

class UserData
{
	protected:
		std::string ownerLogin;

	public:

		// Конструкторы
		UserData();
		UserData(std::string);

		// get-методы
		std::string getOwner() const;
		virtual void display() const;

		// Методы работы с файлом
		virtual void write() = 0;
		virtual void writeType() = 0;
		static std::string read(std::ifstream&);
		static std::vector<UserData*> readAll();

		// Методы взаимодействия с данными
		virtual void create() = 0;

};

class UserPublication : public UserData
{
	protected:
		std::string title;

	public:

		// Конструкторы
		UserPublication();
		UserPublication(std::string);
		UserPublication(std::string, std::string);

		// get-методы
		std::string getTitle() const;
		void display() const;

		// Методы работы с файлом
		void write();
		void writeType();
		static std::string readTitle(std::ifstream&);
		static UserData* read(std::ifstream&);

		// Методы взаимодействия с публикацией
		void create();

};