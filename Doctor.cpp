#include"Doctor.h"
using namespace std;

Doctor::Doctor() :Person()
{
	specialization = "X";
	contact = "0";
	fee = 0;
}
Doctor::Doctor(int id, string n, string pass, string spec,
	string con, double f) :Person(id, n, pass)
{
	specialization = spec;
	contact = con;
	fee = f;
}
string Doctor::getSpecialization() const
{
	return specialization;
}
string Doctor::getContact() const
{
	return contact;
}
double Doctor::getFee() const
{
	return fee;
}
void Doctor::setFee(double f) 
{
	fee = f;
}
bool Doctor::operator==(const Doctor& other) const
{
	return (ID == other.ID);
}
ostream& operator<<(ostream& out, const Doctor& a)
{
	out << "ID: " << a.ID << endl;
	out << "Name: " << a.name << endl;
	out << "Specialization: " << a.specialization << endl;
	out << "Contact: " << a.contact << endl;
	out << "Password: " << a.password << endl;
	out << "Fees: " << a.fee << endl;
	return out;
}
void Doctor::display()
{
	cout << "ID: " << ID << endl;
	cout << "Name: " << name << endl;
	cout << "Specialization: " << specialization << endl;
	cout << "Contact: " << contact << endl;
	cout << "Password: " << password << endl;
	cout << "Fees: " << fee << endl;
}
void Doctor::displayMenu()
{
	cout << "Welcome, Dr. " << name << "  |  Specialization: " << specialization << endl;
	cout << "=================================================================================================\n";
	cout << "1. View Today's Appointments\n";
	cout << "2. Mark Appointment Complete\n";
	cout << "3. Mark Appointment No - Show\n";
	cout << "4. Write Prescription\n";
	cout << "5. View Patient Medical History\n";
	cout << "6. Logout\n";
}

