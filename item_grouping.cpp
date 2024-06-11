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
    float price;  // Mengubah province menjadi price

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
    item.price = stof(priceStr);  // Mengambil nilai price dari CSV
    return item;
}

void Item::display() const {
    cout << "Item Name\t: " << itemName << endl;
    cout << "Weight (kg)\t: " << weight << endl;
    cout << "Origin\t\t: " << origin << endl;
    cout << "Destination\t: " << destination << endl;
    cout << "Price\t\t: " << price << endl;  // Menampilkan harga
}

int main() {
    ifstream inFile("to_send.csv");
    if (!inFile.is_open()) {
        cout << "Unable to open file" << endl;
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
            cout << "Item " << item.itemName << " ditambahkan ke antrian South Route." << endl;
            southRouteQueue.push(item);
        } else if (find(northRoute.begin(), northRoute.end(), item.destination) != northRoute.end()) {
            cout << "Item " << item.itemName << " ditambahkan ke antrian North Route." << endl;
            northRouteQueue.push(item);
        } else if (find(westRoute.begin(), westRoute.end(), item.destination) != westRoute.end()) {
            cout << "Item " << item.itemName << " ditambahkan ke antrian West Route." << endl;
            westRouteQueue.push(item);
        } else {
            cout << "Destinasi " << item.destination << " tidak dikenali." << endl;
        }
    }
    inFile.close();

    // Menampilkan semua barang yang telah dimuat dari CSV
    cout << "\nItems loaded from CSV:\n";
    for (const Item& item : items) {
        item.display();
        cout << "--------------------" << endl;
    }

    return 0;
}