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


		// ������ ������ � ������
		static bool checkLoginData(user);
		static bool checkUserExists(user);

	public:

		// ������������
		user();
		user(std::string, std::string);


		// get-������
		std::string getLogin() const;
		std::string getPassword() const;
		void displayRecords() const;


		// set-������
		void setLogin(std::string);
		void setPassword(std::string);


		// ������ ������ � ������
		void write(const char*);
		void writeLogin(const char*);
		static std::vector<user*> readAll(const char*);


		// auth-������
		static void signUp();
		static user signIn();

		// ������ ������ � ��������
		void createNewRecord();

		// ������������� ������
		bool operator== (user);

		// ���������� ������� �� ������������
		std::vector<UserData*> filterRecords(std::vector<UserData*>) const;

};

class UserData
{
	protected:
		std::string ownerLogin;

	public:

		// ������������
		UserData();
		UserData(std::string);

		// get-������
		std::string getOwner() const;
		virtual void display() const;

		// ������ ������ � ������
		virtual void write() = 0;
		virtual void writeType() = 0;
		static std::string read(std::ifstream&);
		static std::vector<UserData*> readAll();

		// ������ �������������� � �������
		virtual void create() = 0;

};

class UserPublication : public UserData
{
	protected:
		std::string title;

	public:

		// ������������
		UserPublication();
		UserPublication(std::string);
		UserPublication(std::string, std::string);

		// get-������
		std::string getTitle() const;
		void display() const;

		// ������ ������ � ������
		void write();
		void writeType();
		static std::string readTitle(std::ifstream&);
		static UserData* read(std::ifstream&);

		// ������ �������������� � �����������
		void create();

};