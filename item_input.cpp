#include <iostream>
#include <array>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

// Forward declaration
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
    int getPrice() const;
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

vector<array<string, 2>> loadDestinations();
void simpanKeCSV(const string& namaFile, const vector<vector<string>>& data);

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
    cout << "Weight (kg)\t: ";
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

int Item::getPrice(const unordered_map<string, int>& priceMap) const {
    auto it = priceMap.find(destination);
    if (it != priceMap.end()) {
        return it->second;
    } else {
        return 0; // Jika harga tidak ditemukan, kembalikan 0 atau harga default
    }
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

string ItemQueue::itemCSVLine(const Item& itemDetails, const unordered_map<string, int>& priceMap) const {
    return itemDetails.getItemName() + "," +
           to_string(itemDetails.getWeight()) + "," +
           itemDetails.getOrigin() + "," +
           itemDetails.getDestination() + "," +
           to_string(itemDetails.getPrice(priceMap));
}

void ItemQueue::displayQueue(const unordered_map<string, int>& priceMap) const {
    if (!isEmpty()) {
        cout << "Displaying current items in queue (not written in CSV) (" << top << " items)." << endl;
        cout << "Item Name\t\t" << "Weight\t" << "Destination\t" << "Price\t" << endl;
        for (int i = 0; i < top; i++) {
            cout << left << setw(20) << itemQueue[i].getItemName() << "\t" 
                 << itemQueue[i].getWeight() << "\t" 
                 << itemQueue[i].getDestination() << "\t"
                 << itemQueue[i].getPrice(priceMap) << endl;
        }
    } else {
        cout << "Queue is empty." << endl;
    }
}

vector<vector<string>> ItemQueue::getQueueData(const unordered_map<string, int>& priceMap) const {
    vector<vector<string>> data;
    for (int i = 0; i < top; ++i) {
        data.push_back({
            itemQueue[i].getItemName(),
            to_string(itemQueue[i].getWeight()),
            itemQueue[i].getOrigin(),
            itemQueue[i].getDestination(),
            to_string(itemQueue[i].getPrice(priceMap))
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

unordered_map<string, int> loadPrices() {
    unordered_map<string, int> priceMap;

    ifstream priceFile("data/MST Result.csv");
    if (!priceFile.is_open()) {
        cout << "Price file failed to load." << endl;
    } else {
        string line;
        while (getline(priceFile, line)) {
            stringstream ss(line);
            string destination;
            int price;
            if (getline(ss, destination, ',') && ss >> price) {
                priceMap[destination] = price;
            }
        }
    }
    priceFile.close();
    return priceMap;
}

void simpanKeCSV(const string& namaFile, const vector<vector<string>>& data) {
    ofstream file(namaFile);

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
    unordered_map<string, int> priceMap = loadPrices();
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

                            if (item.verifyLocation(destinations, destination)) {
                                cout << "\n==== DETAIL BARANG ====" << endl;
                                cout << "Nama barang\t: " << itemName << endl;
                                cout << "Asal\t\t: " << origin << endl;
                                cout << "Tujuan\t\t: " << destination << endl;
                                cout << "Prov. Tujuan\t: " << province << endl;
                                cout << "Berat (kg)\t: " << weight << endl;
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
                            queue.displayQueue(priceMap);
                            break;
                        case 3: {
                            vector<vector<string>> data = queue.getQueueData(priceMap);
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