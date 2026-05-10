#pragma once
#include"Person.h"
using namespace std;

class Admin :public Person
{
public:
	Admin();
	Admin(int id, string n, string pass);
	void displayMenu()override;
	void display()override;
};