#include <iostream>
#include <string>
#include <ctime>
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "Storage.h"
#include "FileHandler.h"
#include "Validator.h"
#include "HospitalException.h"
#include"GUI.h"
using namespace std;

// ===================== UTILITY FUNCTIONS =====================

string getCurrentDate() {
    time_t t = time(0);
    tm now;
    localtime_s(&now, &t);
    char buf[11];
    sprintf_s(buf, "%02d-%02d-%04d", now.tm_mday, now.tm_mon + 1, now.tm_year + 1900);
    return string(buf);
}

string getCurrentTimestamp() {
    time_t t = time(0);
    tm now;
    localtime_s(&now, &t);
    char buf[30];
    sprintf_s(buf, "%02d-%02d-%04d %02d:%02d:%02d",
        now.tm_mday, now.tm_mon + 1, now.tm_year + 1900,
        now.tm_hour, now.tm_min, now.tm_sec);
    return string(buf);
}

int getMaxID(Storage<Appointment>& appointments) {
    int maxID = 0;
    for (int i = 0; i < appointments.size(); i++) {
        if (appointments.getAll()[i].getID() > maxID)
            maxID = appointments.getAll()[i].getID();
    }
    return maxID;
}

int getMaxBillID(Storage<Bill>& bills) {
    int maxID = 0;
    for (int i = 0; i < bills.size(); i++) {
        if (bills.getAll()[i].getID() > maxID)
            maxID = bills.getAll()[i].getID();
    }
    return maxID;
}

int getMaxPrescriptionID(Storage<Prescription>& prescriptions) {
    int maxID = 0;
    for (int i = 0; i < prescriptions.size(); i++) {
        if (prescriptions.getAll()[i].getID() > maxID)
            maxID = prescriptions.getAll()[i].getID();
    }
    return maxID;
}

string convertDateForComparison(string date) {
    string year = "", month = "", day = "";
    // DD-MM-YYYY
    day += date[0]; day += date[1];
    month += date[3]; month += date[4];
    year += date[6]; year += date[7];
    year += date[8]; year += date[9];
    return year + "-" + month + "-" + day;
}

void sortAppointmentsByDateAsc(Appointment* arr, int count) 
{
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (convertDateForComparison(arr[j].getDate()) >
                convertDateForComparison(arr[j + 1].getDate())) {
                Appointment temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void sortAppointmentsByDateDesc(Appointment* arr, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (convertDateForComparison(arr[j].getDate()) <
                convertDateForComparison(arr[j + 1].getDate())) {
                Appointment temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void sortPrescriptionsByDateDesc(Prescription* arr, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (convertDateForComparison(arr[j].getDate()) <
                convertDateForComparison(arr[j + 1].getDate())) {
                Prescription temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// ===================== FORWARD DECLARATIONS =====================
void showLoginScreen(Storage<Patient>&, Storage<Doctor>&, Storage<Appointment>&, Storage<Bill>&, Storage<Prescription>&, Admin&, FileHandler&, Validator&);
void patientMenu(Patient&, Storage<Doctor>&, Storage<Appointment>&, Storage<Bill>&, Storage<Prescription>&, FileHandler&, Validator&);
void doctorMenu(Doctor&, Storage<Patient>&, Storage<Appointment>&, Storage<Bill>&, Storage<Prescription>&, FileHandler&, Validator&);
void adminMenu(Admin&, Storage<Patient>&, Storage<Doctor>&, Storage<Appointment>&, Storage<Bill>&, Storage<Prescription>&, FileHandler&, Validator&);

// Patient menu functions
void bookAppointment(Patient&, Storage<Doctor>&, Storage<Appointment>&, Storage<Bill>&, FileHandler&, Validator&);
void cancelAppointment(Patient&, Storage<Doctor>&, Storage<Appointment>&, Storage<Bill>&, FileHandler&);
void viewMyAppointments(Patient&, Storage<Doctor>&, Storage<Appointment>&);
void viewMyMedicalRecords(Patient&, Storage<Doctor>&, Storage<Appointment>&, Storage<Prescription>&);
void viewMyBills(Patient&, Storage<Bill>&);
void payBill(Patient&, Storage<Bill>&, FileHandler&);
void topUpBalance(Patient&, FileHandler&, Validator&);

// Doctor menu functions
void viewTodaysAppointments(Doctor&, Storage<Patient>&, Storage<Appointment>&);
void markAppointmentComplete(Doctor&, Storage<Appointment>&, FileHandler&);
void markAppointmentNoShow(Doctor&, Storage<Appointment>&, Storage<Bill>&, FileHandler&);
void writePrescription(Doctor&, Storage<Appointment>&, Storage<Prescription>&, FileHandler&);
void viewPatientMedicalHistory(Doctor&, Storage<Patient>&, Storage<Appointment>&, Storage<Prescription>&);

// Admin menu functions
void addDoctor(Storage<Doctor>&, FileHandler&, Validator&);
void removeDoctor(Storage<Doctor>&, Storage<Appointment>&, FileHandler&);
void viewAllPatients(Storage<Patient>&, Storage<Bill>&);
void viewAllDoctors(Storage<Doctor>&);
void viewAllAppointments(Storage<Patient>&, Storage<Doctor>&, Storage<Appointment>&);
void viewUnpaidBills(Storage<Patient>&, Storage<Bill>&);
void dischargePatient(Storage<Patient>&, Storage<Appointment>&, Storage<Bill>&, Storage<Prescription>&, FileHandler&);
void viewSecurityLog();
void generateDailyReport(Storage<Patient>&, Storage<Doctor>&, Storage<Appointment>&, Storage<Bill>&);

// ===================== MAIN =====================
int main() {
    Storage<Patient>* patients = new Storage<Patient>();
    Storage<Doctor>* doctors = new Storage<Doctor>();
    Storage<Appointment>* appointments = new Storage<Appointment>();
    Storage<Bill>* bills = new Storage<Bill>();
    Storage<Prescription>* prescriptions = new Storage<Prescription>();
    Admin admin;
    FileHandler fh;
    Validator validator;

    try {
        fh.loadPatients(*patients);
        fh.loadDoctors(*doctors);
        fh.loadAdmin(admin);
        fh.loadAppointments(*appointments);
        fh.loadBills(*bills);
        fh.loadPrescriptions(*prescriptions);
    }
    catch (FileNotFoundException& e) {
        cout << e.what() << endl;
        cout << "Some files could not be loaded. Starting fresh." << endl;
    }

    GUI gui(patients, doctors, appointments, bills, prescriptions, &admin, &fh, &validator);
    gui.run();
    delete patients;
    delete doctors;
    delete appointments;
    delete bills;
    delete prescriptions;


    return 0;
}

// ===================== LOGIN =====================
void showLoginScreen(Storage<Patient>& patients, Storage<Doctor>& doctors, Storage<Appointment>& appointments, Storage<Bill>& bills, Storage<Prescription>& prescriptions, Admin& admin, FileHandler& fh, Validator& validator) {
    int choice;
    do {
        cout << "\nWelcome to MediCore Hospital Management System" << endl;
        cout << "===============================================" << endl;
        cout << "Login as:" << endl;
        cout << "1. Patient" << endl;
        cout << "2. Doctor" << endl;
        cout << "3. Admin" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            int id;
            string password;
            int attempts = 0;
            bool loggedIn = false;

            while (attempts < 3 && !loggedIn) {
                cout << "Enter Patient ID: ";
                cin >> id;
                cout << "Enter Password: ";
                cin >> password;

                Patient* p = patients.findByID(id);
                if (p != nullptr && p->getPassword() == password) {
                    loggedIn = true;
                    patientMenu(*p, doctors, appointments, bills, prescriptions, fh, validator);
                }
                else {
                    attempts++;
                    fh.appendSecurityLog(getCurrentTimestamp(), "Patient", to_string(id), "FAILED");
                    if (attempts < 3)
                        cout << "Invalid ID or password. " << 3 - attempts << " attempt(s) remaining." << endl;
                }
            }
            if (!loggedIn) {
                cout << "Account locked. Contact admin." << endl;
            }
        }
        else if (choice == 2) {
            int id;
            string password;
            int attempts = 0;
            bool loggedIn = false;

            while (attempts < 3 && !loggedIn) {
                cout << "Enter Doctor ID: ";
                cin >> id;
                cout << "Enter Password: ";
                cin >> password;

                Doctor* d = doctors.findByID(id);
                if (d != nullptr && d->getPassword() == password) {
                    loggedIn = true;
                    doctorMenu(*d, patients, appointments, bills, prescriptions, fh, validator);
                }
                else {
                    attempts++;
                    fh.appendSecurityLog(getCurrentTimestamp(), "Doctor", to_string(id), "FAILED");
                    if (attempts < 3)
                        cout << "Invalid ID or password. " << 3 - attempts << " attempt(s) remaining." << endl;
                }
            }
            if (!loggedIn) {
                cout << "Account locked. Contact admin." << endl;
            }
        }
        else if (choice == 3) {
            int id;
            string password;
            int attempts = 0;
            bool loggedIn = false;

            while (attempts < 3 && !loggedIn) {
                cout << "Enter Admin ID: ";
                cin >> id;
                cout << "Enter Password: ";
                cin >> password;

                if (admin.getID() == id && admin.getPassword() == password) {
                    loggedIn = true;
                    adminMenu(admin, patients, doctors, appointments, bills, prescriptions, fh, validator);
                }
                else {
                    attempts++;
                    fh.appendSecurityLog(getCurrentTimestamp(), "Admin", to_string(id), "FAILED");
                    if (attempts < 3)
                        cout << "Invalid ID or password. " << 3 - attempts << " attempt(s) remaining." << endl;
                }
            }
            if (!loggedIn) {
                cout << "Account locked. Contact admin." << endl;
            }
        }
        else if (choice != 4) {
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 4);

    cout << "Thank you for using MediCore. Goodbye!" << endl;
}

// ===================== PATIENT MENU =====================
void patientMenu(Patient& p, Storage<Doctor>& doctors, Storage<Appointment>& appointments, Storage<Bill>& bills, Storage<Prescription>& prescriptions, FileHandler& fh, Validator& validator) {
    int choice;
    do {
        p.displayMenu();
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1)
            bookAppointment(p, doctors, appointments, bills, fh, validator);
        else if (choice == 2)
            cancelAppointment(p, doctors, appointments, bills, fh);
        else if (choice == 3)
            viewMyAppointments(p, doctors, appointments);
        else if (choice == 4)
            viewMyMedicalRecords(p, doctors, appointments, prescriptions);
        else if (choice == 5)
            viewMyBills(p, bills);
        else if (choice == 6)
            payBill(p, bills, fh);
        else if (choice == 7)
            topUpBalance(p, fh, validator);
        else if (choice == 8)
            cout << "Logging out..." << endl;
        else
            cout << "Invalid choice. Please try again." << endl;
    } while (choice != 8);
}

// ===================== BOOK APPOINTMENT =====================
void bookAppointment(Patient& p, Storage<Doctor>& doctors, Storage<Appointment>& appointments, Storage<Bill>& bills, FileHandler& fh, Validator& validator) {
    string spec;
    cout << "Enter specialization to search: ";
    cin.ignore();
    getline(cin, spec);

    // convert to lowercase manually
    string specLower = spec;
    for (int i = 0; specLower[i] != '\0'; i++) {
        if (specLower[i] >= 'A' && specLower[i] <= 'Z')
            specLower[i] = specLower[i] + 32;
    }

    bool found = false;
    for (int i = 0; i < doctors.size(); i++) {
        string docSpec = doctors.getAll()[i].getSpecialization();
        string docSpecLower = docSpec;
        for (int j = 0; docSpecLower[j] != '\0'; j++) {
            if (docSpecLower[j] >= 'A' && docSpecLower[j] <= 'Z')
                docSpecLower[j] = docSpecLower[j] + 32;
        }
        if (docSpecLower == specLower) {
            cout << "ID: " << doctors.getAll()[i].getID()
                << " | Name: " << doctors.getAll()[i].getName()
                << " | Fee: PKR " << doctors.getAll()[i].getFee() << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No doctors available for that specialization." << endl;
        return;
    }

    int docID;
    cout << "Enter Doctor ID: ";
    cin >> docID;
    Doctor* doc = doctors.findByID(docID);
    if (doc == nullptr) {
        cout << "Doctor not found." << endl;
        return;
    }

    string date;
    int dateAttempts = 0;
    bool validDate = false;
    while (dateAttempts < 3 && !validDate) {
        cout << "Enter date (DD-MM-YYYY): ";
        cin >> date;
        if (validator.validateDate(date)) {
            validDate = true;
        }
        else {
            cout << "Invalid date. Use format DD-MM-YYYY." << endl;
            dateAttempts++;
        }
    }
    if (!validDate) return;

    // show available slots
    string slots[8] = { "09:00","10:00","11:00","12:00","13:00","14:00","15:00","16:00" };
    cout << "Available time slots:" << endl;
    for (int i = 0; i < 8; i++) {
        bool taken = false;
        for (int j = 0; j < appointments.size(); j++) {
            Appointment temp(0, 0, docID, date, slots[i], "pending");
            if (appointments.getAll()[j] == temp) {
                taken = true;
                break;
            }
        }
        if (!taken)
            cout << slots[i] << endl;
    }

    string slot;
    bool booked = false;
    while (!booked) {
        cout << "Enter time slot (e.g. 09:00): ";
        cin >> slot;
        if (!validator.validateTimeSlot(slot)) {
            cout << "Invalid time slot." << endl;
            continue;
        }
        try {
            bool slotTaken = false;
            for (int j = 0; j < appointments.size(); j++) {
                Appointment temp(0, 0, docID, date, slot, "pending");
                if (appointments.getAll()[j] == temp) {
                    slotTaken = true;
                    break;
                }
            }
            if (slotTaken)
                throw SlotUnavailableException("This slot is already taken. Please choose another.");

            // check balance
            if (p.getBalance() < doc->getFee())
                throw InsufficientFundsException("Insufficient balance. Please top up.");

            // deduct fee
            p -= doc->getFee();

            // create appointment
            int newID = getMaxID(appointments) + 1;
            Appointment newApp(newID, p.getID(), docID, date, slot, "pending");
            appointments.add(newApp);
            fh.appendAppointment(newApp);

            // create bill
            int newBillID = getMaxBillID(bills) + 1;
            Bill newBill(newBillID, p.getID(), newID, doc->getFee(), "unpaid", date);
            bills.add(newBill);
            fh.appendBill(newBill);

            // update patient balance in file
            fh.updatePatient(p);

            cout << "Appointment booked successfully. Appointment ID: " << newID << endl;
            booked = true;
        }
        catch (SlotUnavailableException& e) {
            cout << e.what() << endl;
            // re-display available slots
            cout << "Available time slots:" << endl;
            for (int i = 0; i < 8; i++) {
                bool taken = false;
                for (int j = 0; j < appointments.size(); j++) {
                    Appointment temp(0, 0, docID, date, slots[i], "pending");
                    if (appointments.getAll()[j] == temp) {
                        taken = true;
                        break;
                    }
                }
                if (!taken)
                    cout << slots[i] << endl;
            }
        }
        catch (InsufficientFundsException& e) {
            cout << e.what() << endl;
            return;
        }
    }
}

// ===================== CANCEL APPOINTMENT =====================
void cancelAppointment(Patient& p, Storage<Doctor>& doctors, Storage<Appointment>& appointments, Storage<Bill>& bills, FileHandler& fh) {
    bool hasPending = false;
    for (int i = 0; i < appointments.size(); i++) {
        if (appointments.getAll()[i].getp_id() == p.getID() &&
            appointments.getAll()[i].getStatus() == "pending") {
            Doctor* doc = doctors.findByID(appointments.getAll()[i].getd_id());
            string docName = doc ? doc->getName() : "Unknown";
            cout << "ID: " << appointments.getAll()[i].getID()
                << " | Doctor: " << docName
                << " | Date: " << appointments.getAll()[i].getDate()
                << " | Time: " << appointments.getAll()[i].getTime() << endl;
            hasPending = true;
        }
    }

    if (!hasPending) {
        cout << "You have no pending appointments." << endl;
        return;
    }

    int appID;
    cout << "Enter Appointment ID to cancel: ";
    cin >> appID;

    Appointment* app = appointments.findByID(appID);
    if (app == nullptr || app->getp_id() != p.getID() || app->getStatus() != "pending") {
        cout << "Invalid appointment ID." << endl;
        return;
    }

    Doctor* doc = doctors.findByID(app->getd_id());
    double fee = doc ? doc->getFee() : 0;

    app->setStatus("cancelled");
    fh.updateAppointment(*app);

    // refund
    p += fee;
    fh.updatePatient(p);

    // update bill
    for (int i = 0; i < bills.size(); i++) {
        if (bills.getAll()[i].geta_id() == appID) {
            bills.getAll()[i].setStatus("cancelled");
            fh.updateBill(bills.getAll()[i]);
            break;
        }
    }

    cout << "Appointment cancelled. PKR " << fee << " refunded to your balance." << endl;
}

// ===================== VIEW MY APPOINTMENTS =====================
void viewMyAppointments(Patient& p, Storage<Doctor>& doctors, Storage<Appointment>& appointments) {
    Appointment myApps[100];
    int count = 0;

    for (int i = 0; i < appointments.size(); i++) {
        if (appointments.getAll()[i].getp_id() == p.getID())
            myApps[count++] = appointments.getAll()[i];
    }

    if (count == 0) {
        cout << "No appointments found." << endl;
        return;
    }

    sortAppointmentsByDateAsc(myApps, count);

    for (int i = 0; i < count; i++) {
        Doctor* doc = doctors.findByID(myApps[i].getd_id());
        string docName = doc ? doc->getName() : "Unknown";
        string docSpec = doc ? doc->getSpecialization() : "Unknown";
        cout << "ID: " << myApps[i].getID()
            << " | Doctor: " << docName
            << " | Specialization: " << docSpec
            << " | Date: " << myApps[i].getDate()
            << " | Time: " << myApps[i].getTime()
            << " | Status: " << myApps[i].getStatus() << endl;
    }
}

// ===================== VIEW MY MEDICAL RECORDS =====================
void viewMyMedicalRecords(Patient& p, Storage<Doctor>& doctors, Storage<Appointment>& appointments, Storage<Prescription>& prescriptions) {
    Prescription myPres[100];
    int count = 0;

    for (int i = 0; i < prescriptions.size(); i++) {
        if (prescriptions.getAll()[i].getp_id() == p.getID())
            myPres[count++] = prescriptions.getAll()[i];
    }

    if (count == 0) {
        cout << "No medical records found." << endl;
        return;
    }

    sortPrescriptionsByDateDesc(myPres, count);

    for (int i = 0; i < count; i++) {
        Doctor* doc = doctors.findByID(myPres[i].getd_id());
        string docName = doc ? doc->getName() : "Unknown";
        cout << "Date: " << myPres[i].getDate()
            << " | Doctor: " << docName
            << " | Medicines: " << myPres[i].getMedicines()
            << " | Notes: " << myPres[i].getNotes() << endl;
    }
}

// ===================== VIEW MY BILLS =====================
void viewMyBills(Patient& p, Storage<Bill>& bills) {
    bool found = false;
    double totalUnpaid = 0;

    for (int i = 0; i < bills.size(); i++) {
        if (bills.getAll()[i].getp_id() == p.getID()) {
            cout << "Bill ID: " << bills.getAll()[i].getID()
                << " | Appointment ID: " << bills.getAll()[i].geta_id()
                << " | Amount: PKR " << bills.getAll()[i].getAmount()
                << " | Status: " << bills.getAll()[i].getStatus()
                << " | Date: " << bills.getAll()[i].getDate() << endl;
            if (bills.getAll()[i].getStatus() == "unpaid")
                totalUnpaid += bills.getAll()[i].getAmount();
            found = true;
        }
    }

    if (!found) {
        cout << "No bills found." << endl;
        return;
    }

    cout << "Total outstanding unpaid amount: PKR " << totalUnpaid << endl;
}

// ===================== PAY BILL =====================
void payBill(Patient& p, Storage<Bill>& bills, FileHandler& fh) {
    bool hasUnpaid = false;
    for (int i = 0; i < bills.size(); i++) {
        if (bills.getAll()[i].getp_id() == p.getID() &&
            bills.getAll()[i].getStatus() == "unpaid") {
            cout << "Bill ID: " << bills.getAll()[i].getID()
                << " | Amount: PKR " << bills.getAll()[i].getAmount()
                << " | Date: " << bills.getAll()[i].getDate() << endl;
            hasUnpaid = true;
        }
    }

    if (!hasUnpaid) {
        cout << "No unpaid bills." << endl;
        return;
    }

    int billID;
    cout << "Enter Bill ID to pay: ";
    cin >> billID;

    Bill* bill = bills.findByID(billID);
    if (bill == nullptr || bill->getp_id() != p.getID() || bill->getStatus() != "unpaid") {
        cout << "Invalid bill ID." << endl;
        return;
    }

    try {
        if (p.getBalance() < bill->getAmount())
            throw InsufficientFundsException("Insufficient balance to pay this bill.");

        p -= bill->getAmount();
        bill->setStatus("paid");
        fh.updateBill(*bill);
        fh.updatePatient(p);

        cout << "Bill paid successfully. Remaining balance: PKR " << p.getBalance() << endl;
    }
    catch (InsufficientFundsException& e) {
        cout << e.what() << endl;
    }
}

// ===================== TOP UP BALANCE =====================
void topUpBalance(Patient& p, FileHandler& fh, Validator& validator) {
    int attempts = 0;
    while (attempts < 3) {
        double amount;
        cout << "Enter amount to add (PKR): ";
        cin >> amount;
        try {
            if (!validator.validatePositiveFloat(amount))
                throw InvalidInputException("Amount must be greater than 0.");

            p += amount;
            fh.updatePatient(p);
            cout << "Balance updated. New balance: PKR " << p.getBalance() << endl;
            return;
        }
        catch (InvalidInputException& e) {
            cout << e.what() << endl;
            attempts++;
        }
    }
}

// ===================== DOCTOR MENU =====================
void doctorMenu(Doctor& d, Storage<Patient>& patients, Storage<Appointment>& appointments, Storage<Bill>& bills, Storage<Prescription>& prescriptions, FileHandler& fh, Validator& validator) {
    int choice;
    do {
        d.displayMenu();
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1)
            viewTodaysAppointments(d, patients, appointments);
        else if (choice == 2)
            markAppointmentComplete(d, appointments, fh);
        else if (choice == 3)
            markAppointmentNoShow(d, appointments, bills, fh);
        else if (choice == 4)
            writePrescription(d, appointments, prescriptions, fh);
        else if (choice == 5)
            viewPatientMedicalHistory(d, patients, appointments, prescriptions);
        else if (choice == 6)
            cout << "Logging out..." << endl;
        else
            cout << "Invalid choice. Please try again." << endl;
    } while (choice != 6);
}

// ===================== VIEW TODAY'S APPOINTMENTS =====================
void viewTodaysAppointments(Doctor& d, Storage<Patient>& patients, Storage<Appointment>& appointments) {
    string today = getCurrentDate();
    Appointment todayApps[100];
    int count = 0;

    for (int i = 0; i < appointments.size(); i++) {
        if (appointments.getAll()[i].getd_id() == d.getID() &&
            appointments.getAll()[i].getDate() == today)
            todayApps[count++] = appointments.getAll()[i];
    }

    if (count == 0) {
        cout << "No appointments scheduled for today." << endl;
        return;
    }

    // sort by time slot ascending
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (todayApps[j].getTime() > todayApps[j + 1].getTime()) {
                Appointment temp = todayApps[j];
                todayApps[j] = todayApps[j + 1];
                todayApps[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < count; i++) {
        Patient* pat = patients.findByID(todayApps[i].getp_id());
        string patName = pat ? pat->getName() : "Unknown";
        cout << "ID: " << todayApps[i].getID()
            << " | Patient: " << patName
            << " | Time: " << todayApps[i].getTime()
            << " | Status: " << todayApps[i].getStatus() << endl;
    }
}

// ===================== MARK APPOINTMENT COMPLETE =====================
void markAppointmentComplete(Doctor& d, Storage<Appointment>& appointments, FileHandler& fh) {
    string today = getCurrentDate();
    bool hasPending = false;

    for (int i = 0; i < appointments.size(); i++) {
        if (appointments.getAll()[i].getd_id() == d.getID() &&
            appointments.getAll()[i].getStatus() == "pending" &&
            appointments.getAll()[i].getDate() == today) {
            cout << "ID: " << appointments.getAll()[i].getID()
                << " | Date: " << appointments.getAll()[i].getDate()
                << " | Time: " << appointments.getAll()[i].getTime() << endl;
            hasPending = true;
        }
    }

    if (!hasPending) {
        cout << "No pending appointments for today." << endl;
        return;
    }

    int appID;
    cout << "Enter Appointment ID: ";
    cin >> appID;

    Appointment* app = appointments.findByID(appID);
    if (app == nullptr || app->getd_id() != d.getID() ||
        app->getStatus() != "pending" || app->getDate() != today) {
        cout << "Invalid appointment ID." << endl;
        return;
    }

    app->setStatus("completed");
    fh.updateAppointment(*app);
    cout << "Appointment marked as completed." << endl;
}

// ===================== MARK APPOINTMENT NO SHOW =====================
void markAppointmentNoShow(Doctor& d, Storage<Appointment>& appointments, Storage<Bill>& bills, FileHandler& fh) {
    string today = getCurrentDate();
    bool hasPending = false;

    for (int i = 0; i < appointments.size(); i++) {
        if (appointments.getAll()[i].getd_id() == d.getID() &&
            appointments.getAll()[i].getStatus() == "pending" &&
            appointments.getAll()[i].getDate() == today) {
            cout << "ID: " << appointments.getAll()[i].getID()
                << " | Date: " << appointments.getAll()[i].getDate()
                << " | Time: " << appointments.getAll()[i].getTime() << endl;
            hasPending = true;
        }
    }

    if (!hasPending) {
        cout << "No pending appointments for today." << endl;
        return;
    }

    int appID;
    cout << "Enter Appointment ID: ";
    cin >> appID;

    Appointment* app = appointments.findByID(appID);
    if (app == nullptr || app->getd_id() != d.getID() ||
        app->getStatus() != "pending" || app->getDate() != today) {
        cout << "Invalid appointment ID." << endl;
        return;
    }

    app->setStatus("no-show");
    fh.updateAppointment(*app);

    for (int i = 0; i < bills.size(); i++) {
        if (bills.getAll()[i].geta_id() == appID) {
            bills.getAll()[i].setStatus("cancelled");
            fh.updateBill(bills.getAll()[i]);
            break;
        }
    }

    cout << "Appointment marked as no-show." << endl;
}

// ===================== WRITE PRESCRIPTION =====================
void writePrescription(Doctor& d, Storage<Appointment>& appointments, Storage<Prescription>& prescriptions, FileHandler& fh) {
    int appID;
    cout << "Enter Appointment ID: ";
    cin >> appID;

    Appointment* app = appointments.findByID(appID);
    if (app == nullptr || app->getd_id() != d.getID() || app->getStatus() != "completed") {
        cout << "Invalid appointment ID or appointment not completed." << endl;
        return;
    }

    for (int i = 0; i < prescriptions.size(); i++) {
        if (prescriptions.getAll()[i].geta_id() == appID) {
            cout << "Prescription already written for this appointment." << endl;
            return;
        }
    }

    cin.ignore();
    string medicines;
    cout << "Enter medicines (e.g. Paracetamol 500mg;Amoxicillin 250mg): ";
    getline(cin, medicines);

    string notes;
    cout << "Enter notes (max 300 chars): ";
    getline(cin, notes);

    if (notes.length() > 300)
        notes = notes.substr(0, 300);

    int newID = getMaxPrescriptionID(prescriptions) + 1;
    Prescription newPres(newID, appID, app->getp_id(), d.getID(), app->getDate(), medicines, notes);
    prescriptions.add(newPres);
    fh.appendPrescription(newPres);

    cout << "Prescription saved." << endl;
}

// ===================== VIEW PATIENT MEDICAL HISTORY =====================
void viewPatientMedicalHistory(Doctor& d, Storage<Patient>& patients, Storage<Appointment>& appointments, Storage<Prescription>& prescriptions) {
    int patID;
    cout << "Enter Patient ID: ";
    cin >> patID;

    Patient* pat = patients.findByID(patID);
    if (pat == nullptr) {
        cout << "Access denied. You can only view records of your own patients." << endl;
        return;
    }

    bool hasCompleted = false;
    for (int i = 0; i < appointments.size(); i++) {
        if (appointments.getAll()[i].getp_id() == patID &&
            appointments.getAll()[i].getd_id() == d.getID() &&
            appointments.getAll()[i].getStatus() == "completed") {
            hasCompleted = true;
            break;
        }
    }

    if (!hasCompleted) {
        cout << "Access denied. You can only view records of your own patients." << endl;
        return;
    }

    Prescription myPres[100];
    int count = 0;
    for (int i = 0; i < prescriptions.size(); i++) {
        if (prescriptions.getAll()[i].getp_id() == patID &&
            prescriptions.getAll()[i].getd_id() == d.getID())
            myPres[count++] = prescriptions.getAll()[i];
    }

    if (count == 0) {
        cout << "No prescriptions found for this patient." << endl;
        return;
    }

    sortPrescriptionsByDateDesc(myPres, count);

    for (int i = 0; i < count; i++) {
        cout << "Date: " << myPres[i].getDate()
            << " | Medicines: " << myPres[i].getMedicines()
            << " | Notes: " << myPres[i].getNotes() << endl;
    }
}

// ===================== ADMIN MENU =====================
void adminMenu(Admin& admin, Storage<Patient>& patients, Storage<Doctor>& doctors, Storage<Appointment>& appointments, Storage<Bill>& bills, Storage<Prescription>& prescriptions, FileHandler& fh, Validator& validator) {
    int choice;
    do {
        admin.displayMenu();
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1)
            addDoctor(doctors, fh, validator);
        else if (choice == 2)
            removeDoctor(doctors, appointments, fh);
        else if (choice == 3)
            viewAllPatients(patients, bills);
        else if (choice == 4)
            viewAllDoctors(doctors);
        else if (choice == 5)
            viewAllAppointments(patients, doctors, appointments);
        else if (choice == 6)
            viewUnpaidBills(patients, bills);
        else if (choice == 7)
            dischargePatient(patients, appointments, bills, prescriptions, fh);
        else if (choice == 8)
            viewSecurityLog();
        else if (choice == 9)
            generateDailyReport(patients, doctors, appointments, bills);
        else if (choice == 10)
            cout << "Logging out..." << endl;
        else
            cout << "Invalid choice. Please try again." << endl;
    } while (choice != 10);
}

// ===================== ADD DOCTOR =====================
void addDoctor(Storage<Doctor>& doctors, FileHandler& fh, Validator& validator) {
    string name, spec, contact, password;
    double fee;

    cin.ignore();
    cout << "Enter name: ";
    getline(cin, name);

    cout << "Enter specialization: ";
    getline(cin, spec);

    bool validContact = false;
    while (!validContact) {
        cout << "Enter contact (11 digits): ";
        cin >> contact;
        if (validator.validateContact(contact))
            validContact = true;
        else
            cout << "Invalid contact. Must be exactly 11 digits." << endl;
    }

    bool validPassword = false;
    while (!validPassword) {
        cout << "Enter password (min 6 chars): ";
        cin >> password;
        if (validator.validatePassword(password))
            validPassword = true;
        else
            cout << "Invalid password. Must be at least 6 characters." << endl;
    }

    bool validFee = false;
    while (!validFee) {
        cout << "Enter consultation fee: ";
        cin >> fee;
        if (validator.validatePositiveFloat(fee))
            validFee = true;
        else
            cout << "Fee must be a positive number." << endl;
    }

    int newID = 1;
    for (int i = 0; i < doctors.size(); i++) {
        if (doctors.getAll()[i].getID() >= newID)
            newID = doctors.getAll()[i].getID() + 1;
    }

    Doctor newDoc(newID, name, password, spec, contact, fee);
    doctors.add(newDoc);
    fh.appendDoctor(newDoc);
    cout << "Doctor added successfully. ID: " << newID << endl;
}

// ===================== REMOVE DOCTOR =====================
void removeDoctor(Storage<Doctor>& doctors, Storage<Appointment>& appointments, FileHandler& fh) {
    for (int i = 0; i < doctors.size(); i++) {
        cout << "ID: " << doctors.getAll()[i].getID()
            << " | Name: " << doctors.getAll()[i].getName()
            << " | Specialization: " << doctors.getAll()[i].getSpecialization()
            << " | Fee: PKR " << doctors.getAll()[i].getFee() << endl;
    }

    int docID;
    cout << "Enter Doctor ID to remove: ";
    cin >> docID;

    for (int i = 0; i < appointments.size(); i++) {
        if (appointments.getAll()[i].getd_id() == docID &&
            appointments.getAll()[i].getStatus() == "pending") {
            cout << "Cannot remove doctor with pending appointments. Cancel or reassign them first." << endl;
            return;
        }
    }

    doctors.removeByID(docID);
    fh.deleteDoctor(docID);
    cout << "Doctor removed." << endl;
}

// ===================== VIEW ALL PATIENTS =====================
void viewAllPatients(Storage<Patient>& patients, Storage<Bill>& bills) {
    for (int i = 0; i < patients.size(); i++) {
        int unpaidCount = 0;
        for (int j = 0; j < bills.size(); j++) {
            if (bills.getAll()[j].getp_id() == patients.getAll()[i].getID() &&
                bills.getAll()[j].getStatus() == "unpaid")
                unpaidCount++;
        }
        cout << "ID: " << patients.getAll()[i].getID()
            << " | Name: " << patients.getAll()[i].getName()
            << " | Age: " << patients.getAll()[i].getAge()
            << " | Gender: " << patients.getAll()[i].getGender()
            << " | Contact: " << patients.getAll()[i].getContact()
            << " | Balance: PKR " << patients.getAll()[i].getBalance()
            << " | Unpaid Bills: " << unpaidCount << endl;
    }
}

// ===================== VIEW ALL DOCTORS =====================
void viewAllDoctors(Storage<Doctor>& doctors) {
    for (int i = 0; i < doctors.size(); i++) {
        cout << "ID: " << doctors.getAll()[i].getID()
            << " | Name: " << doctors.getAll()[i].getName()
            << " | Specialization: " << doctors.getAll()[i].getSpecialization()
            << " | Contact: " << doctors.getAll()[i].getContact()
            << " | Fee: PKR " << doctors.getAll()[i].getFee() << endl;
    }
}

// ===================== VIEW ALL APPOINTMENTS =====================
void viewAllAppointments(Storage<Patient>& patients, Storage<Doctor>& doctors, Storage<Appointment>& appointments) {
    Appointment allApps[100];
    int count = appointments.size();
    for (int i = 0; i < count; i++)
        allApps[i] = appointments.getAll()[i];

    sortAppointmentsByDateDesc(allApps, count);

    for (int i = 0; i < count; i++) {
        Patient* pat = patients.findByID(allApps[i].getp_id());
        Doctor* doc = doctors.findByID(allApps[i].getd_id());
        string patName = pat ? pat->getName() : "Unknown";
        string docName = doc ? doc->getName() : "Unknown";
        cout << "ID: " << allApps[i].getID()
            << " | Patient: " << patName
            << " | Doctor: " << docName
            << " | Date: " << allApps[i].getDate()
            << " | Time: " << allApps[i].getTime()
            << " | Status: " << allApps[i].getStatus() << endl;
    }
}

// ===================== VIEW UNPAID BILLS =====================
void viewUnpaidBills(Storage<Patient>& patients, Storage<Bill>& bills) {
    string today = getCurrentDate();
    bool found = false;

    for (int i = 0; i < bills.size(); i++) {
        if (bills.getAll()[i].getStatus() == "unpaid") {
            Patient* pat = patients.findByID(bills.getAll()[i].getp_id());
            string patName = pat ? pat->getName() : "Unknown";

            string billDate = bills.getAll()[i].getDate();
            string display = billDate;

            // check if overdue (more than 7 days)
            // parse today
            int td = (today[0] - '0') * 10 + (today[1] - '0');
            int tm = (today[3] - '0') * 10 + (today[4] - '0');
            int ty = (today[6] - '0') * 1000 + (today[7] - '0') * 100 + (today[8] - '0') * 10 + (today[9] - '0');
            // parse bill date
            int bd = (billDate[0] - '0') * 10 + (billDate[1] - '0');
            int bm = (billDate[3] - '0') * 10 + (billDate[4] - '0');
            int by = (billDate[6] - '0') * 1000 + (billDate[7] - '0') * 100 + (billDate[8] - '0') * 10 + (billDate[9] - '0');

            struct tm t1 = {}, t2 = {};
            t1.tm_mday = td; t1.tm_mon = tm - 1; t1.tm_year = ty - 1900;
            t2.tm_mday = bd; t2.tm_mon = bm - 1; t2.tm_year = by - 1900;
            time_t time1 = mktime(&t1);
            time_t time2 = mktime(&t2);
            double diff = difftime(time1, time2) / (60 * 60 * 24);
            if (diff > 7)
                display += " [OVERDUE]";

            cout << "Bill ID: " << bills.getAll()[i].getID()
                << " | Patient: " << patName
                << " | Amount: PKR " << bills.getAll()[i].getAmount()
                << " | Date: " << display << endl;
            found = true;
        }
    }

    if (!found)
        cout << "No unpaid bills." << endl;
}

// ===================== DISCHARGE PATIENT =====================
void dischargePatient(Storage<Patient>& patients, Storage<Appointment>& appointments, Storage<Bill>& bills, Storage<Prescription>& prescriptions, FileHandler& fh) {
    int patID;
    cout << "Enter Patient ID: ";
    cin >> patID;

    Patient* pat = patients.findByID(patID);
    if (pat == nullptr) {
        cout << "Patient not found." << endl;
        return;
    }

    for (int i = 0; i < bills.size(); i++) {
        if (bills.getAll()[i].getp_id() == patID &&
            bills.getAll()[i].getStatus() == "unpaid") {
            cout << "Cannot discharge patient with unpaid bills." << endl;
            return;
        }
    }

    for (int i = 0; i < appointments.size(); i++) {
        if (appointments.getAll()[i].getp_id() == patID &&
            appointments.getAll()[i].getStatus() == "pending") {
            cout << "Cannot discharge patient with pending appointments." << endl;
            return;
        }
    }

    fh.dischargePatient(patID);
    patients.removeByID(patID);

    // remove from in-memory storage
    for (int i = 0; i < appointments.size(); i++) {
        if (appointments.getAll()[i].getp_id() == patID)
            appointments.removeByID(appointments.getAll()[i].getID());
    }

    cout << "Patient discharged and archived successfully." << endl;
}

// ===================== VIEW SECURITY LOG =====================
void viewSecurityLog() {
    ifstream file("security_log.txt");
    if (!file.is_open()) {
        cout << "No security events logged." << endl;
        return;
    }

    string line;
    bool empty = true;
    while (getline(file, line)) {
        if (line != "") {
            cout << line << endl;
            empty = false;
        }
    }
    file.close();

    if (empty)
        cout << "No security events logged." << endl;
}

// ===================== GENERATE DAILY REPORT =====================
void generateDailyReport(Storage<Patient>& patients, Storage<Doctor>& doctors, Storage<Appointment>& appointments, Storage<Bill>& bills) {
    string today = getCurrentDate();

    int total = 0, pending = 0, completed = 0, noshow = 0, cancelled = 0;
    double revenue = 0;

    for (int i = 0; i < appointments.size(); i++) {
        if (appointments.getAll()[i].getDate() == today) {
            total++;
            string s = appointments.getAll()[i].getStatus();
            if (s == "pending") pending++;
            else if (s == "completed") completed++;
            else if (s == "no-show") noshow++;
            else if (s == "cancelled") cancelled++;
        }
    }

    for (int i = 0; i < bills.size(); i++) {
        if (bills.getAll()[i].getDate() == today &&
            bills.getAll()[i].getStatus() == "paid")
            revenue += bills.getAll()[i].getAmount();
    }

    cout << "===== Daily Report: " << today << " =====" << endl;
    cout << "Total appointments today: " << total
        << " (Pending: " << pending
        << " Completed: " << completed
        << " No-show: " << noshow
        << " Cancelled: " << cancelled << ")" << endl;
    cout << "Revenue collected today (paid bills): PKR " << revenue << endl;

    cout << "\nPatients with outstanding unpaid bills:" << endl;
    for (int i = 0; i < patients.size(); i++) {
        double owed = 0;
        for (int j = 0; j < bills.size(); j++) {
            if (bills.getAll()[j].getp_id() == patients.getAll()[i].getID() &&
                bills.getAll()[j].getStatus() == "unpaid")
                owed += bills.getAll()[j].getAmount();
        }
        if (owed > 0)
            cout << patients.getAll()[i].getName() << " | Total Owed: PKR " << owed << endl;
    }

    cout << "\nDoctor-wise summary for today:" << endl;
    for (int i = 0; i < doctors.size(); i++) {
        int dc = 0, dp = 0, dn = 0;
        for (int j = 0; j < appointments.size(); j++) {
            if (appointments.getAll()[j].getd_id() == doctors.getAll()[i].getID() &&
                appointments.getAll()[j].getDate() == today) {
                string s = appointments.getAll()[j].getStatus();
                if (s == "completed") dc++;
                else if (s == "pending") dp++;
                else if (s == "no-show") dn++;
            }
        }
        if (dc + dp + dn > 0)
            cout << doctors.getAll()[i].getName()
            << " | Completed: " << dc
            << " | Pending: " << dp
            << " | No-show: " << dn << endl;
    }
}