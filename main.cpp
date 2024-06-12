#include <iostream>
#include <string>
#include <conio.h>
#include <array>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "prim.cpp"

using namespace std;

struct Akun {
    string nama;
    string alamat;
    string nomorTelepon;
    string email;
    string password;
};

struct ItemQueue;

class Item {
private:
    string itemName;
    string origin;
    string destination;
    string province;
    float weight;

public:
    void input();
    string getItemName() const;
    string getOrigin() const;
    bool verifyLocation(const vector<array<string, 2>>& locations, const string& locationName) const;
    string getDestination() const;
    string getProvince() const;
    int getPrice(string destination, int itemWeight) const;
    float getWeight() const;
};

struct ItemQueue {
private:
    int top;
    Item itemQueue[500];

public:
    void createQueue();
    bool isEmpty() const;
    bool isFull() const;
    void insertQueue(const Item& itemToDeliver);
    string itemCSVLine(const Item& itemDetails) const;
    void displayQueue() const;
    vector<vector<string>> getQueueData() const;
};

string getHiddenPassword() {
    string password;
    char ch;
    while (true) {
        ch = _getch(); 
        if (ch == 13) { 
            cout << endl;
            break;
        } else if (ch == 8) {
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b";
            }
        } else {
            password.push_back(ch);
            cout << '*';
        }
    }
    return password;
}

void buatAkunBaru(Akun &akun) {
    cin.ignore();
    cout << "Masukkan nama: ";
    getline(cin, akun.nama);
    cout << "Masukkan alamat: ";
    getline(cin, akun.alamat);
    cout << "Masukkan nomor telepon: ";
    getline(cin, akun.nomorTelepon);
    cout << "Masukkan email: ";
    getline(cin, akun.email);
    cout << "Masukkan password: ";
    akun.password = getHiddenPassword();

    cout << "Akun baru berhasil dibuat!" << endl;
}

bool login(Akun &akun) {
    string email, passwordMasukan;

    cin.ignore(); // Membersihkan buffer
    cout << "Masukkan email: ";
    getline(cin, email);
    cout << "Masukkan password: ";
    passwordMasukan = getHiddenPassword();

    if (email == akun.email && passwordMasukan == akun.password) {
        cout << "Login berhasil!" << endl;
        return true;
    } else {
        cout << "Email atau password salah!" << endl;
        return false;
    }
}

void Item::input() {
    cin.ignore();
    cout << "Item Name\t: ";
    getline(cin, itemName);
    cout << "Origin\t\t: ";
    getline(cin, origin);
    cout << "Destination\t: ";
    getline(cin, destination);
    cout << "Province\t: ";
    getline(cin, province);
    cout << "Weight (g)\t: ";
    cin >> weight;
    cin.ignore(); // Clear the newline character from the buffer
}

string Item::getItemName() const {
    return itemName;
}

string Item::getOrigin() const {
    return origin;
}

bool Item::verifyLocation(const vector<array<string, 2>>& locations, const string& locationName) const {
    for (const auto& arr : locations) {
        if (locationName == arr[1]) {
            return true;
        }
    }
    return false;
}

string Item::getDestination() const {
    return destination;
}

string Item::getProvince() const {
    return province;
}

int Item::getPrice(string destination, int itemWeight) const {
    int totalRange = getRange(destination);
    int finalPrice;
    if (totalRange < 300 && itemWeight > 1){
        return (7000 * itemWeight);
    } else if (totalRange < 300 && itemWeight > 1){
        return 7000;
    } else {
        return (7000 * itemWeight * 25); // Rp 25 / km
    }


    return 0; 
}

float Item::getWeight() const {
    return (weight / 1000); // Convert weight from grams to kilograms
}

void ItemQueue::createQueue() {
    top = 0;
}

bool ItemQueue::isEmpty() const {
    return top == 0;
}

bool ItemQueue::isFull() const {
    return top >= 500;
}

void ItemQueue::insertQueue(const Item& itemToDeliver) {
    if (!isFull()) {
        itemQueue[top++] = itemToDeliver;
        cout << "Item " << itemToDeliver.getItemName() << " inserted into queue." << endl;
    } else {
        cout << "Queue is full. Item not inserted." << endl;
    }
}

string ItemQueue::itemCSVLine(const Item& itemDetails) const {
    return itemDetails.getItemName() + "," +
           to_string(itemDetails.getWeight()) + "," +
           itemDetails.getOrigin() + "," +
           itemDetails.getDestination() + "," +
           to_string(itemDetails.getPrice(itemDetails.getDestination(), itemDetails.getWeight()));
}

void ItemQueue::displayQueue() const {
    if (!isEmpty()) {
        cout << "Displaying current items in queue (not written in CSV) (" << top << " items)." << endl;
        cout << "Item Name\t\t" << "Weight\t" << "Destination\t" << endl;
        for (int i = 0; i < top; i++) {
            cout << left << setw(20) << itemQueue[i].getItemName() << "\t" 
                 << itemQueue[i].getWeight() << "\t" 
                 << itemQueue[i].getDestination() << endl;
        }
    } else {
        cout << "Queue is empty." << endl;
    }
}

vector<vector<string>> ItemQueue::getQueueData() const {
    vector<vector<string>> data;
    for (int i = 0; i < top; ++i) {
        data.push_back({
            itemQueue[i].getItemName(),
            to_string(itemQueue[i].getWeight()),
            itemQueue[i].getOrigin(),
            itemQueue[i].getDestination(),
            to_string(itemQueue[i].getPrice(itemQueue[i].getDestination(), itemQueue[i].getWeight()))
        });
    }
    return data;
}

vector<array<string, 2>> loadDestinations() {
    vector<array<string, 2>> destinations;

    ifstream cityFiles("data/destinations.csv");
    if (!cityFiles.is_open()) {
        cout << "Destination files failed to load." << endl;
    } else {
        string line;
        while (getline(cityFiles, line)) {
            stringstream ss(line);
            string province, city;
            if (getline(ss, province, ',') && getline(ss, city, ',')) {
                destinations.push_back({ province, city });
            }
        }
    }
    cityFiles.close();
    return destinations;
}

void simpanKeCSV(const string& namaFile, const vector<vector<string>>& data) {
    // Open the file in append mode
    ofstream file(namaFile, ios::app);

    if (file.is_open()) {
        for (const auto& baris : data) {
            for (size_t i = 0; i < baris.size(); ++i) {
                file << baris[i];
                if (i < baris.size() - 1) file << ","; // Tambahkan koma di antara kolom
            }
            file << "\n"; // Baris baru untuk setiap record
        }
        file.close();
        cout << "Data berhasil disimpan ke " << namaFile << endl;
    } else {
        cerr << "Tidak dapat membuka file untuk menulis!" << endl;
    }
}

int main() {
    Akun akun;
    vector<array<string, 2>> destinations = loadDestinations();
    ItemQueue queue;
    queue.createQueue();

    bool isLoggedIn = false;
    int mainSelection;

    while (!isLoggedIn) {
        int pilihan;
        cout << "\nMenu Akun Pengiriman Barang:" << endl;
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "3. Keluar" << endl;
        cout << "Masukkan pilihan: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                buatAkunBaru(akun);
                break;
            case 2:
                isLoggedIn = login(akun);
                break;
            case 3:
                cout << "Terima kasih telah menggunakan aplikasi!" << endl;
                return 0;
            default:
                cout << "Pilihan tidak valid!" << endl;
        }
    }

    do {
        cout << "\nMenu Utama:" << endl;
        cout << "1. Menu Barang" << endl;
        cout << "2. Keluar" << endl;
        cout << "Masukkan pilihan: ";
        cin >> mainSelection;

        switch (mainSelection) {
            case 1: {
                while (true) {
                    int selection;
                    cout << "1. Add item\n";
                    cout << "2. Display queue\n";
                    cout << "3. Exit\n";
                    cout << "Selection: "; cin >> selection;
                    switch (selection) {
                        case 1: {
                            string confirmation;
                            Item item;

                            item.input();
                            string itemName = item.getItemName();
                            string origin = item.getOrigin();
                            string destination = item.getDestination();
                            string province = item.getProvince();
                            float weight = item.getWeight();
                            int price = item.getPrice(destination, weight);

                            if (item.verifyLocation(destinations, destination)) {
                                cout << "\n==== DETAIL BARANG ====" << endl;
                                cout << "Nama barang\t: " << itemName << endl;
                                cout << "Asal\t\t: " << origin << endl;
                                cout << "Tujuan\t\t: " << destination << endl;
                                cout << "Prov. Tujuan\t: " << province << endl;
                                cout << "Berat (kg)\t: " << weight << endl;
                                cout << "Harga\t" << price << endl;
                                cout << "Tambah barang? (Y/N): ";
                                cin >> confirmation;
                                if (confirmation == "Y" || confirmation == "y") {
                                    queue.insertQueue(item);
                                } else {
                                    cout << "Barang tidak ditambahkan." << endl;
                                }
                            } else {
                                cout << "Destinasi tidak benar." << endl;
                            }
                            break;
                        }
                        case 2:
                            queue.displayQueue();
                            break;
                        case 3: {
                            vector<vector<string>> data = queue.getQueueData();
                            simpanKeCSV("to_send.csv", data);
                            return 0;
                        }
                        default:
                            cout << "Input salah!" << endl;
                    }
                }
                break;
            }
            case 2:
                cout << "Terima kasih telah menggunakan aplikasi!" << endl;
                break;
            default:
                cout << "Pilihan tidak valid!" << endl;
        }
    } while (mainSelection != 2);

    return 0;
}
