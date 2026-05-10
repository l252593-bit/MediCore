#include"Appointment.h"
using namespace std;

Appointment::Appointment()
{
	appointment_id = 0;
	patient_id = 0;
	doctor_id = 0;
	date = "0";
	time_slot = "0";
	status = "X";
}
Appointment::Appointment(int a_id, int p_id, int d_id, string date, string time, string stat)
{
	appointment_id = a_id;
	patient_id = p_id;
	doctor_id = d_id;
	this->date = date;
	time_slot = time;
	status = stat;
}
int Appointment::getID() const
{
	return appointment_id;
}
int Appointment::getp_id() const
{
	return patient_id;
}
int Appointment::getd_id() const
{
	return doctor_id;
}
string Appointment::getDate() const
{
	return date;
}
string Appointment::getTime() const
{
	return time_slot;
}
string Appointment::getStatus() const
{
	return status;
}
void Appointment::setStatus(string s)
{
	status = s;
}
bool Appointment::operator==(const Appointment& other) const
{
	if (status == "cancelled" || other.status == "cancelled")
	{
		return false;
	}
	if (date==other.date && doctor_id == other.doctor_id && time_slot == other.time_slot)
	{
		return true;
	}
	else
	{
		return false;
	}
}
ostream& operator<<(ostream& out, const Appointment& a)
{
	out << "Appointment ID: " << a.appointment_id << endl;
	out << "Patient ID: " << a.patient_id << endl;
	out << "Doctor ID: " << a.doctor_id << endl;
	out << "Date: " << a.date << endl;
	out << "Time Slot: " << a.time_slot << endl;
	out << "Status: " << a.status << endl;
	return out;
}