#pragma once
#include"Person.h"
using namespace std;

class Doctor :public Person
{
	friend ostream& operator<<(ostream& out, const Doctor& a);
private:
	string specialization;
	string contact;
	double fee;
public:
	Doctor();
	Doctor(int id, string n, string pass, string spec, string con, double f);
	string getSpecialization() const;
	string getContact() const;
	double getFee() const;
	void setFee(double f);
	void displayMenu()override;
	void display()override;
	bool operator==(const Doctor& other) const;
};