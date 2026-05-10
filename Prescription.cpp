#include"Prescription.h"
using namespace std;

Prescription::Prescription()
{
	prescription_id = 0;
	appointment_id = 0;
	patient_id = 0;
	doctor_id = 0;
	date = "0";
	medicines = "Unknown";
	notes = "X";
}
Prescription::Prescription(int pres_id, int a_id, int p_id, int d_id, string d, string meds, string ns)
{
	prescription_id = pres_id;
	appointment_id = a_id;
	patient_id = p_id;
	doctor_id = d_id;
	date = d;
	medicines = meds;
	notes = ns;
}
int Prescription::getID() const
{
	return prescription_id;
}
int Prescription::geta_id() const
{
	return appointment_id;
}
int Prescription::getp_id() const
{
	return patient_id;
}
int Prescription::getd_id() const
{
	return doctor_id;
}
string Prescription::getDate() const
{
	return date;
}
string Prescription::getMedicines() const
{
	return medicines;
}
string Prescription::getNotes() const
{
	return notes;
}
ostream& operator<<(ostream& out, const Prescription& pres)
{
	out << "Prescription ID: " << pres.prescription_id << endl;
	out << "Appointment ID: " << pres.appointment_id << endl;
	out << "Patient ID: " << pres.patient_id << endl;
	out << "Doctor ID: " << pres.doctor_id << endl;
	out << "Date: " << pres.date << endl;
	out << "Medicines: " << pres.medicines << endl;
	out << "Doctor's Notes: " << pres.notes << endl;
	return out;
}