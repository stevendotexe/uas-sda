#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class Item {
public:
    string itemName;
    float weight;
    string origin;
    string destination;
    float price;

    static Item fromCSV(const string& csvLine);
    void display() const;
};

Item Item::fromCSV(const string& csvLine) {
    Item item;
    stringstream ss(csvLine);
    getline(ss, item.itemName, ',');
    ss >> item.weight;
    ss.ignore(); // Ignore the comma
    getline(ss, item.origin, ',');
    getline(ss, item.destination, ',');
    ss >> item.price;
    return item;
}

void Item::display() const {
    cout << "Nama Barang: " << itemName << endl;
    cout << "Berat (kg): " << weight << endl;
    cout << "Asal: " << origin << endl;
    cout << "Tujuan: " << destination << endl;
    cout << "Harga: " << price << endl;
}

int main() {
    ifstream inFile("to_send.csv");
    if (!inFile.is_open()) {
        cout << "Tidak bisa membuka file to_send.csv" << endl;
        return 1;
    }

    string line;
    vector<Item> items;
    queue<Item> southRouteQueue;
    queue<Item> northRouteQueue;
    queue<Item> westRouteQueue;
    vector<string> southRoute = {"sukabumi", "bandung", "tasikmalaya", "purwokerto", "yogyakarta"};
    vector<string> northRoute = {"jabodetabek", "purwakarta", "indramayu", "cirebon", "tegal", "pekalongan", "semarang", "solo", "ngawi", "madiun", "surabaya", "madura", "malang", "banyuwangi", "bali"};
    vector<string> westRoute = {"serang"};

    while (getline(inFile, line)) {
        Item item = Item::fromCSV(line);
        items.push_back(item);

        auto southIter = find(southRoute.begin(), southRoute.end(), item.destination);
        auto northIter = find(northRoute.begin(), northRoute.end(), item.destination);
        auto westIter = find(westRoute.begin(), westRoute.end(), item.destination);

        if (southIter != southRoute.end()) {
            cout << "Item " << item.itemName << " ditambahkan ke antrian Rute Selatan." << endl;
            southRouteQueue.push(item);
        } else if (northIter != northRoute.end()) {
            cout << "Item " << item.itemName << " ditambahkan ke antrian Rute Utara." << endl;
            northRouteQueue.push(item);
        } else if (westIter != westRoute.end()) {
            cout << "Item " << item.itemName << " ditambahkan ke antrian Rute Barat." << endl;
            westRouteQueue.push(item);
        } else {
            cout << "Destinasi " << item.destination << " tidak dikenali." << endl;
        }
    }
    inFile.close();

    cout << "\nDaftar barang dari file CSV:\n";
    for (const Item& item : items) {
        item.display();
        cout << "--------------------" << endl;
    }

    auto writeRouteToFile = [](const string& filename, queue<Item>& routeQueue) {
        ofstream outFile(filename);
        if (outFile.is_open()) {
            while (!routeQueue.empty()) {
                Item item = routeQueue.front();
                routeQueue.pop();
                outFile << item.itemName << "," << item.weight << "," << item.origin << "," << item.destination << "," << item.price << endl;
            }
            outFile.close();
            cout << "Data rute ditulis ke file " << filename << endl;
        } else {
            cout << "Tidak bisa membuka file " << filename << endl;
        }
    };

    writeRouteToFile("data/south.csv", southRouteQueue);
    writeRouteToFile("data/north.csv", northRouteQueue);
    writeRouteToFile("data/west.csv", westRouteQueue);

    return 0;
}
