#pragma once
#include<iostream>
#include<string>
using namespace std;

class Appointment
{
	friend ostream& operator<<(ostream& out, const Appointment& a);
private:
	int appointment_id;
	int patient_id;
	int doctor_id;
	string date;
	string time_slot;
	string status;
public:
	Appointment();
	Appointment(int a_id, int p_id, int d_id, string date, string time, string stat);
	int getID()const;
	int getp_id() const;
	int getd_id() const;
	string getDate() const;
	string getTime() const;
	string getStatus() const;
	void setStatus(string s);
	bool operator==(const Appointment& other) const;
};
