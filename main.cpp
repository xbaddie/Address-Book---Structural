#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstdio>
#include <cctype>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;



struct User {
    int id;
    string login, password;
};

struct Contact {
    int id, userId;
    string name, secondName, emailAddress, phoneNumber, address;

};

string readLine() {
    string input;
    cin.sync();
    getline(cin, input);

    return input;
}

void readContactData(Contact singleContactData) {
    cout << endl <<"ID: " << singleContactData.id << endl;
    cout << "Imie: " << singleContactData.name << endl;
    cout << "Nazwisko: " << singleContactData.secondName << endl;
    cout << "Numer telefonu: " << singleContactData.phoneNumber << endl;
    cout << "Email: " << singleContactData.emailAddress << endl;
    cout << "Adres: " << singleContactData.address << endl << endl;
}

void wrongChoiceErrorHandler() {
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cout << "Nieprawidlowy wybor" << endl;
    system("pause");
    system("CLS");
}

void populateUserBase(vector <User>& users) {
    fstream usersBase;
    string line, tempId;
    User userData;

    usersBase.open("usersBase.txt", ios::in);
    if (usersBase.good()==false) {
        ofstream userBase("usersBase.txt", ios::out | ios::app | ios::in);
    } else {

        while(getline(usersBase, line)) {
            std::stringstream singleUserData(line);
            getline(singleUserData, tempId, '|');
            userData.id = stoi(tempId);
            getline(singleUserData, userData.login, '|');
            getline(singleUserData, userData.password, '|');
            users.push_back(userData);

        }
    }
    usersBase.close();
}

void writeUserToFile(vector<User>& users) {
    ofstream usersBase("usersBase.txt", ios::out | ios::app);

    usersBase   << users.back().id << "|"
                << users.back().login << "|"
                << users.back().password << endl;

    usersBase.close();
}

void checkIfUserExists(vector <User>& users, string login) {

    for (const auto& singleUserData : users) {
        if (singleUserData.login == login) {
            cout << "Wybrany login juz istnieje, wybierz inny" << endl;
            cout << "Podaj login: ";
            login = readLine();
            checkIfUserExists(users, login);
        }
    }
}

int getNewUserId(vector <User>& users) {
    if (users.empty()) {
        return 1;
    } else {
        return users.back().id + 1;
    }
}

void registerNewUser(vector <User>& users) {
    string login, password = "a", passwordCheck = "b";
    int newUserId;
    User userData;

    newUserId = getNewUserId(users);

    cout << "Witaj w kreatorze uzytkownikow." << endl;
    cout << "Podaj login: ";

    login = readLine();
    checkIfUserExists(users, login);

    while(password != passwordCheck) {
        cout << "Podaj haslo: ";
        password = readLine();
        cout << "Ponownie podaj haslo: ";
        passwordCheck = readLine();

        if (password != passwordCheck) {
            cout << "Hasla sie roznia, podaj haslo jeszcze raz" << endl;
        }
    }

    userData.id = newUserId;
    userData.login = login;
    userData.password = password;


    users.push_back(userData);
    writeUserToFile(users);
    system("CLS");
}

int login(vector<User>& users) {
    string searchedLogin, password;

    cout << "Podaj Login: " << endl;
    searchedLogin = readLine();

    for (const auto& singleUserData : users) {
        if (singleUserData.login == searchedLogin) {
            cout << "Podaj haslo: ";
            password = readLine();
            if (password == singleUserData.password) {
                cout << "Zalogowano" << endl;
                system("pause");
                system("CLS");
                return singleUserData.id;
            }

        }
    }
    cout << "Login lub haslo niepoprawne" << endl;
    system("pause");
    system("CLS");
    return 0;
}

void renameFiles(fstream &phoneBook, ofstream &tempPhoneBook) {
    tempPhoneBook.close();
    phoneBook.close();

    remove("phoneBook.txt");
    rename("tempPhoneBook.txt", "phoneBook.txt");
}

void searchBookByFirstName(vector <Contact>& phoneBookContacts) {
    string searchedName;
    bool contactFound = false;

    cout << "Wpisz imie wyszukiwanego kontaktu: " << endl;

    searchedName = readLine();

    for (const auto& singleContactData : phoneBookContacts) {
        if (singleContactData.name == searchedName) {
            readContactData(singleContactData);
            contactFound = true;
        }
    }
    if (!contactFound) {
        cout << "Brak wyszukiwanego kontaktu" << endl;
    }

    system("pause");
    system("CLS");
}

void searchBookBySecondName(vector <Contact>& phoneBookContacts) {
    string searchedSecondName;
    bool contactFound = false;

    cout << "Wpisz nazwisko wyszukiwanego kontaktu: " << endl;

    searchedSecondName = readLine();

    for (const auto& singleContactData : phoneBookContacts) {
        if (singleContactData.secondName == searchedSecondName) {
            readContactData(singleContactData);
            contactFound = true;
        }
    }
    if (!contactFound) {
        cout << "Brak wyszukiwanego kontaktu" << endl;
    }

    system("pause");
    system("CLS");
}

void searchBook(vector <Contact>& phoneBookContacts) {
    int choice = 0;

    cout << "Wyszukiwanie kontakow:" << endl;
    cout << "1. Wyszukaj kontaktu po imieniu" << endl;
    cout << "2. Wyszukaj kontaktu po nazwisku" << endl;
    cout << "Wpisz cyfre i zatwierdz enterem w celu wybrania opcji: ";

    cin >> choice;

    if (cin.fail()) {
        wrongChoiceErrorHandler();
    }

    switch (choice) {
    case 1:
        searchBookByFirstName(phoneBookContacts);
        break;
    case 2:
        searchBookBySecondName(phoneBookContacts);
        break;

        system("CLS");
    }
}

void displayAllContacts(vector <Contact>& phoneBookContacts) {

    cout << "Wyswietlam wszystkie kontakty: " << endl;

    for (size_t i = 0; i < phoneBookContacts.size(); i++) {
        readContactData(phoneBookContacts[i]);
    }

    system("pause");
    system("CLS");
}

void writeDataToFile(Contact contactDetails, ofstream &file) {
    file   << contactDetails.id << "|"
           << contactDetails.userId << "|"
           << contactDetails.name << "|"
           << contactDetails.secondName << "|"
           << contactDetails.emailAddress << "|"
           << contactDetails.phoneNumber << "|"
           << contactDetails.address << endl;

    file.close();
}

int createEntry(vector <Contact>& phoneBookContacts, int idOfLastRecordInBook, int userId) {
    ofstream phoneBook("phoneBook.txt", ios::out | ios::app);
    Contact contactDetails;

    cout << "Witaj w kreatorze kontaktow." << endl;
    cout << "Podaj imie: ";
    contactDetails.name = readLine();
    cout << "Podaj nazwisko: ";
    contactDetails.secondName = readLine();
    cout << "Podaj numer telefonu: ";
    contactDetails.phoneNumber = readLine();
    cout << "Podaj adres email: ";
    contactDetails.emailAddress = readLine();
    cout << "Podaj pelen adres: ";
    contactDetails.address = readLine();

    contactDetails.id = idOfLastRecordInBook + 1;
    idOfLastRecordInBook = idOfLastRecordInBook + 1;
    contactDetails.userId = userId;

    phoneBookContacts.push_back(contactDetails);
    writeDataToFile(contactDetails, phoneBook);

    system("CLS");
    return idOfLastRecordInBook;
}

void deleteContactFromVector(vector<Contact>& phoneBookContacts, int idOfRecordToDelete) {
    phoneBookContacts.erase(
    remove_if(phoneBookContacts.begin(), phoneBookContacts.end(), [&](Contact const& singleContact) {
        return singleContact.id == idOfRecordToDelete;
    }),
    phoneBookContacts.end());
}

int deleteContactFromPhoneBook(vector<Contact>& phoneBookContacts, int idOfLastRecordInBook) {
    fstream phoneBook("phoneBook.txt", ios::in);
    ofstream tempPhoneBook("tempPhoneBook.txt", ios::out | ios::app);
    string line;
    bool contactFound = false;

    char choice;
    int idOfRecordToDelete;

    cout << "Podaj ID kontaktu, ktory chcesz usunac: ";
    cin >> idOfRecordToDelete;
    cout << "Czy na pewno chcesz usunac wybrany kontakt? T/N" << endl;
    cin >> choice;

    if (choice == 'T' || choice == 't') {
        deleteContactFromVector(phoneBookContacts, idOfRecordToDelete);
        while (getline(phoneBook, line)) {
            string tempId;
            stringstream singleContactData(line);
            getline(singleContactData, tempId, '|');
            if (stoi(tempId) == idOfRecordToDelete) {
                contactFound = true;
            }
            if (stoi(tempId) != idOfRecordToDelete) {
                tempPhoneBook << line << endl;
            }
        }
    }

    if (!contactFound) {
        cout << endl << "Brak wyszukiwanego kontaktu" << endl;
        phoneBook.close();
        tempPhoneBook.close();
    } else {
        cout << endl << "Kontakt usuniety" << endl;
        renameFiles(phoneBook, tempPhoneBook);
    }

    if(idOfLastRecordInBook == idOfRecordToDelete && contactFound) {
        return idOfLastRecordInBook - 1;
    }
    return idOfLastRecordInBook;
}

int populatePhoneBookVector(vector <Contact>& phoneBookContacts, int userId) {
    fstream phoneBook;
    string line, tempId;
    int idOfLastRecordInBook;
    Contact contactDetails;

    phoneBook.open("phoneBook.txt", ios::in);
    if (phoneBook.good()==false) {
        idOfLastRecordInBook = 0;
    } else {

        while(getline(phoneBook, line)) {
            std::stringstream singleContactData(line);

            getline(singleContactData, tempId, '|');
            contactDetails.id = stoi(tempId);
            idOfLastRecordInBook = stoi(tempId);
            getline(singleContactData, tempId, '|');
            contactDetails.userId = stoi(tempId);

            if (contactDetails.userId != userId) {
                continue;
            }

            getline(singleContactData, contactDetails.name, '|');
            getline(singleContactData, contactDetails.secondName, '|');
            getline(singleContactData, contactDetails.emailAddress, '|');
            getline(singleContactData, contactDetails.phoneNumber, '|');
            getline(singleContactData, contactDetails.address, '|');
            phoneBookContacts.push_back(contactDetails);
        }
    }
    phoneBook.close();
    return idOfLastRecordInBook;
}

void displayContactByID(vector <Contact>& phoneBookContacts, int id) {
    for (auto& singleContactData : phoneBookContacts) {
        if (singleContactData.id == id) {
            readContactData(singleContactData);
        }
    }
}

void applyEditToFile(Contact contactData) {
    fstream phoneBook("phoneBook.txt", ios::in);
    ofstream tempPhoneBook("tempPhoneBook.txt", ios::out | ios::app);
    string line;

    while (getline(phoneBook, line)) {
        string tempId;
        stringstream singleContactData(line);
        getline(singleContactData, tempId, '|');
        if (stoi(tempId) == contactData.id) {
            writeDataToFile(contactData, tempPhoneBook);
        }
        if (stoi(tempId) != contactData.id) {
            tempPhoneBook << line << endl;
        }
    }
    renameFiles(phoneBook, tempPhoneBook);
}

void editContact(vector <Contact>& phoneBookContacts, int userId) {
    int id, choice;
    bool contactFound = false;

    cout << "Podaj ID kontaktu ktory chcesz edytowac: " << endl;
    cin >> id;
    cout << "Dane kontaktu przed edycja:" << endl;
    displayContactByID(phoneBookContacts, id);

    for (auto& singleContactData : phoneBookContacts) {
        if (singleContactData.id == id) {
            contactFound = true;
            cout << "Ktora wartosc chcesz edytowac ?" << endl;
            cout << "1. Imie" << endl;
            cout << "2. Nazwisko" << endl;
            cout << "3. Numer telefonu" << endl;
            cout << "4. Email" << endl;
            cout << "5. Adres" << endl;
            cout << "6. Wyjdz z menu edycji" << endl;

            cin >> choice;

            if (cin.fail()) {
                wrongChoiceErrorHandler();
            }

            switch(choice) {
            case 1:
                cout << "Podaj nowe imie: ";
                singleContactData.name = readLine();
                applyEditToFile(singleContactData);
                break;
            case 2:
                cout << "Podaj nowe nazwisko: ";
                singleContactData.secondName = readLine();
                applyEditToFile(singleContactData);
                break;
            case 3:
                cout << "Podaj nowy numer telefonu: ";
                singleContactData.phoneNumber = readLine();
                applyEditToFile(singleContactData);
                break;
            case 4:
                cout << "Podaj nowy email: ";
                singleContactData.emailAddress = readLine();
                applyEditToFile(singleContactData);
                break;
            case 5:
                cout << "Podaj nowy adres: ";
                singleContactData.address = readLine();
                applyEditToFile(singleContactData);
                break;
            case 6:
                system("CLS");
                return;
            }
            break;
        }
    }

    if (!contactFound) {
        cout << "Kontakt o podanym ID nie istnieje." << endl;
    } else {
        cout << "Dane kontaktu po edycji";
        displayContactByID(phoneBookContacts, id);
    }

    system("pause");
    system("CLS");
}

void changePassword(vector <User>& users, int userId) {
    bool userFound = false;
    string password, newPassword, passwordCheck;

    cout << "Wpisz swoje obecne haslo" << endl;
    cin >> password;

    for (auto& singleUserData : users) {
        if (singleUserData.id == userId) {
            userFound = true;
            if (singleUserData.password == password) {
                cout << "Podaj nowe haslo: " << endl;
                newPassword = readLine();
                cout << "Wpisz haslo ponownie: " << endl;
                passwordCheck = readLine();
                singleUserData.password = newPassword;
                writeUserToFile(users);
                cout << "Haslo zostalo zmienione" << endl;
                system("pause");
                system("CLS");
            }
            break;
        }

    }
    if (!userFound) {
        cout << "Uzytkownik nie jestnieje lub podane haslo jest nieprawidlowe." << endl;
        system("pause");
        system("CLS");
    }
}

void openPhoneBook(int userId, vector <User>& users) {

    int menuChoice = 0, idOfLastRecordInBook;

    vector <Contact> phoneBookContacts;

    idOfLastRecordInBook = populatePhoneBookVector(phoneBookContacts, userId);

    while (menuChoice != 7) {

        cout << "Witaj w aplikacji ksiazka adresowa, oto dostepne opcje:" << endl;
        cout << "1. Utworz nowy wpis w ksiazce." << endl;
        cout << "2. Przeszukaj ksiazke." << endl;
        cout << "3. Wyswietl wszystkie wpisy w ksiazce." << endl;
        cout << "4. Edycja kontaktow" << endl;
        cout << "5. Usuwanie kontakow" << endl;
        cout << "6. Zmien haslo" << endl;
        cout << "7. Wyloguj i zapisz zmiany" << endl;
        cout << "Wpisz cyfre i zatwierdz enterem w celu wybrania opcji: ";

        cin >> menuChoice;

        if (cin.fail()) {
            wrongChoiceErrorHandler();
        }

        switch (menuChoice) {
        case 1:
            idOfLastRecordInBook = createEntry(phoneBookContacts, idOfLastRecordInBook, userId);
            break;
        case 2:
            searchBook(phoneBookContacts);
            break;
        case 3:
            displayAllContacts(phoneBookContacts);
            break;
        case 4:
            editContact(phoneBookContacts, userId);
            break;
        case 5:
            idOfLastRecordInBook = deleteContactFromPhoneBook(phoneBookContacts, idOfLastRecordInBook);
            system("pause");
            system("CLS");
            break;
        case 6:
            changePassword(users, userId);
            break;
        case 7:
            phoneBookContacts.clear();
            cout << "Wylogowano" << endl;
            system("pause");
            system("CLS");
            break;
        }
    }
}

int main() {
    int loggedUserId = 0, menuChoice = 0;
    vector <User> users;

    populateUserBase(users);

    while (menuChoice != 3) {

        cout << "Witaj w aplikacji ksiazka adresowa, oto dostepne opcje:" << endl;
        cout << "1. Logowanie." << endl;
        cout << "2. Rejestracja." << endl;
        cout << "3. Zamknij Program." << endl;


        cin >> menuChoice;

        if (cin.fail()) {
            wrongChoiceErrorHandler();
        }

        switch (menuChoice) {
        case 1:
            loggedUserId = login(users);
            if ((loggedUserId != 0))  {
                openPhoneBook(loggedUserId, users);
            }
            break;
        case 2:
            registerNewUser(users);
            break;
        case 3:
            break;
        }
    }

    return 0;
}
