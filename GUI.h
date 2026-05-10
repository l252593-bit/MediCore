#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <string>
#include "Storage.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "FileHandler.h"
#include "Validator.h"
using namespace std;

class GUI {
private:
    sf::RenderWindow window;
    sf::Font font;

    // Colors
    sf::Color bgColor;
    sf::Color buttonColor;
    sf::Color buttonHoverColor;
    sf::Color buttonTextColor;
    sf::Color titleColor;
    sf::Color errorColor;
    sf::Color successColor;

    // Data references
    Storage<Patient>* patients;
    Storage<Doctor>* doctors;
    Storage<Appointment>* appointments;
    Storage<Bill>* bills;
    Storage<Prescription>* prescriptions;
    Admin* admin;
    FileHandler* fh;
    Validator* validator;

    // Helper drawing functions
    sf::Text makeText(string str, int size, sf::Color color, float x, float y);
    sf::RectangleShape makeButton(float x, float y, float w, float h, sf::Color color);
    bool isMouseOver(sf::RectangleShape& btn, sf::RenderWindow& win);
    string getTextInput(string prompt, bool isPassword = false);
    int showMenu(string title, string options[], int count, string subtitle = "");
    void showMessage(string message, bool isError = false);
    void drawBackground();

    // Login
    int showRoleSelection();
    bool loginPatient(int& outID);
    bool loginDoctor(int& outID);
    bool loginAdmin();

    // Patient screens
    void runPatientMenu(Patient& p);
    void bookAppointment(Patient& p);
    void cancelAppointment(Patient& p);
    void viewMyAppointments(Patient& p);
    void viewMyMedicalRecords(Patient& p);
    void viewMyBills(Patient& p);
    void payBill(Patient& p);
    void topUpBalance(Patient& p);

    // Doctor screens
    void runDoctorMenu(Doctor& d);
    void viewTodaysAppointments(Doctor& d);
    void markAppointmentComplete(Doctor& d);
    void markAppointmentNoShow(Doctor& d);
    void writePrescription(Doctor& d);
    void viewPatientMedicalHistory(Doctor& d);

    // Admin screens
    void runAdminMenu();
    void addDoctor();
    void removeDoctor();
    void viewAllPatients();
    void viewAllDoctors();
    void viewAllAppointments();
    void viewUnpaidBills();
    void dischargePatient();
    void viewSecurityLog();
    void generateDailyReport();

    // Utility
    string getCurrentDate();
    int getMaxAppointmentID();
    int getMaxBillID();
    int getMaxPrescriptionID();
    string convertDateForComparison(string date);

public:
    GUI(Storage<Patient>* p, Storage<Doctor>* d, Storage<Appointment>* a,
        Storage<Bill>* b, Storage<Prescription>* pr, Admin* adm,
        FileHandler* fh, Validator* v);
    void run();
};