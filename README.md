# MediCore Hospital Management System
## Description
A Hospital Management System built in C++ with a GUI made with SFML.
The system can support three users: Patient, Doctor, and Admin, and handles appointments, prescriptions, billing, and medical records. The system contains file handling so all data is read from and stored in .txt files.
## OOP Concepts Used
1.**Inheritance & Exception Handling**: The program shows inheritance at two different instances; 
First between classes Person, Doctor, Patient, and Admin. Person is the base class and Docor, Patient, and Admin inherit form it hierarchially.
Second between classes HospitalException, FileNotFoundException, InsufficientFundsException, InvalidInputException, and SlotUnavailableException. HospitalException is the base class and the others are its derived classes. All these are also custom exception classes made specifically for this System and are used throughout the program.
2.**Polymrphism**: Virtual functions dispay() and displayMenu() of class Person are overridden in its derived classes.
3.**Templates**: Storage<T> is a generic template class used to stor any type of object such as: Patient, Doctor, Appointment, Bill, Prescription.
4.**Operator Overoading**: += and -= on Patient for balance, == on Patient, Doctor, and Appointment for comparison, << on Patient, Doctor and Appointment for display.
## User Roles
### Admin
- Add and remove doctors
- View all patients, doctors and appointments
- View and manage unpaid bills
- Discharge patients
- View security log and generate daily reports
### Doctor
- View today's appointments
- Mark appointments as completed or no-show
- Write prescriptions for completed appointments
- View patient medical history
### Patient
- Book and cancel appointments with doctors by specialization
- View appointments, medical records and bills
- Pay bills and top up account balance
