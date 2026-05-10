#pragma once
#include"Person.h"

using namespace std;

class Patient :public Person
{
	friend ostream& operator<<(ostream& out, const Patient& a);
private:
	int age;
	char gender;
	string contact;
	double balance;
public:
	Patient();
	Patient(int id, string n, string pass,int age, char gen, string con, double bal);
	int getAge() const;
	char getGender() const;
	string getContact() const;
	double getBalance() const;
	void setBalance(double b);
	void displayMenu()override;
	void display()override;
	Patient& operator+=(double amount);
	Patient& operator-=(double amount);
	bool operator==(const Patient& other) const;

};