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
    string weightStr;
    getline(ss, weightStr, ',');
    item.weight = stof(weightStr);
    getline(ss, item.origin, ',');
    getline(ss, item.destination, ',');
    string priceStr;
    getline(ss, priceStr, ',');
    item.price = stof(priceStr);  
    return item;
}

void Item::display() const {
    cout << "Nama Barang\t: " << itemName << endl;
    cout << "Berat (kg)\t: " << weight << endl;
    cout << "Asal\t\t: " << origin << endl;
    cout << "Tujuan\t\t: " << destination << endl;
    cout << "Harga\t\t: " << price << endl;  
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

        if (find(southRoute.begin(), southRoute.end(), item.destination) != southRoute.end()) {
            cout << "Item " << item.itemName << " ditambahkan ke antrian Rute Selatan." << endl;
            southRouteQueue.push(item);
        } else if (find(northRoute.begin(), northRoute.end(), item.destination) != northRoute.end()) {
            cout << "Item " << item.itemName << " ditambahkan ke antrian Rute Utara." << endl;
            northRouteQueue.push(item);
        } else if (find(westRoute.begin(), westRoute.end(), item.destination) != westRoute.end()) {
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

    return 0;
}