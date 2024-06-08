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
    string origin;
    string destination;
    string province;
    float weight;

    static Item fromCSV(const string& csvLine);
    void display() const;
};

Item Item::fromCSV(const string& csvLine) {
    Item item;
    stringstream ss(csvLine);
    getline(ss, item.itemName, ',');
    getline(ss, item.origin, ',');
    getline(ss, item.destination, ',');
    getline(ss, item.province, ',');
    string weightStr;
    getline(ss, weightStr, ',');
    item.weight = stof(weightStr);
    return item;
}

void Item::display() const {
    cout << "Item Name\t: " << itemName << endl;
    cout << "Origin\t\t: " << origin << endl;
    cout << "Destination\t: " << destination << endl;
    cout << "Province\t: " << province << endl;
    cout << "Weight (kg)\t: " << weight << endl;
}

int main() {
    ifstream inFile("item_data.csv");
    if (!inFile.is_open()) {
        cout << "Unable to open file" << endl;
        return 1;
    }

    string line;
    vector<Item> items;
    queue<Item> southRouteQueue;
    queue<Item> northRouteQueue;
    queue<Item> westRouteQueue;
    vector<string> southRoute = {"Sukabumi", "Bandung", "Tasikmalaya", "Purwokerto", "Yogyakarta"};
    vector<string> northRoute = {"Jabodetabek", "Purwakarta", "Indramayu", "Cirebon", "Tegal", "Pekalongan", "Semarang", "Solo", "Ngawi", "Madiun", "Surabaya", "Madura", "Malang", "Banyuwangi", "Bali"};
    vector<string> westRoute = {"Serang"};

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