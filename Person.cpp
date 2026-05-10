#include"Person.h"
using namespace std;

Person::Person()
{
	ID = 0;
	name = "Unknown";
	password = "0";
}
Person::Person(int id, string n, string pass)
{
	ID = id;
	name = n;
	password = pass;
}
int Person::getID() const
{
	return ID;
}
string Person::getName() const
{
	return name;
}
string Person::getPassword() const
{
	return password;
}
void Person::setName(string n)
{
	name = n;
}
void Person::setPassword(string p)
{
	password = p;
}
Person::~Person(){}