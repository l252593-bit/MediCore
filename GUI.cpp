#include "GUI.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <fstream>
#include <ctime>
#include <sstream>
using namespace std;

// ===================== CONSTRUCTOR =====================
GUI::GUI(Storage<Patient>* p, Storage<Doctor>* d, Storage<Appointment>* a,
    Storage<Bill>* b, Storage<Prescription>* pr, Admin* adm,
    FileHandler* fh, Validator* v)
{
    this->patients = p;
    this->doctors = d;
    this->appointments = a;
    this->bills = b;
    this->prescriptions = pr;
    this->admin = adm;
    this->fh = fh;
    this->validator = v;

    bgColor = sf::Color(240, 248, 255);
    buttonColor = sf::Color(70, 130, 180);
    buttonHoverColor = sf::Color(100, 160, 210);
    buttonTextColor = sf::Color::White;
    titleColor = sf::Color(30, 80, 140);
    errorColor = sf::Color(200, 50, 50);
    successColor = sf::Color(50, 150, 50);

    window.create(sf::VideoMode(900, 650), "MediCore Hospital Management System");
    window.setFramerateLimit(60);

    if (!font.loadFromFile("arial.ttf")) {
        font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
    }
}

// ===================== HELPER: DRAW BACKGROUND =====================
void GUI::drawBackground() {
    window.clear(bgColor);
    // top bar
    sf::RectangleShape topBar(sf::Vector2f(900, 60));
    topBar.setFillColor(titleColor);
    topBar.setPosition(0, 0);
    window.draw(topBar);

    sf::Text title = makeText("MediCore Hospital Management System", 20, sf::Color::White, 20, 15);
    window.draw(title);
}

// ===================== HELPER: MAKE TEXT =====================
sf::Text GUI::makeText(string str, int size, sf::Color color, float x, float y) {
    sf::Text text;
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setPosition(x, y);
    return text;
}

// ===================== HELPER: MAKE BUTTON =====================
sf::RectangleShape GUI::makeButton(float x, float y, float w, float h, sf::Color color) {
    sf::RectangleShape btn(sf::Vector2f(w, h));
    btn.setPosition(x, y);
    btn.setFillColor(color);
    return btn;
}

// ===================== HELPER: IS MOUSE OVER =====================
bool GUI::isMouseOver(sf::RectangleShape& btn, sf::RenderWindow& win) {
    sf::Vector2i mouse = sf::Mouse::getPosition(win);
    sf::FloatRect bounds = btn.getGlobalBounds();
    return bounds.contains((float)mouse.x, (float)mouse.y);
}

// ===================== HELPER: SHOW MESSAGE =====================
void GUI::showMessage(string message, bool isError) {
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed)
                return;
        }
        drawBackground();
        sf::RectangleShape box = makeButton(150, 200, 600, 150, sf::Color::White);
        box.setOutlineThickness(2);
        box.setOutlineColor(isError ? errorColor : successColor);
        window.draw(box);

        sf::Color col = isError ? errorColor : successColor;
        sf::Text msg = makeText(message, 18, col, 170, 250);
        window.draw(msg);

        sf::Text hint = makeText("Press any key or click to continue...", 14, sf::Color(100, 100, 100), 250, 310);
        window.draw(hint);

        window.display();
        if (clock.getElapsedTime().asSeconds() > 5) return;
    }
}

// ===================== HELPER: GET TEXT INPUT =====================
string GUI::getTextInput(string prompt, bool isPassword) {
    string input = "";
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == 8 && input.size() > 0)
                    input.pop_back();
                else if (event.text.unicode == 13)
                    return input;
                else if (event.text.unicode >= 32 && event.text.unicode < 128)
                    input += (char)event.text.unicode;
            }
        }
        drawBackground();
        sf::Text promptText = makeText(prompt, 20, titleColor, 100, 200);
        window.draw(promptText);
        string display = isPassword ? string(input.size(), '*') : input;
        sf::RectangleShape inputBox = makeButton(100, 260, 700, 45, sf::Color::White);
        inputBox.setOutlineThickness(2);
        inputBox.setOutlineColor(buttonColor);
        window.draw(inputBox);
        sf::Text inputText = makeText(display + "|", 20, sf::Color::Black, 110, 268);
        window.draw(inputText);
        sf::Text hint = makeText("Press Enter to confirm", 14, sf::Color(100, 100, 100), 100, 320);
        window.draw(hint);
        window.display();
    }
    return input;
}

// ===================== HELPER: SHOW MENU =====================
int GUI::showMenu(string title, string options[], int count, string subtitle) {
    float btnW = 600, btnH = 45, startX = 150, startY = 130, gap = 55;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                for (int i = 0; i < count; i++) {
                    sf::RectangleShape btn = makeButton(startX, startY + i * gap, btnW, btnH, buttonColor);
                    if (isMouseOver(btn, window))
                        return i + 1;
                }
            }
        }

        drawBackground();

        sf::Text titleText = makeText(title, 24, titleColor, startX, 75);
        window.draw(titleText);

        if (subtitle != "") {
            sf::Text sub = makeText(subtitle, 16, sf::Color(80, 80, 80), startX, 105);
            window.draw(sub);
        }

        for (int i = 0; i < count; i++) {
            sf::RectangleShape btn = makeButton(startX, startY + i * gap, btnW, btnH, buttonColor);
            if (isMouseOver(btn, window))
                btn.setFillColor(buttonHoverColor);
            window.draw(btn);

            string label = to_string(i + 1) + ". " + options[i];
            sf::Text t = makeText(label, 18, buttonTextColor, startX + 15, startY + i * gap + 12);
            window.draw(t);
        }

        window.display();
    }
    return -1;
}

// ===================== UTILITY =====================
string GUI::getCurrentDate() {
    time_t t = time(0);
    tm now;
    localtime_s(&now, &t);
    char buf[11];
    sprintf_s(buf, "%02d-%02d-%04d", now.tm_mday, now.tm_mon + 1, now.tm_year + 1900);
    return string(buf);
}

int GUI::getMaxAppointmentID() {
    int maxID = 0;
    for (int i = 0; i < appointments->size(); i++)
        if (appointments->getAll()[i].getID() > maxID)
            maxID = appointments->getAll()[i].getID();
    return maxID;
}

int GUI::getMaxBillID() {
    int maxID = 0;
    for (int i = 0; i < bills->size(); i++)
        if (bills->getAll()[i].getID() > maxID)
            maxID = bills->getAll()[i].getID();
    return maxID;
}

int GUI::getMaxPrescriptionID() {
    int maxID = 0;
    for (int i = 0; i < prescriptions->size(); i++)
        if (prescriptions->getAll()[i].getID() > maxID)
            maxID = prescriptions->getAll()[i].getID();
    return maxID;
}

string GUI::convertDateForComparison(string date) {
    string year = "", month = "", day = "";
    day += date[0]; day += date[1];
    month += date[3]; month += date[4];
    year += date[6]; year += date[7];
    year += date[8]; year += date[9];
    return year + "-" + month + "-" + day;
}

// ===================== RUN =====================
void GUI::run() {
    while (window.isOpen()) {
        int role = showRoleSelection();
        if (role == 4 || role == -1) {
            window.close();
            return;
        }

        if (role == 1) {
            int patID;
            if (loginPatient(patID)) {
                Patient* p = patients->findByID(patID);
                if (p) runPatientMenu(*p);
            }
        }
        else if (role == 2) {
            int docID;
            if (loginDoctor(docID)) {
                Doctor* d = doctors->findByID(docID);
                if (d) runDoctorMenu(*d);
            }
        }
        else if (role == 3) {
            if (loginAdmin()) {
                runAdminMenu();
            }
        }
    }
}

// ===================== ROLE SELECTION =====================
int GUI::showRoleSelection() {
    string options[] = { "Patient", "Doctor", "Admin", "Exit" };
    return showMenu("Welcome to MediCore Hospital Management System", options, 4, "Please select your role to login");
}

// ===================== LOGIN PATIENT =====================
bool GUI::loginPatient(int& outID) {
    int attempts = 0;
    while (attempts < 3) {
        string idStr = getTextInput("Enter Patient ID:");
        int id = 0;
        for (int i = 0; idStr[i] != '\0'; i++)
            id = id * 10 + (idStr[i] - '0');

        string pass = getTextInput("Enter Password:", true);

        Patient* p = patients->findByID(id);
        if (p != nullptr && p->getPassword() == pass) {
            outID = id;
            showMessage("Login successful! Welcome, " + p->getName(), false);
            return true;
        }
        else {
            attempts++;
            string msg = "Invalid ID or password. " + to_string(3 - attempts) + " attempt(s) remaining.";
            if (attempts >= 3) msg = "Account locked. Contact admin.";
            showMessage(msg, true);
            fh->appendSecurityLog(getCurrentDate(), "Patient", idStr, "FAILED");
        }
    }
    return false;
}

// ===================== LOGIN DOCTOR =====================
bool GUI::loginDoctor(int& outID) {
    int attempts = 0;
    while (attempts < 3) {
        string idStr = getTextInput("Enter Doctor ID:");
        int id = 0;
        for (int i = 0; idStr[i] != '\0'; i++)
            id = id * 10 + (idStr[i] - '0');

        string pass = getTextInput("Enter Password:", true);

        Doctor* d = doctors->findByID(id);
        if (d != nullptr && d->getPassword() == pass) {
            outID = id;
            showMessage("Login successful! Welcome, Dr. " + d->getName(), false);
            return true;
        }
        else {
            attempts++;
            string msg = "Invalid ID or password. " + to_string(3 - attempts) + " attempt(s) remaining.";
            if (attempts >= 3) msg = "Account locked. Contact admin.";
            showMessage(msg, true);
            fh->appendSecurityLog(getCurrentDate(), "Doctor", idStr, "FAILED");
        }
    }
    return false;
}

// ===================== LOGIN ADMIN =====================
bool GUI::loginAdmin() {
    int attempts = 0;
    while (attempts < 3) {
        string idStr = getTextInput("Enter Admin ID:");

        // clean idStr - remove spaces, \r, \n
        string cleanID = "";
        for (int i = 0; i < (int)idStr.size(); i++) {
            if (idStr[i] != ' ' && idStr[i] != '\r' && idStr[i] != '\n')
                cleanID += idStr[i];
        }

        // convert to int
        int id = 0;
        for (int i = 0; i < (int)cleanID.size(); i++)
            id = id * 10 + (cleanID[i] - '0');

        string pass = getTextInput("Enter Password:", true);

        // clean pass
        string cleanPass = "";
        for (int i = 0; i < (int)pass.size(); i++) {
            if (pass[i] != ' ' && pass[i] != '\r' && pass[i] != '\n')
                cleanPass += pass[i];
        }

        if (admin->getID() == id && admin->getPassword() == cleanPass) {
            showMessage("Login successful! Welcome, Admin.", false);
            return true;
        }
        else {
            attempts++;
            string msg = "Invalid ID or password. " + to_string(3 - attempts) + " attempt(s) remaining.";
            if (attempts >= 3) msg = "Account locked. Contact admin.";
            showMessage(msg, true);
            fh->appendSecurityLog(getCurrentDate(), "Admin", idStr, "FAILED");
        }
    }
    return false;
}

// ===================== PATIENT MENU =====================
void GUI::runPatientMenu(Patient& p) {
    while (window.isOpen()) {
        string options[] = {
            "Book Appointment",
            "Cancel Appointment",
            "View My Appointments",
            "View My Medical Records",
            "View My Bills",
            "Pay Bill",
            "Top Up Balance",
            "Logout"
        };
        string subtitle = "Welcome, " + p.getName() + "  |  Balance: PKR " + to_string((int)p.getBalance());
        int choice = showMenu("Patient Menu", options, 8, subtitle);

        if (choice == 1) bookAppointment(p);
        else if (choice == 2) cancelAppointment(p);
        else if (choice == 3) viewMyAppointments(p);
        else if (choice == 4) viewMyMedicalRecords(p);
        else if (choice == 5) viewMyBills(p);
        else if (choice == 6) payBill(p);
        else if (choice == 7) topUpBalance(p);
        else if (choice == 8 || choice == -1) return;
    }
}

// ===================== BOOK APPOINTMENT =====================
void GUI::bookAppointment(Patient& p) {
    string spec = getTextInput("Enter specialization to search (e.g. Cardiology):");

    // convert to lowercase
    string specLower = spec;
    for (int i = 0; i < (int)specLower.size(); i++)
        if (specLower[i] >= 'A' && specLower[i] <= 'Z')
            specLower[i] += 32;

    // find matching doctors
    string docList = "Available Doctors:\n\n";
    bool found = false;
    for (int i = 0; i < doctors->size(); i++) {
        string ds = doctors->getAll()[i].getSpecialization();
        string dsL = ds;
        for (int j = 0; j < (int)dsL.size(); j++)
            if (dsL[j] >= 'A' && dsL[j] <= 'Z') dsL[j] += 32;
        if (dsL == specLower) {
            docList += "ID: " + to_string(doctors->getAll()[i].getID()) +
                "  Name: " + doctors->getAll()[i].getName() +
                "  Fee: PKR " + to_string((int)doctors->getAll()[i].getFee()) + "\n";
            found = true;
        }
    }

    if (!found) {
        showMessage("No doctors available for that specialization.", true);
        return;
    }

    showMessage(docList, false);

    string docIDStr = getTextInput("Enter Doctor ID:");
    int docID = 0;
    for (int i = 0; docIDStr[i] != '\0'; i++)
        docID = docID * 10 + (docIDStr[i] - '0');

    Doctor* doc = doctors->findByID(docID);
    if (!doc) {
        showMessage("Doctor not found.", true);
        return;
    }

    string date = "";
    int dateAttempts = 0;
    bool validDate = false;
    while (dateAttempts < 3 && !validDate) {
        date = getTextInput("Enter date (DD-MM-YYYY):");
        if (validator->validateDate(date))
            validDate = true;
        else {
            dateAttempts++;
            showMessage("Invalid date. Use format DD-MM-YYYY.", true);
        }
    }
    if (!validDate) return;

    // show available slots
    string slots[8] = { "09:00","10:00","11:00","12:00","13:00","14:00","15:00","16:00" };
    string slotList = "Available slots for Dr. " + doc->getName() + " on " + date + ":\n\n";
    for (int i = 0; i < 8; i++) {
        bool taken = false;
        for (int j = 0; j < appointments->size(); j++) {
            if (appointments->getAll()[j].getd_id() == docID &&
                appointments->getAll()[j].getDate() == date &&
                appointments->getAll()[j].getTime() == slots[i] &&
                appointments->getAll()[j].getStatus() != "cancelled") {
                taken = true;
                break;
            }
        }
        if (!taken) slotList += slots[i] + "\n";
    }
    showMessage(slotList, false);

    string slot = getTextInput("Enter time slot (e.g. 09:00):");

    if (!validator->validateTimeSlot(slot)) {
        showMessage("Invalid time slot.", true);
        return;
    }

    // check slot taken
    for (int j = 0; j < appointments->size(); j++) {
        if (appointments->getAll()[j].getd_id() == docID &&
            appointments->getAll()[j].getDate() == date &&
            appointments->getAll()[j].getTime() == slot &&
            appointments->getAll()[j].getStatus() != "cancelled") {
            showMessage("This slot is already taken. Please choose another.", true);
            return;
        }
    }

    // check balance
    if (p.getBalance() < doc->getFee()) {
        showMessage("Insufficient balance. Please top up your account.", true);
        return;
    }

    // book
    p -= doc->getFee();
    int newID = getMaxAppointmentID() + 1;
    Appointment newApp(newID, p.getID(), docID, date, slot, "pending");
    appointments->add(newApp);
    fh->appendAppointment(newApp);

    int newBillID = getMaxBillID() + 1;
    Bill newBill(newBillID, p.getID(), newID, doc->getFee(), "unpaid", date);
    bills->add(newBill);
    fh->appendBill(newBill);
    fh->updatePatient(p);

    showMessage("Appointment booked successfully! Appointment ID: " + to_string(newID), false);
}

// ===================== CANCEL APPOINTMENT =====================
void GUI::cancelAppointment(Patient& p) {
    string apList = "Your Pending Appointments:\n\n";
    bool found = false;
    for (int i = 0; i < appointments->size(); i++) {
        if (appointments->getAll()[i].getp_id() == p.getID() &&
            appointments->getAll()[i].getStatus() == "pending") {
            Doctor* doc = doctors->findByID(appointments->getAll()[i].getd_id());
            string dn = doc ? doc->getName() : "Unknown";
            apList += "ID: " + to_string(appointments->getAll()[i].getID()) +
                "  Dr. " + dn +
                "  Date: " + appointments->getAll()[i].getDate() +
                "  Time: " + appointments->getAll()[i].getTime() + "\n";
            found = true;
        }
    }

    if (!found) {
        showMessage("You have no pending appointments.", true);
        return;
    }

    showMessage(apList, false);

    string idStr = getTextInput("Enter Appointment ID to cancel:");
    int appID = 0;
    for (int i = 0; idStr[i] != '\0'; i++)
        appID = appID * 10 + (idStr[i] - '0');

    Appointment* app = appointments->findByID(appID);
    if (!app || app->getp_id() != p.getID() || app->getStatus() != "pending") {
        showMessage("Invalid appointment ID.", true);
        return;
    }

    Doctor* doc = doctors->findByID(app->getd_id());
    double fee = doc ? doc->getFee() : 0;

    app->setStatus("cancelled");
    fh->updateAppointment(*app);
    p += fee;
    fh->updatePatient(p);

    for (int i = 0; i < bills->size(); i++) {
        if (bills->getAll()[i].geta_id() == appID) {
            bills->getAll()[i].setStatus("cancelled");
            fh->updateBill(bills->getAll()[i]);
            break;
        }
    }

    showMessage("Appointment cancelled. PKR " + to_string((int)fee) + " refunded to your balance.", false);
}

// ===================== VIEW MY APPOINTMENTS =====================
void GUI::viewMyAppointments(Patient& p) {
    string result = "Your Appointments:\n\n";
    bool found = false;
    for (int i = 0; i < appointments->size(); i++) {
        if (appointments->getAll()[i].getp_id() == p.getID()) {
            Doctor* doc = doctors->findByID(appointments->getAll()[i].getd_id());
            string dn = doc ? doc->getName() : "Unknown";
            string ds = doc ? doc->getSpecialization() : "Unknown";
            result += "ID: " + to_string(appointments->getAll()[i].getID()) +
                "  Dr. " + dn + " (" + ds + ")" +
                "  Date: " + appointments->getAll()[i].getDate() +
                "  Time: " + appointments->getAll()[i].getTime() +
                "  Status: " + appointments->getAll()[i].getStatus() + "\n";
            found = true;
        }
    }
    if (!found) result = "No appointments found.";
    showMessage(result, false);
}

// ===================== VIEW MY MEDICAL RECORDS =====================
void GUI::viewMyMedicalRecords(Patient& p) {
    string result = "Your Medical Records:\n\n";
    bool found = false;
    for (int i = 0; i < prescriptions->size(); i++) {
        if (prescriptions->getAll()[i].getp_id() == p.getID()) {
            Doctor* doc = doctors->findByID(prescriptions->getAll()[i].getd_id());
            string dn = doc ? doc->getName() : "Unknown";
            result += "Date: " + prescriptions->getAll()[i].getDate() +
                "  Dr. " + dn +
                "\nMedicines: " + prescriptions->getAll()[i].getMedicines() +
                "\nNotes: " + prescriptions->getAll()[i].getNotes() + "\n\n";
            found = true;
        }
    }
    if (!found) result = "No medical records found.";
    showMessage(result, false);
}

// ===================== VIEW MY BILLS =====================
void GUI::viewMyBills(Patient& p) {
    string result = "Your Bills:\n\n";
    bool found = false;
    double totalUnpaid = 0;
    for (int i = 0; i < bills->size(); i++) {
        if (bills->getAll()[i].getp_id() == p.getID()) {
            result += "Bill ID: " + to_string(bills->getAll()[i].getID()) +
                "  Amount: PKR " + to_string((int)bills->getAll()[i].getAmount()) +
                "  Status: " + bills->getAll()[i].getStatus() +
                "  Date: " + bills->getAll()[i].getDate() + "\n";
            if (bills->getAll()[i].getStatus() == "unpaid")
                totalUnpaid += bills->getAll()[i].getAmount();
            found = true;
        }
    }
    if (!found) result = "No bills found.";
    else result += "\nTotal Unpaid: PKR " + to_string((int)totalUnpaid);
    showMessage(result, false);
}

// ===================== PAY BILL =====================
void GUI::payBill(Patient& p) {
    string billList = "Unpaid Bills:\n\n";
    bool found = false;
    for (int i = 0; i < bills->size(); i++) {
        if (bills->getAll()[i].getp_id() == p.getID() &&
            bills->getAll()[i].getStatus() == "unpaid") {
            billList += "Bill ID: " + to_string(bills->getAll()[i].getID()) +
                "  Amount: PKR " + to_string((int)bills->getAll()[i].getAmount()) +
                "  Date: " + bills->getAll()[i].getDate() + "\n";
            found = true;
        }
    }

    if (!found) {
        showMessage("No unpaid bills.", true);
        return;
    }

    showMessage(billList, false);

    string idStr = getTextInput("Enter Bill ID to pay:");
    int billID = 0;
    for (int i = 0; idStr[i] != '\0'; i++)
        billID = billID * 10 + (idStr[i] - '0');

    Bill* bill = bills->findByID(billID);
    if (!bill || bill->getp_id() != p.getID() || bill->getStatus() != "unpaid") {
        showMessage("Invalid bill ID.", true);
        return;
    }

    if (p.getBalance() < bill->getAmount()) {
        showMessage("Insufficient balance to pay this bill.", true);
        return;
    }

    p -= bill->getAmount();
    bill->setStatus("paid");
    fh->updateBill(*bill);
    fh->updatePatient(p);

    showMessage("Bill paid successfully! Remaining balance: PKR " + to_string((int)p.getBalance()), false);
}

// ===================== TOP UP BALANCE =====================
void GUI::topUpBalance(Patient& p) {
    int attempts = 0;
    while (attempts < 3) {
        string amtStr = getTextInput("Enter amount to add (PKR):");
        double amount = 0;
        bool valid = true;
        for (int i = 0; amtStr[i] != '\0'; i++) {
            if (amtStr[i] >= '0' && amtStr[i] <= '9')
                amount = amount * 10 + (amtStr[i] - '0');
            else { valid = false; break; }
        }
        if (!valid || amount <= 0) {
            attempts++;
            showMessage("Invalid amount. Must be a positive number.", true);
            continue;
        }
        p += amount;
        fh->updatePatient(p);
        showMessage("Balance updated! New balance: PKR " + to_string((int)p.getBalance()), false);
        return;
    }
}

// ===================== DOCTOR MENU =====================
void GUI::runDoctorMenu(Doctor& d) {
    while (window.isOpen()) {
        string options[] = {
            "View Today's Appointments",
            "Mark Appointment Complete",
            "Mark Appointment No-Show",
            "Write Prescription",
            "View Patient Medical History",
            "Logout"
        };
        string subtitle = "Welcome, Dr. " + d.getName() + "  |  " + d.getSpecialization();
        int choice = showMenu("Doctor Menu", options, 6, subtitle);

        if (choice == 1) viewTodaysAppointments(d);
        else if (choice == 2) markAppointmentComplete(d);
        else if (choice == 3) markAppointmentNoShow(d);
        else if (choice == 4) writePrescription(d);
        else if (choice == 5) viewPatientMedicalHistory(d);
        else if (choice == 6 || choice == -1) return;
    }
}

// ===================== VIEW TODAY'S APPOINTMENTS =====================
void GUI::viewTodaysAppointments(Doctor& d) {
    string today = getCurrentDate();
    string result = "Today's Appointments (" + today + "):\n\n";
    bool found = false;
    for (int i = 0; i < appointments->size(); i++) {
        if (appointments->getAll()[i].getd_id() == d.getID() &&
            appointments->getAll()[i].getDate() == today) {
            Patient* pat = patients->findByID(appointments->getAll()[i].getp_id());
            string pn = pat ? pat->getName() : "Unknown";
            result += "ID: " + to_string(appointments->getAll()[i].getID()) +
                "  Patient: " + pn +
                "  Time: " + appointments->getAll()[i].getTime() +
                "  Status: " + appointments->getAll()[i].getStatus() + "\n";
            found = true;
        }
    }
    if (!found) result = "No appointments scheduled for today.";
    showMessage(result, false);
}

// ===================== MARK APPOINTMENT COMPLETE =====================
void GUI::markAppointmentComplete(Doctor& d) {
    string today = getCurrentDate();
    string apList = "Today's Pending Appointments:\n\n";
    bool found = false;
    for (int i = 0; i < appointments->size(); i++) {
        if (appointments->getAll()[i].getd_id() == d.getID() &&
            appointments->getAll()[i].getStatus() == "pending" &&
            appointments->getAll()[i].getDate() == today) {
            apList += "ID: " + to_string(appointments->getAll()[i].getID()) +
                "  Time: " + appointments->getAll()[i].getTime() + "\n";
            found = true;
        }
    }
    if (!found) {
        showMessage("No pending appointments for today.", true);
        return;
    }
    showMessage(apList, false);

    string idStr = getTextInput("Enter Appointment ID to mark complete:");
    int appID = 0;
    for (int i = 0; idStr[i] != '\0'; i++)
        appID = appID * 10 + (idStr[i] - '0');

    Appointment* app = appointments->findByID(appID);
    if (!app || app->getd_id() != d.getID() || app->getStatus() != "pending" || app->getDate() != today) {
        showMessage("Invalid appointment ID.", true);
        return;
    }
    app->setStatus("completed");
    fh->updateAppointment(*app);
    showMessage("Appointment marked as completed.", false);
}

// ===================== MARK APPOINTMENT NO SHOW =====================
void GUI::markAppointmentNoShow(Doctor& d) {
    string today = getCurrentDate();
    string apList = "Today's Pending Appointments:\n\n";
    bool found = false;
    for (int i = 0; i < appointments->size(); i++) {
        if (appointments->getAll()[i].getd_id() == d.getID() &&
            appointments->getAll()[i].getStatus() == "pending" &&
            appointments->getAll()[i].getDate() == today) {
            apList += "ID: " + to_string(appointments->getAll()[i].getID()) +
                "  Time: " + appointments->getAll()[i].getTime() + "\n";
            found = true;
        }
    }
    if (!found) {
        showMessage("No pending appointments for today.", true);
        return;
    }
    showMessage(apList, false);

    string idStr = getTextInput("Enter Appointment ID to mark no-show:");
    int appID = 0;
    for (int i = 0; idStr[i] != '\0'; i++)
        appID = appID * 10 + (idStr[i] - '0');

    Appointment* app = appointments->findByID(appID);
    if (!app || app->getd_id() != d.getID() || app->getStatus() != "pending" || app->getDate() != today) {
        showMessage("Invalid appointment ID.", true);
        return;
    }
    app->setStatus("no-show");
    fh->updateAppointment(*app);

    for (int i = 0; i < bills->size(); i++) {
        if (bills->getAll()[i].geta_id() == appID) {
            bills->getAll()[i].setStatus("cancelled");
            fh->updateBill(bills->getAll()[i]);
            break;
        }
    }
    showMessage("Appointment marked as no-show.", false);
}

// ===================== WRITE PRESCRIPTION =====================
void GUI::writePrescription(Doctor& d) {
    string idStr = getTextInput("Enter Appointment ID:");
    int appID = 0;
    for (int i = 0; idStr[i] != '\0'; i++)
        appID = appID * 10 + (idStr[i] - '0');

    Appointment* app = appointments->findByID(appID);
    if (!app || app->getd_id() != d.getID() || app->getStatus() != "completed") {
        showMessage("Invalid appointment or not completed.", true);
        return;
    }

    for (int i = 0; i < prescriptions->size(); i++) {
        if (prescriptions->getAll()[i].geta_id() == appID) {
            showMessage("Prescription already written for this appointment.", true);
            return;
        }
    }

    string medicines = getTextInput("Enter medicines (e.g. Paracetamol 500mg;Amoxicillin 250mg):");
    string notes = getTextInput("Enter notes:");

    int newID = getMaxPrescriptionID() + 1;
    Prescription newPres(newID, appID, app->getp_id(), d.getID(), app->getDate(), medicines, notes);
    prescriptions->add(newPres);
    fh->appendPrescription(newPres);
    showMessage("Prescription saved.", false);
}

// ===================== VIEW PATIENT MEDICAL HISTORY =====================
void GUI::viewPatientMedicalHistory(Doctor& d) {
    string idStr = getTextInput("Enter Patient ID:");
    int patID = 0;
    for (int i = 0; idStr[i] != '\0'; i++)
        patID = patID * 10 + (idStr[i] - '0');

    Patient* pat = patients->findByID(patID);
    if (!pat) {
        showMessage("Access denied. You can only view records of your own patients.", true);
        return;
    }

    bool hasCompleted = false;
    for (int i = 0; i < appointments->size(); i++) {
        if (appointments->getAll()[i].getp_id() == patID &&
            appointments->getAll()[i].getd_id() == d.getID() &&
            appointments->getAll()[i].getStatus() == "completed") {
            hasCompleted = true;
            break;
        }
    }

    if (!hasCompleted) {
        showMessage("Access denied. You can only view records of your own patients.", true);
        return;
    }

    string result = "Medical History for " + pat->getName() + ":\n\n";
    bool found = false;
    for (int i = 0; i < prescriptions->size(); i++) {
        if (prescriptions->getAll()[i].getp_id() == patID &&
            prescriptions->getAll()[i].getd_id() == d.getID()) {
            result += "Date: " + prescriptions->getAll()[i].getDate() +
                "\nMedicines: " + prescriptions->getAll()[i].getMedicines() +
                "\nNotes: " + prescriptions->getAll()[i].getNotes() + "\n\n";
            found = true;
        }
    }
    if (!found) result = "No prescriptions found for this patient.";
    showMessage(result, false);
}

// ===================== ADMIN MENU =====================
void GUI::runAdminMenu() {
    while (window.isOpen()) {
        string options[] = {
            "Add Doctor",
            "Remove Doctor",
            "View All Patients",
            "View All Doctors",
            "View All Appointments",
            "View Unpaid Bills",
            "Discharge Patient",
            "View Security Log",
            "Generate Daily Report",
            "Logout"
        };
        int choice = showMenu("Admin Panel - MediCore", options, 10, "");

        if (choice == 1) addDoctor();
        else if (choice == 2) removeDoctor();
        else if (choice == 3) viewAllPatients();
        else if (choice == 4) viewAllDoctors();
        else if (choice == 5) viewAllAppointments();
        else if (choice == 6) viewUnpaidBills();
        else if (choice == 7) dischargePatient();
        else if (choice == 8) viewSecurityLog();
        else if (choice == 9) generateDailyReport();
        else if (choice == 10 || choice == -1) return;
    }
}

// ===================== ADD DOCTOR =====================
void GUI::addDoctor() {
    string name = getTextInput("Enter doctor name:");
    string spec = getTextInput("Enter specialization:");

    string contact = "";
    bool validContact = false;
    while (!validContact) {
        contact = getTextInput("Enter contact (11 digits):");
        if (validator->validateContact(contact))
            validContact = true;
        else
            showMessage("Invalid contact. Must be exactly 11 digits.", true);
    }

    string password = "";
    bool validPass = false;
    while (!validPass) {
        password = getTextInput("Enter password (min 6 chars):", true);
        if (validator->validatePassword(password))
            validPass = true;
        else
            showMessage("Password must be at least 6 characters.", true);
    }

    string feeStr = getTextInput("Enter consultation fee:");
    double fee = 0;
    bool decimal = false;
    double decimalPlace = 0.1;
    for (int i = 0; feeStr[i] != '\0'; i++) {
        if (feeStr[i] == '.') {
            decimal = true;
        }
        else if (decimal) {
            fee += (feeStr[i] - '0') * decimalPlace;
            decimalPlace *= 0.1;
        }
        else {
            fee = fee * 10 + (feeStr[i] - '0');
        }
    }
    if (fee <= 0) {
        showMessage("Fee must be a positive number.", true);
        return;
    }
    int newID = 1;
    for (int i = 0; i < doctors->size(); i++)
        if (doctors->getAll()[i].getID() >= newID)
            newID = doctors->getAll()[i].getID() + 1;
    Doctor newDoc(newID, name, password, spec, contact, fee);
    doctors->add(newDoc);
    fh->appendDoctor(newDoc);
    showMessage("Doctor added successfully! ID: " + to_string(newID), false);
}

// ===================== REMOVE DOCTOR =====================
void GUI::removeDoctor() {
    string docList = "All Doctors:\n\n";
    for (int i = 0; i < doctors->size(); i++) {
        docList += "ID: " + to_string(doctors->getAll()[i].getID()) +
            "  " + doctors->getAll()[i].getName() +
            "  " + doctors->getAll()[i].getSpecialization() +
            "  Fee: PKR " + to_string((int)doctors->getAll()[i].getFee()) + "\n";
    }
    showMessage(docList, false);

    string idStr = getTextInput("Enter Doctor ID to remove:");
    int docID = 0;
    for (int i = 0; idStr[i] != '\0'; i++)
        docID = docID * 10 + (idStr[i] - '0');

    for (int i = 0; i < appointments->size(); i++) {
        if (appointments->getAll()[i].getd_id() == docID &&
            appointments->getAll()[i].getStatus() == "pending") {
            showMessage("Cannot remove doctor with pending appointments.", true);
            return;
        }
    }

    doctors->removeByID(docID);
    fh->deleteDoctor(docID);
    showMessage("Doctor removed successfully.", false);
}

// ===================== VIEW ALL PATIENTS =====================
void GUI::viewAllPatients() {
    string result = "All Patients:\n\n";
    for (int i = 0; i < patients->size(); i++) {
        int unpaid = 0;
        for (int j = 0; j < bills->size(); j++)
            if (bills->getAll()[j].getp_id() == patients->getAll()[i].getID() &&
                bills->getAll()[j].getStatus() == "unpaid")
                unpaid++;
        result += "ID: " + to_string(patients->getAll()[i].getID()) +
            "  " + patients->getAll()[i].getName() +
            "  Age: " + to_string(patients->getAll()[i].getAge()) +
            "  Balance: PKR " + to_string((int)patients->getAll()[i].getBalance()) +
            "  Unpaid Bills: " + to_string(unpaid) + "\n";
    }
    if (patients->size() == 0) result = "No patients found.";
    showMessage(result, false);
}

// ===================== VIEW ALL DOCTORS =====================
void GUI::viewAllDoctors() {
    string result = "All Doctors:\n\n";
    for (int i = 0; i < doctors->size(); i++) {
        result += "ID: " + to_string(doctors->getAll()[i].getID()) +
            "  " + doctors->getAll()[i].getName() +
            "  " + doctors->getAll()[i].getSpecialization() +
            "  Fee: PKR " + to_string((int)doctors->getAll()[i].getFee()) + "\n";
    }
    if (doctors->size() == 0) result = "No doctors found.";
    showMessage(result, false);
}

// ===================== VIEW ALL APPOINTMENTS =====================
void GUI::viewAllAppointments() {
    string result = "All Appointments:\n\n";
    for (int i = 0; i < appointments->size(); i++) {
        Patient* pat = patients->findByID(appointments->getAll()[i].getp_id());
        Doctor* doc = doctors->findByID(appointments->getAll()[i].getd_id());
        string pn = pat ? pat->getName() : "Unknown";
        string dn = doc ? doc->getName() : "Unknown";
        result += "ID: " + to_string(appointments->getAll()[i].getID()) +
            "  Patient: " + pn +
            "  Dr. " + dn +
            "  Date: " + appointments->getAll()[i].getDate() +
            "  Time: " + appointments->getAll()[i].getTime() +
            "  Status: " + appointments->getAll()[i].getStatus() + "\n";
    }
    if (appointments->size() == 0) result = "No appointments found.";
    showMessage(result, false);
}

// ===================== VIEW UNPAID BILLS =====================
void GUI::viewUnpaidBills() {
    string result = "Unpaid Bills:\n\n";
    bool found = false;
    for (int i = 0; i < bills->size(); i++) {
        if (bills->getAll()[i].getStatus() == "unpaid") {
            Patient* pat = patients->findByID(bills->getAll()[i].getp_id());
            string pn = pat ? pat->getName() : "Unknown";
            result += "Bill ID: " + to_string(bills->getAll()[i].getID()) +
                "  Patient: " + pn +
                "  Amount: PKR " + to_string((int)bills->getAll()[i].getAmount()) +
                "  Date: " + bills->getAll()[i].getDate() + "\n";
            found = true;
        }
    }
    if (!found) result = "No unpaid bills.";
    showMessage(result, false);
}

// ===================== DISCHARGE PATIENT =====================
void GUI::dischargePatient() {
    string idStr = getTextInput("Enter Patient ID to discharge:");
    int patID = 0;
    for (int i = 0; idStr[i] != '\0'; i++)
        patID = patID * 10 + (idStr[i] - '0');

    Patient* pat = patients->findByID(patID);
    if (!pat) {
        showMessage("Patient not found.", true);
        return;
    }

    for (int i = 0; i < bills->size(); i++) {
        if (bills->getAll()[i].getp_id() == patID &&
            bills->getAll()[i].getStatus() == "unpaid") {
            showMessage("Cannot discharge patient with unpaid bills.", true);
            return;
        }
    }

    for (int i = 0; i < appointments->size(); i++) {
        if (appointments->getAll()[i].getp_id() == patID &&
            appointments->getAll()[i].getStatus() == "pending") {
            showMessage("Cannot discharge patient with pending appointments.", true);
            return;
        }
    }

    fh->dischargePatient(patID);
    patients->removeByID(patID);
    showMessage("Patient discharged and archived successfully.", false);
}

// ===================== VIEW SECURITY LOG =====================
void GUI::viewSecurityLog() {
    ifstream file("security_log.txt");
    string result = "Security Log:\n\n";
    if (!file.is_open()) {
        showMessage("No security events logged.", true);
        return;
    }
    string line;
    bool empty = true;
    while (getline(file, line)) {
        if (line != "") {
            result += line + "\n";
            empty = false;
        }
    }
    file.close();
    if (empty) result = "No security events logged.";
    showMessage(result, false);
}

// ===================== GENERATE DAILY REPORT =====================
void GUI::generateDailyReport() {
    string today = getCurrentDate();
    int total = 0, pending = 0, completed = 0, noshow = 0, cancelled = 0;
    double revenue = 0;

    for (int i = 0; i < appointments->size(); i++) {
        if (appointments->getAll()[i].getDate() == today) {
            total++;
            string s = appointments->getAll()[i].getStatus();
            if (s == "pending") pending++;
            else if (s == "completed") completed++;
            else if (s == "no-show") noshow++;
            else if (s == "cancelled") cancelled++;
        }
    }

    for (int i = 0; i < bills->size(); i++) {
        if (bills->getAll()[i].getDate() == today &&
            bills->getAll()[i].getStatus() == "paid")
            revenue += bills->getAll()[i].getAmount();
    }

    string result = "Daily Report: " + today + "\n\n";
    result += "Total Appointments: " + to_string(total) + "\n";
    result += "  Pending: " + to_string(pending) + "\n";
    result += "  Completed: " + to_string(completed) + "\n";
    result += "  No-show: " + to_string(noshow) + "\n";
    result += "  Cancelled: " + to_string(cancelled) + "\n\n";
    result += "Revenue Today: PKR " + to_string((int)revenue) + "\n\n";

    result += "Patients with Unpaid Bills:\n";
    for (int i = 0; i < patients->size(); i++) {
        double owed = 0;
        for (int j = 0; j < bills->size(); j++)
            if (bills->getAll()[j].getp_id() == patients->getAll()[i].getID() &&
                bills->getAll()[j].getStatus() == "unpaid")
                owed += bills->getAll()[j].getAmount();
        if (owed > 0)
            result += patients->getAll()[i].getName() + ": PKR " + to_string((int)owed) + "\n";
    }

    showMessage(result, false);
}