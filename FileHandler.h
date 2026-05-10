#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "Storage.h"
#include "HospitalException.h"
using namespace std;

class FileHandler
{
public:
    // Load functions
    void loadPatients(Storage<Patient>& storage);
    void loadDoctors(Storage<Doctor>& storage);
    void loadAdmin(Admin& admin);
    void loadAppointments(Storage<Appointment>& storage);
    void loadBills(Storage<Bill>& storage);
    void loadPrescriptions(Storage<Prescription>& storage);

    // Append functions
    void appendPatient(Patient& p);
    void appendDoctor(Doctor& d);
    void appendAppointment(Appointment& a);
    void appendBill(Bill& b);
    void appendPrescription(Prescription& p);

    // Update functions
    void updatePatient(Patient& p);
    void updateDoctor(Doctor& d);
    void updateAppointment(Appointment& a);
    void updateBill(Bill& b);

    // Delete functions
    void deletePatient(int id);
    void deleteDoctor(int id);
    void deleteAppointment(int id);
    void deleteBill(int id);
    void deletePrescription(int id);
    void deleteAppointmentsByPatientID(int patientID);
    void deleteBillsByPatientID(int patientID);
    void deletePrescriptionsByPatientID(int patientID);

    // Special functions
    void appendSecurityLog(string timestamp, string role, string enteredID, string result);
    void dischargePatient(int id);

private:
    // helper to parse a single line into fields
    void parseLine(string line, string* fields, int& fieldCount);
};