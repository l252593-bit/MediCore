#pragma once
#include<iostream>
#include<string>
using namespace std;

class Prescription
{
	friend ostream& operator<<(ostream& out, const Prescription& pres);
private:
	int prescription_id;
	int appointment_id;
	int patient_id;
	int doctor_id;
	string date;
	string medicines;
	string notes;
public:
	Prescription();
	Prescription(int pres_id, int a_id, int p_id, int d_id, string d, string meds, string ns);
	int getID() const;
	int geta_id()const;
	int getp_id() const;
	int getd_id() const;
	string getDate() const;
	string getMedicines() const;
	string getNotes() const;
};