#include"Patient.h"
using namespace std;

Patient::Patient():Person()
{
	age = 0;
	gender = 'X';
	contact = "0";
	balance = 0;
}
Patient::Patient(int id, string n, string pass, int age, 
	char gen, string con, double bal) :Person(id,n,pass)
{
	this->age = age;
	gender = gen;
	contact = con;
	balance = bal;
}
int Patient::getAge() const
{
	return age;
}
char Patient::getGender() const
{
	return gender;
}
string Patient::getContact() const
{
	return contact;
}
double Patient::getBalance() const
{
	return balance;
}
void Patient::setBalance(double b)
{
	balance = b;
}
Patient& Patient::operator+=(double amount)
{
	balance = balance + amount;
	return *this;
}
Patient& Patient::operator-=(double amount)
{
	balance = balance - amount;
	return *this;
}
bool Patient::operator==(const Patient& other) const
{
	return (ID == other.ID);
}
ostream& operator<<(ostream& out, const Patient& a)
{
	out << "ID: " << a.ID << endl;
	out << "Name: " << a.name << endl;
	out << "Age: " << a.age << endl;
	out << "Gender: " << a.gender << endl;
	out << "Contact: " << a.contact << endl;
	out << "Password: " << a.password << endl;
	out << "Balance: " << a.balance << endl;
	return out;
}
void Patient::display()
{
	cout << "ID: " << ID << endl;
	cout << "Name: " << name << endl;
	cout << "Age: " << age << endl;
	cout << "Gender: " << gender << endl;
	cout << "Contact: " << contact << endl;
	cout << "Password: " << password << endl;
	cout << "Balance: " << balance << endl;
}
void Patient::displayMenu()
{
	cout << "Welcome, " << name << endl;
	cout << "Balance: PKR " << balance << endl;
	cout << "========================================\n";
	cout << "1. Book Appointment\n";
	cout << "2. Cancel Appointment\n";
	cout << "3. View My Appointments\n";
	cout << " 4. View My Medical Records\n";
	cout << " 5. View My Bills\n";
	cout << " 6. Pay Bill\n";
	cout << " 7. Top Up Balance\n";
	cout << " 8. Logout\n";
}