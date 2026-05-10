#pragma once
#include <iostream>
#include <string>
using namespace std;

class Bill
{
    friend ostream& operator<<(ostream& out, const Bill& b);
private:
    int bill_id;
    int patient_id;
    int appointment_id;
    double amount;
    string status;
    string date;
public:
    Bill();
    Bill(int b_id, int p_id, int a_id, double amt, string stat, string d);

    int getID() const;
    int getp_id() const;
    int geta_id() const;
    double getAmount() const;
    string getStatus() const;
    string getDate() const;
    void setStatus(string s);
};