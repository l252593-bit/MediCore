#include "Bill.h"
using namespace std;

Bill::Bill()
{
    bill_id = 0;
    patient_id = 0;
    appointment_id = 0;
    amount = 0.0;
    status = "X";
    date = "0";
}

Bill::Bill(int b_id, int p_id, int a_id, double amt, string stat, string d)
{
    bill_id = b_id;
    patient_id = p_id;
    appointment_id = a_id;
    amount = amt;
    status = stat;
    date = d;
}

int Bill::getID() const
{
    return bill_id;
}
int Bill::getp_id() const
{
    return patient_id;
}
int Bill::geta_id() const
{
    return appointment_id;
}
double Bill::getAmount() const
{
    return amount;
}
string Bill::getStatus() const
{
    return status;
}
string Bill::getDate() const
{
    return date;
}

void Bill::setStatus(string s)
{
    status = s;
}

ostream& operator<<(ostream& out, const Bill& b)
{
    out << "Bill ID: " << b.bill_id << endl;
    out << "Patient ID: " << b.patient_id << endl;
    out << "Appointment ID: " << b.appointment_id << endl;
    out << "Amount: PKR " << b.amount << endl;
    out << "Status: " << b.status << endl;
    out << "Date: " << b.date << endl;
    return out;
}