#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstdio>
#include <cctype>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

struct user {
    int id;
    string login, password;
};

struct contact {
    int id, userId;
    string name, secondName, emailAddress, phoneNumber, address;

};

string readLine() {
    string input;
    cin.sync();
    getline(cin, input);

    return input;
}

void populateUserBase(vector <user>& users, int& userId) {
    fstream usersBase;
    string line, tempId;
    user userData;

    usersBase.open("usersBase.txt", ios::in);
    if (usersBase.good()==false) {
        ofstream userBase("usersBase.txt", ios::out | ios::app | ios::in);
        userId = 1;
    } else {

        while(getline(usersBase, line)) {
            std::stringstream singleUserData(line);

            getline(singleUserData, tempId, '|');
            userData.id = stoi(tempId);
            userId = stoi(tempId) + 1;
            getline(singleUserData, userData.login, '|');
            getline(singleUserData, userData.password, '|');

            users.push_back(userData);

        }
    }
    usersBase.close();
}


void checkIfUserExists(vector <user>& users, string &login) {

    for (const auto& singleUserData : users) {
        if (singleUserData.login == login) {
            cout << "Wybrany login juz istnieje, wybierz inny" << endl;
            cout << "Podaj login: ";
            login = readLine();
            checkIfUserExists(users, login);
        }
    }
}

void registerNewUser(vector <user>& users, int &userId) {

    string login, password = "a", passwordCheck = "b";
    user userData;

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


    userData.id = userId;
    userData.login = login;
    userData.password = password;


    users.push_back(userData);

    userId++;

    system("CLS");
}

void writeUsersToFile(vector<user>& users) {
    ofstream usersBase("usersBase.txt", ios::out | ios::trunc);

    for (const auto& userData : users) {
        usersBase   << userData.id << "|"
                    << userData.login << "|"
                    << userData.password << endl;
    }
    usersBase.close();
}

bool login(vector<user>& users, int &userId) {
    string searchedLogin, password;
    bool loginSuccessful;

    cout << "Podaj Login: " << endl;
    searchedLogin = readLine();

    for (const auto& singleUserData : users) {
        if (singleUserData.login == searchedLogin) {
            cout << "Podaj haslo: ";
            password = readLine();
            if (password == singleUserData.password) {
                userId = singleUserData.id;
                loginSuccessful = true;
            } else {
                loginSuccessful = false;
            }
        }
    }
    if(loginSuccessful) {
        cout << "Zalogowano" << endl;
        system("pause");
        system("CLS");
        return true;
    } else {
        cout << "Login lub haslo niepoprawne" << endl;
        system("pause");
        system("CLS");
        return false;
    }
}

void writeContactsToFile(vector<contact>& phoneBookContacts, int userId) {
    fstream phoneBook;
    ofstream tempPhoneBook;
    string line;

    phoneBook.open("phoneBook.txt", ios::in);
    if (phoneBook.good()==false) {
        ofstream phoneBook("phoneBook.txt", ios::out | ios::app | ios::in);
        for (const auto& contact : phoneBookContacts) {
            phoneBook   << contact.id << "|"
                        << contact.userId << "|"
                        << contact.name << "|"
                        << contact.secondName << "|"
                        << contact.emailAddress << "|"
                        << contact.phoneNumber << "|"
                        << contact.address << endl;
        }
    } else {
        ofstream tempPhoneBook("tempPhoneBook.txt", ios::out | ios::app | ios::in);
        while (getline(phoneBook, line)) {
            string tempId, tempUserId;
            bool contactFound = false;
            stringstream singleContactData(line);
            getline(singleContactData, tempId, '|');
            getline(singleContactData, tempUserId, '|');

            for (auto it = phoneBookContacts.begin(); it != phoneBookContacts.end(); ) {
                const auto& contact = *it;
                if (contact.id == stoi(tempId)) {
                    tempPhoneBook   << contact.id << "|"
                                    << contact.userId << "|"
                                    << contact.name << "|"
                                    << contact.secondName << "|"
                                    << contact.emailAddress << "|"
                                    << contact.phoneNumber << "|"
                                    << contact.address << endl;
                    it = phoneBookContacts.erase(it);
                    contactFound = true;
                    break;
                } else {
                    it++;
                    break;
                }
            }
            if (!contactFound && stoi(tempUserId) != userId) {
                tempPhoneBook << line << endl;
            }
        }
        for (const auto& contact : phoneBookContacts) {
            tempPhoneBook   << contact.id << "|"
                            << contact.userId << "|"
                            << contact.name << "|"
                            << contact.secondName << "|"
                            << contact.emailAddress << "|"
                            << contact.phoneNumber << "|"
                            << contact.address << endl;
        }
    }
    tempPhoneBook.close();
    phoneBook.close();

    phoneBookContacts.clear();
    remove("phoneBook.txt");
    rename("tempPhoneBook.txt", "phoneBook.txt");
    cout << endl << "Wylogowano" << endl;
    system("pause");
    system("CLS");
}


void searchBookByFirstName(vector <contact>& phoneBookContacts) {
    string searchedName;
    bool contactFound;

    cout << "Wpisz imie wyszukiwanego kontaktu: " << endl;

    searchedName = readLine();

    for (const auto& singleContactData : phoneBookContacts) {
        if (singleContactData.name == searchedName) {
            cout << endl <<"ID: " << singleContactData.id << endl;
            cout << "Imie: " << singleContactData.name << endl;
            cout << "Nazwisko: " << singleContactData.secondName << endl;
            cout << "Numer telefonu: " << singleContactData.phoneNumber << endl;
            cout << "Email: " << singleContactData.emailAddress << endl;
            cout << "Adres: " << singleContactData.address << endl << endl;
            contactFound = true;
        }
    }
    if (!contactFound) {
        cout << "Brak wyszukiwanego kontaktu" << endl;
    }

    system("pause");
    system("CLS");
}

void searchBookBySecondName(vector <contact>& phoneBookContacts) {
    string searchedSecondName;
    bool contactFound;

    cout << "Wpisz nazwisko wyszukiwanego kontaktu: " << endl;

    searchedSecondName = readLine();

    for (const auto& singleContactData : phoneBookContacts) {
        if (singleContactData.name == searchedSecondName) {
            cout << endl <<"ID: " << singleContactData.id << endl;
            cout << "Imie: " << singleContactData.name << endl;
            cout << "Nazwisko: " << singleContactData.secondName << endl;
            cout << "Numer telefonu: " << singleContactData.phoneNumber << endl;
            cout << "Email: " << singleContactData.emailAddress << endl;
            cout << "Adres: " << singleContactData.address << endl << endl;
            contactFound = true;
        }
    }
    if (!contactFound) {
        cout << "Brak wyszukiwanego kontaktu" << endl;
    }

    system("pause");
    system("CLS");
}

void searchBook(vector <contact>& phoneBookContacts, int userId) {
    int choice = 0;

    cout << "Wyszukiwanie kontakow:" << endl;
    cout << "1. Wyszukaj kontaktu po imieniu" << endl;
    cout << "2. Wyszukaj kontaktu po nazwisku" << endl;
    cout << "Wpisz cyfre i zatwierdz enterem w celu wybrania opcji: ";

    cin >> choice;

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

void displayAllContacts(vector <contact>& phoneBookContacts, int userId) {

    cout << "Wyswietlam wszystkie kontakty: " << endl;

    for (int i = 0; i < phoneBookContacts.size(); i++) {

        cout << endl <<"ID: " << phoneBookContacts[i].id << endl;
        cout << "Imie: " << phoneBookContacts[i].name << endl;
        cout << "Nazwisko: " << phoneBookContacts[i].secondName << endl;
        cout << "Numer telefonu: " << phoneBookContacts[i].phoneNumber << endl;
        cout << "Email: " << phoneBookContacts[i].emailAddress << endl;
        cout << "Adres: " << phoneBookContacts[i].address << endl << endl;

    }

    system("pause");
    system("CLS");
}

void createEntry(vector <contact>& phoneBookContacts, int &id, int userId) {

    string name, secondName, emailAddress, phoneNumber, address;
    contact contactDetails;

    cout << "Witaj w kreatorze kontaktow." << endl;
    cout << "Podaj imie: ";
    name = readLine();
    cout << "Podaj nazwisko: ";
    secondName = readLine();
    cout << "Podaj numer telefonu: ";
    phoneNumber = readLine();
    cout << "Podaj adres email: ";
    emailAddress = readLine();
    cout << "Podaj pelen adres: ";
    address = readLine();

    contactDetails.id = id;
    contactDetails.userId = userId;
    contactDetails.name = name;
    contactDetails.secondName = secondName;
    contactDetails.emailAddress = emailAddress;
    contactDetails.phoneNumber = phoneNumber;
    contactDetails.address = address;

    phoneBookContacts.push_back(contactDetails);

    id++;

    system("CLS");
}

void deleteEntry(vector<contact>& phoneBookContacts, int& id, int userId) {
    char choice;
    int searchedID;

    cout << "Podaj ID kontaktu, ktory chcesz usunac: ";
    cin >> id;

    auto it = find_if(phoneBookContacts.begin(), phoneBookContacts.end(), [id](const contact& element) {
        return element.id == id;
    });
    if (it != phoneBookContacts.end()) {
        searchedID = distance(phoneBookContacts.begin(), it);

        cout << endl << "ID: " << phoneBookContacts[searchedID].id << endl;
        cout << "Imiê: " << phoneBookContacts[searchedID].name << endl;
        cout << "Nazwisko: " << phoneBookContacts[searchedID].secondName << endl;
        cout << "Numer telefonu: " << phoneBookContacts[searchedID].phoneNumber << endl;
        cout << "Email: " << phoneBookContacts[searchedID].emailAddress << endl;
        cout << "Adres: " << phoneBookContacts[searchedID].address << endl << endl;

        cout << "Czy na pewno chcesz usunac wybrany kontakt? T/N" << endl;
        cin >> choice;

        if (choice == 'T' || choice == 't') {
            phoneBookContacts.erase(
            remove_if(phoneBookContacts.begin(), phoneBookContacts.end(), [&](contact const& singleContact) {
                return singleContact.id == id;
            }),
            phoneBookContacts.end());
        }
        system("pause");
    } else {
        cout << "Nie znaleziono kontaktu o podanym ID." << endl;
        system("pause");
    }
    id = (phoneBookContacts.back()).id + 1;
    system("CLS");
}

void populatePhoneBookVector(vector <contact>& phoneBookContacts, int& id, int userId) {
    fstream phoneBook;
    string line, tempId;
    contact contactDetails;


    phoneBook.open("phoneBook.txt", ios::in);
    if (phoneBook.good()==false) {
        id = 1;
    } else {

        while(getline(phoneBook, line)) {
            std::stringstream singleContactData(line);

            getline(singleContactData, tempId, '|');
            contactDetails.id = stoi(tempId);
            id = stoi(tempId) + 1;
            getline(singleContactData, tempId, '|');
            contactDetails.userId = stoi(tempId);
            getline(singleContactData, contactDetails.name, '|');
            getline(singleContactData, contactDetails.secondName, '|');
            getline(singleContactData, contactDetails.emailAddress, '|');
            getline(singleContactData, contactDetails.phoneNumber, '|');
            getline(singleContactData, contactDetails.address, '|');

            if (contactDetails.userId == userId) {
                phoneBookContacts.push_back(contactDetails);
            }
        }
    }
    phoneBook.close();
}

void displayContactByID(vector <contact>& phoneBookContacts, int id, int userId) {
    for (auto& singleContactData : phoneBookContacts) {
        if (singleContactData.id == id) {
            cout << endl <<"ID: " << singleContactData.id << endl;
            cout << "Imie: " << singleContactData.name << endl;
            cout << "Nazwisko: " << singleContactData.secondName << endl;
            cout << "Numer telefonu: " << singleContactData.phoneNumber << endl;
            cout << "Email: " << singleContactData.emailAddress << endl;
            cout << "Adres: " << singleContactData.address << endl << endl;
        }
    }
}

void editContact(vector <contact>& phoneBookContacts, int userId) {
    int id, choice;
    bool contactFound = false;

    cout << "Podaj ID kontaktu ktory chcesz edytowac: " << endl;
    cin >> id;
    cout << "Dane kontaktu przed edycja:" << endl;
    displayContactByID(phoneBookContacts, id, userId);

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

            switch(choice) {
            case 1:
                cout << "Podaj nowe imie: ";
                singleContactData.name = readLine();
                break;
            case 2:
                cout << "Podaj nowe nazwisko: ";
                singleContactData.secondName = readLine();
                break;
            case 3:
                cout << "Podaj nowy numer telefonu: ";
                singleContactData.phoneNumber = readLine();
                break;
            case 4:
                cout << "Podaj nowy email: ";
                singleContactData.emailAddress = readLine();
                break;
            case 5:
                cout << "Podaj nowy adres: ";
                singleContactData.address = readLine();
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
        displayContactByID(phoneBookContacts, id, userId);
    }

    system("pause");
    system("CLS");
}

void changePassword(vector <user>& users, int userId) {
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
                writeUsersToFile(users);
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

void openPhoneBook(int userId, vector <user>& users) {

    int menuChoice, id;

    vector <contact> phoneBookContacts;

    populatePhoneBookVector(phoneBookContacts, id, userId);

    while (menuChoice != 7) {

        menuChoice = 0;
        cout << "Witaj w aplikacji ksiazka adresowa, oto dostepne opcje:" << endl;
        cout << "1. Utworz nowy wpis w ksiazce." << endl;
        cout << "2. Przeszukaj ksiazke." << endl;
        cout << "3. Wyswietl wszystkie wpisy w ksiazce." << endl;
        cout << "4. Edycja kontaktow" << endl;
        cout << "5. Usuwanie kontakow" << endl;
        cout << "6. Zmien haslo" << endl;
        cout << "7. Wyloguj" << endl;
        cout << "Wpisz cyfre i zatwierdz enterem w celu wybrania opcji: ";

        cin >> menuChoice;

        switch (menuChoice) {
        case 1:
            createEntry(phoneBookContacts, id, userId);
            break;
        case 2:
            searchBook(phoneBookContacts, userId);
            break;
        case 3:
            displayAllContacts(phoneBookContacts,userId);
            break;
        case 4:
            editContact(phoneBookContacts, userId);
            break;
        case 5:
            deleteEntry(phoneBookContacts, id, userId);
            break;
        case 6:
            changePassword(users, userId);
            break;
        case 7:
            writeContactsToFile(phoneBookContacts, userId);
            break;
        }
    }
}

int main() {
    int userId, menuChoice;
    vector <user> users;
    populateUserBase(users, userId);



    while (menuChoice != 3) {

        menuChoice = 0;
        cout << "Witaj w aplikacji ksiazka adresowa, oto dostepne opcje:" << endl;
        cout << "1. Logowanie." << endl;
        cout << "2. Rejestracja." << endl;
        cout << "3. Zamknij Program." << endl;


        cin >> menuChoice;

        switch (menuChoice) {
        case 1:
            if (login(users, userId)) {
                openPhoneBook(userId, users);
            }
            break;
        case 2:
            registerNewUser(users, userId);
            break;
        case 3:
            writeUsersToFile(users);
            break;
        }
    }


    return 0;
}
