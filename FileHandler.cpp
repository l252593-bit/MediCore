#include"FileHandler.h"
using namespace std;

void FileHandler::parseLine(string line, string* fields, int& fieldCount)
{
    fieldCount = 0;
    string current = "";
    for (int i = 0; line[i] != '\0'; i++)
    {
        if (line[i] == ',')
        {
            fields[fieldCount] = current;
            fieldCount++;
            current = "";
        }
        else
        {
            current = current + line[i];
        }
    }
    if (current != "")
    {
        fields[fieldCount] = current;
        fieldCount++;
    }

    for (int i = 0; i < fieldCount; i++)
    {
        if (fields[i].size() > 0 && fields[i].back() == '\r')
        {
            fields[i].pop_back();
        }
    }
}

void FileHandler::loadPatients(Storage<Patient>& storage)
{
    ifstream file("patients.txt");
    if (!file.is_open())
        throw FileNotFoundException("Error: patients.txt could not be opened.");

    string line;
    while (getline(file, line))
    {
        if (line == "") continue;
        string fields[10];
        int fieldCount = 0;
        parseLine(line, fields, fieldCount);
        if (fieldCount < 7) continue;

        int id = 0;
        for (int i = 0; fields[0][i] != '\0'; i++)
            id = id * 10 + (fields[0][i] - '0');

        int age = 0;
        for (int i = 0; fields[2][i] != '\0'; i++)
            age = age * 10 + (fields[2][i] - '0');

        char gender = fields[3][0];

        double balance = 0.0;
        int dotPos = -1;
        for (int i = 0; fields[6][i] != '\0'; i++)
        {
            if (fields[6][i] == '.') { dotPos = i; break; }
        }
        if (dotPos == -1)
        {
            for (int i = 0; fields[6][i] != '\0'; i++)
                balance = balance * 10 + (fields[6][i] - '0');
        }
        else
        {
            for (int i = 0; i < dotPos; i++)
                balance = balance * 10 + (fields[6][i] - '0');
            double decimal = 0.1;
            for (int i = dotPos + 1; fields[6][i] != '\0'; i++)
            {
                balance += (fields[6][i] - '0') * decimal;
                decimal *= 0.1;
            }
        }

        Patient p(id, fields[1], fields[5], age, gender, fields[4], balance);
        storage.add(p);
    }
    file.close();
}

void FileHandler::loadDoctors(Storage<Doctor>& storage)
{
    ifstream file("doctors.txt");
    if (!file.is_open())
        throw FileNotFoundException("Error: doctors.txt could not be opened.");

    string line;
    while (getline(file, line))
    {
        if (line == "") continue;
        string fields[10];
        int fieldCount = 0;
        parseLine(line, fields, fieldCount);
        if (fieldCount < 6) continue;

        int id = 0;
        for (int i = 0; fields[0][i] != '\0'; i++)
            id = id * 10 + (fields[0][i] - '0');

        double fee = 0.0;
        int dotPos = -1;
        for (int i = 0; fields[5][i] != '\0'; i++)
        {
            if (fields[5][i] == '.') { dotPos = i; break; }
        }
        if (dotPos == -1)
        {
            for (int i = 0; fields[5][i] != '\0'; i++)
                fee = fee * 10 + (fields[5][i] - '0');
        }
        else
        {
            for (int i = 0; i < dotPos; i++)
                fee = fee * 10 + (fields[5][i] - '0');
            double decimal = 0.1;
            for (int i = dotPos + 1; fields[5][i] != '\0'; i++)
            {
                fee += (fields[5][i] - '0') * decimal;
                decimal *= 0.1;
            }
        }

        Doctor d(id, fields[1], fields[4], fields[2], fields[3], fee);
        storage.add(d);
    }
    file.close();
}

void FileHandler::loadAdmin(Admin& admin)
{
    ifstream file("admin.txt");
    if (!file.is_open())
        throw FileNotFoundException("Error: admin.txt could not be opened.");

    string line;
    if (getline(file, line))
    {
        if (line != "")
        {
            string fields[10];
            int fieldCount = 0;
            parseLine(line, fields, fieldCount);
            if (fieldCount >= 3)
            {
                int id = 0;
                for (int i = 0; fields[0][i] != '\0'; i++)
                    id = id * 10 + (fields[0][i] - '0');
                admin = Admin(id, fields[1], fields[2]);
            }
        }
    }
    file.close();
}

void FileHandler::loadAppointments(Storage<Appointment>& storage)
{
    ifstream file("appointments.txt");
    if (!file.is_open())
        throw FileNotFoundException("Error: appointments.txt could not be opened.");

    string line;
    while (getline(file, line))
    {
        if (line == "") continue;
        string fields[10];
        int fieldCount = 0;
        parseLine(line, fields, fieldCount);
        if (fieldCount < 6) continue;

        int aid = 0, pid = 0, did = 0;
        for (int i = 0; fields[0][i] != '\0'; i++)
            aid = aid * 10 + (fields[0][i] - '0');
        for (int i = 0; fields[1][i] != '\0'; i++)
            pid = pid * 10 + (fields[1][i] - '0');
        for (int i = 0; fields[2][i] != '\0'; i++)
            did = did * 10 + (fields[2][i] - '0');

        Appointment a(aid, pid, did, fields[3], fields[4], fields[5]);
        storage.add(a);
    }
    file.close();
}

void FileHandler::loadBills(Storage<Bill>& storage)
{
    ifstream file("bills.txt");
    if (!file.is_open())
        throw FileNotFoundException("Error: bills.txt could not be opened.");

    string line;
    while (getline(file, line))
    {
        if (line == "") continue;
        string fields[10];
        int fieldCount = 0;
        parseLine(line, fields, fieldCount);
        if (fieldCount < 6) continue;

        int bid = 0, pid = 0, aid = 0;
        for (int i = 0; fields[0][i] != '\0'; i++)
            bid = bid * 10 + (fields[0][i] - '0');
        for (int i = 0; fields[1][i] != '\0'; i++)
            pid = pid * 10 + (fields[1][i] - '0');
        for (int i = 0; fields[2][i] != '\0'; i++)
            aid = aid * 10 + (fields[2][i] - '0');

        double amount = 0.0;
        int dotPos = -1;
        for (int i = 0; fields[3][i] != '\0'; i++)
        {
            if (fields[3][i] == '.') { dotPos = i; break; }
        }
        if (dotPos == -1)
        {
            for (int i = 0; fields[3][i] != '\0'; i++)
                amount = amount * 10 + (fields[3][i] - '0');
        }
        else
        {
            for (int i = 0; i < dotPos; i++)
                amount = amount * 10 + (fields[3][i] - '0');
            double decimal = 0.1;
            for (int i = dotPos + 1; fields[3][i] != '\0'; i++)
            {
                amount += (fields[3][i] - '0') * decimal;
                decimal *= 0.1;
            }
        }

        Bill b(bid, pid, aid, amount, fields[4], fields[5]);
        storage.add(b);
    }
    file.close();
}

void FileHandler::loadPrescriptions(Storage<Prescription>& storage)
{
    ifstream file("prescriptions.txt");
    if (!file.is_open())
        throw FileNotFoundException("Error: prescriptions.txt could not be opened.");

    string line;
    while (getline(file, line))
    {
        if (line == "") continue;
        string fields[10];
        int fieldCount = 0;
        parseLine(line, fields, fieldCount);
        if (fieldCount < 7) continue;

        int presid = 0, aid = 0, pid = 0, did = 0;
        for (int i = 0; fields[0][i] != '\0'; i++)
            presid = presid * 10 + (fields[0][i] - '0');
        for (int i = 0; fields[1][i] != '\0'; i++)
            aid = aid * 10 + (fields[1][i] - '0');
        for (int i = 0; fields[2][i] != '\0'; i++)
            pid = pid * 10 + (fields[2][i] - '0');
        for (int i = 0; fields[3][i] != '\0'; i++)
            did = did * 10 + (fields[3][i] - '0');

        Prescription p(presid, aid, pid, did, fields[4], fields[5], fields[6]);
        storage.add(p);
    }
    file.close();
}

void FileHandler::appendPatient(Patient& p)
{
    ofstream file("patients.txt", ios::app);
    if (!file.is_open())
        throw FileNotFoundException("Error: patients.txt could not be opened.");
    file << p.getID() << "," << p.getName() << "," << p.getAge() << ","
        << p.getGender() << "," << p.getContact() << "," << p.getPassword()
        << "," << p.getBalance() << "\n";
    file.close();
}

void FileHandler::appendDoctor(Doctor& d)
{
    ofstream file("doctors.txt", ios::app);
    if (!file.is_open())
        throw FileNotFoundException("Error: doctors.txt could not be opened.");
    file << d.getID() << "," << d.getName() << "," << d.getSpecialization()
        << "," << d.getContact() << "," << d.getPassword() << ","
        << d.getFee() << "\n";
    file.close();
}

void FileHandler::appendAppointment(Appointment& a)
{
    ofstream file("appointments.txt", ios::app);
    if (!file.is_open())
        throw FileNotFoundException("Error: appointments.txt could not be opened.");
    file << a.getID() << "," << a.getp_id() << "," << a.getd_id()
        << "," << a.getDate() << "," << a.getTime() << ","
        << a.getStatus() << "\n";
    file.close();
}

void FileHandler::appendBill(Bill& b)
{
    ofstream file("bills.txt", ios::app);
    if (!file.is_open())
        throw FileNotFoundException("Error: bills.txt could not be opened.");
    file << b.getID() << "," << b.getp_id() << "," << b.geta_id()
        << "," << b.getAmount() << "," << b.getStatus() << ","
        << b.getDate() << "\n";
    file.close();
}

void FileHandler::appendPrescription(Prescription& p)
{
    ofstream file("prescriptions.txt", ios::app);
    if (!file.is_open())
        throw FileNotFoundException("Error: prescriptions.txt could not be opened.");
    file << p.getID() << "," << p.geta_id() << ","
        << p.getp_id() << "," << p.getd_id() << "," << p.getDate()
        << "," << p.getMedicines() << "," << p.getNotes() << "\n";
    file.close();
}

void FileHandler::updatePatient(Patient& p)
{
    ifstream file("patients.txt");
    if (!file.is_open())
        throw FileNotFoundException("Error: patients.txt could not be opened.");

    string lines[100];
    int lineCount = 0;
    string line;
    while (getline(file, line))
    {
        if (line != "")
            lines[lineCount++] = line;
    }
    file.close();

    ofstream out("patients.txt");
    for (int i = 0; i < lineCount; i++)
    {
        string fields[10];
        int fieldCount = 0;
        parseLine(lines[i], fields, fieldCount);
        int id = 0;
        for (int j = 0; fields[0][j] != '\0'; j++)
            id = id * 10 + (fields[0][j] - '0');
        if (id == p.getID())
        {
            out << p.getID() << "," << p.getName() << "," << p.getAge()
                << "," << p.getGender() << "," << p.getContact() << ","
                << p.getPassword() << "," << p.getBalance() << "\n";
        }
        else
        {
            out << lines[i] << "\n";
        }
    }
    out.close();
}

void FileHandler::updateDoctor(Doctor& d)
{
    ifstream file("doctors.txt");
    if (!file.is_open())
        throw FileNotFoundException("Error: doctors.txt could not be opened.");

    string lines[100];
    int lineCount = 0;
    string line;
    while (getline(file, line))
    {
        if (line != "")
            lines[lineCount++] = line;
    }
    file.close();

    ofstream out("doctors.txt");
    for (int i = 0; i < lineCount; i++)
    {
        string fields[10];
        int fieldCount = 0;
        parseLine(lines[i], fields, fieldCount);
        int id = 0;
        for (int j = 0; fields[0][j] != '\0'; j++)
            id = id * 10 + (fields[0][j] - '0');
        if (id == d.getID())
        {
            out << d.getID() << "," << d.getName() << ","
                << d.getSpecialization() << "," << d.getContact() << ","
                << d.getPassword() << "," << d.getFee() << "\n";
        }
        else
        {
            out << lines[i] << "\n";
        }
    }
    out.close();
}

void FileHandler::updateAppointment(Appointment& a)
{
    ifstream file("appointments.txt");
    if (!file.is_open())
        throw FileNotFoundException("Error: appointments.txt could not be opened.");

    string lines[100];
    int lineCount = 0;
    string line;
    while (getline(file, line))
    {
        if (line != "")
            lines[lineCount++] = line;
    }
    file.close();

    ofstream out("appointments.txt");
    for (int i = 0; i < lineCount; i++)
    {
        string fields[10];
        int fieldCount = 0;
        parseLine(lines[i], fields, fieldCount);
        int id = 0;
        for (int j = 0; fields[0][j] != '\0'; j++)
            id = id * 10 + (fields[0][j] - '0');
        if (id == a.getID())
        {
            out << a.getID() << "," << a.getp_id() << "," << a.getd_id()
                << "," << a.getDate() << "," << a.getTime() << ","
                << a.getStatus() << "\n";
        }
        else
        {
            out << lines[i] << "\n";
        }
    }
    out.close();
}

void FileHandler::updateBill(Bill& b)
{
    ifstream file("bills.txt");
    if (!file.is_open())
        throw FileNotFoundException("Error: bills.txt could not be opened.");

    string lines[100];
    int lineCount = 0;
    string line;
    while (getline(file, line))
    {
        if (line != "")
            lines[lineCount++] = line;
    }
    file.close();

    ofstream out("bills.txt");
    for (int i = 0; i < lineCount; i++)
    {
        string fields[10];
        int fieldCount = 0;
        parseLine(lines[i], fields, fieldCount);
        int id = 0;
        for (int j = 0; fields[0][j] != '\0'; j++)
            id = id * 10 + (fields[0][j] - '0');
        if (id == b.getID())
        {
            out << b.getID() << "," << b.getp_id() << "," << b.geta_id()
                << "," << b.getAmount() << "," << b.getStatus() << ","
                << b.getDate() << "\n";
        }
        else
        {
            out << lines[i] << "\n";
        }
    }
    out.close();
}

void FileHandler::deletePatient(int id)
{
    ifstream file("patients.txt");
    if (!file.is_open())
        throw FileNotFoundException("Error: patients.txt could not be opened.");

    string lines[100];
    int lineCount = 0;
    string line;
    while (getline(file, line))
    {
        if (line != "")
            lines[lineCount++] = line;
    }
    file.close();

    ofstream out("patients.txt");
    for (int i = 0; i < lineCount; i++)
    {
        string fields[10];
        int fieldCount = 0;
        parseLine(lines[i], fields, fieldCount);
        int fid = 0;
        for (int j = 0; fields[0][j] != '\0'; j++)
            fid = fid * 10 + (fields[0][j] - '0');
        if (fid != id)
            out << lines[i] << "\n";
    }
    out.close();
}

void FileHandler::deleteDoctor(int id)
{
    ifstream file("doctors.txt");
    if (!file.is_open())
        throw FileNotFoundException("Error: doctors.txt could not be opened.");

    string lines[100];
    int lineCount = 0;
    string line;
    while (getline(file, line))
    {
        if (line != "")
            lines[lineCount++] = line;
    }
    file.close();

    ofstream out("doctors.txt");
    for (int i = 0; i < lineCount; i++)
    {
        string fields[10];
        int fieldCount = 0;
        parseLine(lines[i], fields, fieldCount);
        int fid = 0;
        for (int j = 0; fields[0][j] != '\0'; j++)
            fid = fid * 10 + (fields[0][j] - '0');
        if (fid != id)
            out << lines[i] << "\n";
    }
    out.close();
}

void FileHandler::deleteAppointmentsByPatientID(int patientID)
{
    ifstream file("appointments.txt");
    if (!file.is_open())
        throw FileNotFoundException("Error: appointments.txt could not be opened.");

    string lines[100];
    int lineCount = 0;
    string line;
    while (getline(file, line))
    {
        if (line != "")
            lines[lineCount++] = line;
    }
    file.close();

    ofstream out("appointments.txt");
    for (int i = 0; i < lineCount; i++)
    {
        string fields[10];
        int fieldCount = 0;
        parseLine(lines[i], fields, fieldCount);
        int pid = 0;
        for (int j = 0; fields[1][j] != '\0'; j++)
            pid = pid * 10 + (fields[1][j] - '0');
        if (pid != patientID)
            out << lines[i] << "\n";
    }
    out.close();
}

void FileHandler::deleteBillsByPatientID(int patientID)
{
    ifstream file("bills.txt");
    if (!file.is_open())
        throw FileNotFoundException("Error: bills.txt could not be opened.");

    string lines[100];
    int lineCount = 0;
    string line;
    while (getline(file, line))
    {
        if (line != "")
            lines[lineCount++] = line;
    }
    file.close();

    ofstream out("bills.txt");
    for (int i = 0; i < lineCount; i++)
    {
        string fields[10];
        int fieldCount = 0;
        parseLine(lines[i], fields, fieldCount);
        int pid = 0;
        for (int j = 0; fields[1][j] != '\0'; j++)
            pid = pid * 10 + (fields[1][j] - '0');
        if (pid != patientID)
            out << lines[i] << "\n";
    }
    out.close();
}

void FileHandler::deletePrescriptionsByPatientID(int patientID)
{
    ifstream file("prescriptions.txt");
    if (!file.is_open())
        throw FileNotFoundException("Error: prescriptions.txt could not be opened.");

    string lines[100];
    int lineCount = 0;
    string line;
    while (getline(file, line))
    {
        if (line != "")
            lines[lineCount++] = line;
    }
    file.close();

    ofstream out("prescriptions.txt");
    for (int i = 0; i < lineCount; i++)
    {
        string fields[10];
        int fieldCount = 0;
        parseLine(lines[i], fields, fieldCount);
        int pid = 0;
        for (int j = 0; fields[2][j] != '\0'; j++)
            pid = pid * 10 + (fields[2][j] - '0');
        if (pid != patientID)
            out << lines[i] << "\n";
    }
    out.close();
}

void FileHandler::deleteAppointment(int id)
{
    ifstream file("appointments.txt");
    if (!file.is_open())
        throw FileNotFoundException("Error: appointments.txt could not be opened.");

    string lines[100];
    int lineCount = 0;
    string line;
    while (getline(file, line))
    {
        if (line != "")
            lines[lineCount++] = line;
    }
    file.close();

    ofstream out("appointments.txt");
    for (int i = 0; i < lineCount; i++)
    {
        string fields[10];
        int fieldCount = 0;
        parseLine(lines[i], fields, fieldCount);
        int fid = 0;
        for (int j = 0; fields[0][j] != '\0'; j++)
            fid = fid * 10 + (fields[0][j] - '0');
        if (fid != id)
            out << lines[i] << "\n";
    }
    out.close();
}

void FileHandler::deleteBill(int id)
{
    ifstream file("bills.txt");
    if (!file.is_open())
        throw FileNotFoundException("Error: bills.txt could not be opened.");

    string lines[100];
    int lineCount = 0;
    string line;
    while (getline(file, line))
    {
        if (line != "")
            lines[lineCount++] = line;
    }
    file.close();

    ofstream out("bills.txt");
    for (int i = 0; i < lineCount; i++)
    {
        string fields[10];
        int fieldCount = 0;
        parseLine(lines[i], fields, fieldCount);
        int fid = 0;
        for (int j = 0; fields[0][j] != '\0'; j++)
            fid = fid * 10 + (fields[0][j] - '0');
        if (fid != id)
            out << lines[i] << "\n";
    }
    out.close();
}

void FileHandler::deletePrescription(int id)
{
    ifstream file("prescriptions.txt");
    if (!file.is_open())
        throw FileNotFoundException("Error: prescriptions.txt could not be opened.");

    string lines[100];
    int lineCount = 0;
    string line;
    while (getline(file, line))
    {
        if (line != "")
            lines[lineCount++] = line;
    }
    file.close();

    ofstream out("prescriptions.txt");
    for (int i = 0; i < lineCount; i++)
    {
        string fields[10];
        int fieldCount = 0;
        parseLine(lines[i], fields, fieldCount);
        int fid = 0;
        for (int j = 0; fields[0][j] != '\0'; j++)
            fid = fid * 10 + (fields[0][j] - '0');
        if (fid != id)
            out << lines[i] << "\n";
    }
    out.close();
}

void FileHandler::appendSecurityLog(string timestamp, string role, string enteredID, string result)
{
    ofstream file("security_log.txt", ios::app);
    if (!file.is_open())
        throw FileNotFoundException("Error: security_log.txt could not be opened.");
    file << timestamp << "," << role << "," << enteredID << "," << result << "\n";
    file.close();
}

void FileHandler::dischargePatient(int id)
{
    ifstream pfile("patients.txt");
    if (!pfile.is_open())
        throw FileNotFoundException("Error: patients.txt could not be opened.");

    ofstream dfile("discharged.txt", ios::app);
    if (!dfile.is_open())
        throw FileNotFoundException("Error: discharged.txt could not be opened.");

    string line;
    while (getline(pfile, line))
    {
        if (line == "") continue;
        string fields[10];
        int fieldCount = 0;
        parseLine(line, fields, fieldCount);
        int fid = 0;
        for (int i = 0; fields[0][i] != '\0'; i++)
            fid = fid * 10 + (fields[0][i] - '0');
        if (fid == id)
        {
            dfile << line << "\n";
            break;
        }
    }
    pfile.close();
    dfile.close();

    deletePatient(id);
    deleteAppointmentsByPatientID(id);
    deleteBillsByPatientID(id);
    deletePrescriptionsByPatientID(id);
}