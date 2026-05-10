#pragma once
#include<iostream>
#include<string>
using namespace std;

class Person
{
protected:
	int ID;
	string name;
	string password;
public:
	Person();
	Person(int id, string n, string pass);

	int getID() const;
	string getName() const;
	string getPassword() const;
	void setName(string n);
	void setPassword(string p);


	virtual void displayMenu() = 0;
	virtual void display() = 0;

	virtual~Person();
};
