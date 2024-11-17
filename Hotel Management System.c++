#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib> // For exit()

using namespace std;

void intro() {
    cout << "\n\n\tC++ Project On Hotel Management System";
   
}

void clearScreen() {
    cout << "\033[2J\033[1;1H"; // ANSI escape code for clearing console
}

void head() {
    clearScreen();
    cout << "=============================================\n";
    cout << "\t\tXYZ Group of Hotels\n";
    cout << "=============================================\n";
}

void loading() {
    cout << "\nConnecting to Server\nSyncing Data";
    for (int i = 0; i < 10; ++i) {
        cout << ".";
        for (volatile int j = 0; j < 10000000; ++j); // Simulate loading
    }
    cout << "\n";
}

class Hotel {
private:
    int room_no;
    string name;
    string address;
    string phone;
    int days;
    float fare;

public:
    void main_menu();
    void add();
    void display();
    void rooms();
    void edit();
    int check(int);
    void modify(int);
    void delete_rec(int);
};

void Hotel::main_menu() {
    int choice;
    do {
        head();
        cout << "\n\t\tMAIN MENU";
        cout << "\n1. Book A Room";
        cout << "\n2. Customer Record";
        cout << "\n3. Rooms Allotted";
        cout << "\n4. Edit Record";
        cout << "\n5. Exit";
        cout << "\nEnter Your Choice: ";
        cin >> choice;

        switch (choice) {
            case 1: add(); break;
            case 2: display(); break;
            case 3: rooms(); break;
            case 4: edit(); break;
            case 5: cout << "\nExiting...\n"; break;
            default: cout << "\nInvalid choice. Try again.\n";
        }
    } while (choice != 5);
}

void Hotel::add() {
    head();
    int r;
    cout << "\nEnter Room Number: ";
    cin >> r;

    if (check(r)) {
        cout << "\nSorry, Room is already booked.\n";
        return;
    }

    room_no = r;
    cin.ignore(); // Clear newline character
    cout << "Name: ";
    getline(cin, name);
    cout << "Address: ";
    getline(cin, address);
    cout << "Phone: ";
    getline(cin, phone);
    cout << "No of Days to Checkout: ";
    cin >> days;
    fare = days * 900;

    ofstream fout("Record.dat", ios::app | ios::binary);
    fout.write(reinterpret_cast<char*>(this), sizeof(Hotel));
    fout.close();

    cout << "\nRoom is booked successfully.\n";
}

void Hotel::display() {
    head();
    int r, found = 0;
    cout << "\nEnter Room Number: ";
    cin >> r;

    ifstream fin("Record.dat", ios::in | ios::binary);
    while (fin.read(reinterpret_cast<char*>(this), sizeof(Hotel))) {
        if (room_no == r) {
            cout << "\nCustomer Details:\n";
            cout << "Room No: " << room_no << "\n";
            cout << "Name: " << name << "\n";
            cout << "Address: " << address << "\n";
            cout << "Phone: " << phone << "\n";
            cout << "Days: " << days << "\n";
            cout << "Total Fare: " << fare << "\n";
            found = 1;
            break;
        }
    }
    fin.close();

    if (!found) cout << "\nRoom not found or vacant.\n";
}

void Hotel::rooms() {
    head();
    cout << "\nList of Rooms Allotted:\n";

    ifstream fin("Record.dat", ios::in | ios::binary);
    while (fin.read(reinterpret_cast<char*>(this), sizeof(Hotel))) {
        cout << "\nRoom No: " << room_no << "\n";
        cout << "Name: " << name << "\n";
        cout << "Address: " << address << "\n";
        cout << "Phone: " << phone << "\n";
        cout << "Days: " << days << "\n";
        cout << "Total Fare: " << fare << "\n";
        cout << "--------------------------\n";
    }
    fin.close();
}

void Hotel::edit() {
    head();
    int choice, r;
    cout << "\nEDIT MENU";
    cout << "\n1. Modify Customer Record";
    cout << "\n2. Delete Customer Record";
    cout << "\nEnter Your Choice: ";
    cin >> choice;

    cout << "\nEnter Room Number: ";
    cin >> r;

    switch (choice) {
        case 1: modify(r); break;
        case 2: delete_rec(r); break;
        default: cout << "\nInvalid choice.\n";
    }
}

int Hotel::check(int r) {
    ifstream fin("Record.dat", ios::in | ios::binary);
    while (fin.read(reinterpret_cast<char*>(this), sizeof(Hotel))) {
        if (room_no == r) {
            fin.close();
            return 1;
        }
    }
    fin.close();
    return 0;
}

void Hotel::modify(int r) {
    head();
    fstream file("Record.dat", ios::in | ios::out | ios::binary);
    int found = 0;
    while (file.read(reinterpret_cast<char*>(this), sizeof(Hotel))) {
        if (room_no == r) {
            cout << "\nEnter New Details:\n";
            cin.ignore();
            cout << "Name: ";
            getline(cin, name);
            cout << "Address: ";
            getline(cin, address);
            cout << "Phone: ";
            getline(cin, phone);
            cout << "No of Days: ";
            cin >> days;
            fare = days * 900;

            file.seekp(-static_cast<int>(sizeof(Hotel)), ios::cur);
            file.write(reinterpret_cast<char*>(this), sizeof(Hotel));
            cout << "\nRecord modified successfully.\n";
            found = 1;
            break;
        }
    }
    file.close();
    if (!found) cout << "\nRoom not found or vacant.\n";
}

void Hotel::delete_rec(int r) {
    head();
    ifstream fin("Record.dat", ios::in | ios::binary);
    ofstream fout("Temp.dat", ios::out | ios::binary);
    int found = 0;

    while (fin.read(reinterpret_cast<char*>(this), sizeof(Hotel))) {
        if (room_no == r) {
            found = 1;
            cout << "\nRecord Deleted Successfully.\n";
        } else {
            fout.write(reinterpret_cast<char*>(this), sizeof(Hotel));
        }
    }
    fin.close();
    fout.close();

    if (found) {
        remove("Record.dat");
        rename("Temp.dat", "Record.dat");
    } else {
        cout << "\nRoom not found or vacant.\n";
    }
}

int main() {
    Hotel h;
    intro();
    loading();
    h.main_menu();
    return 0;
}
