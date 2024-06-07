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
    void insertQueue(Item itemToDeliver);
    string itemCSVLine(Item itemDetails);
    void displayQueue();
};

vector<array<string, 2>> loadDestinations();
void insertDataIntoCSV();

// Function definitions
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

int Item::getPrice() const {
    // TODO: COUNT USING MST AND GET ITEM PRICE
    return 0; // Placeholder
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

void ItemQueue::insertQueue(Item itemToDeliver) {
    if (!isFull()) {
        itemQueue[top++] = itemToDeliver;
        cout << "Item " << itemToDeliver.getItemName() << " inserted into queue." << endl;
    } else {
        // write into csv
        cout << "Queue is full. Item not inserted." << endl;
    }
}

string ItemQueue::itemCSVLine(Item itemDetails) {
    if (isEmpty()) {
        return "";
    } else {
        Item itemToSend = itemQueue[0];
        for (int i = 1; i < top; i++) {
            itemQueue[i - 1] = itemQueue[i];
        }
        top--;
        return itemToSend.getItemName() + "," +
            to_string(itemToSend.getWeight()) + "," +
            itemToSend.getOrigin() + "," +
            itemToSend.getDestination() + "," +
            to_string(itemToSend.getPrice());
    }
}

void ItemQueue::displayQueue() {
    if (!isEmpty()) {
        cout << "Displaying current items in queue (not written in CSV) (" << top << " items)." << endl;
        cout << "Item Name\t\t" << "Weight\t" << "Destination\t" << endl;
        for (int i = 0; i < top; i++) {
            cout << left << setw(20) << itemQueue[i].getItemName() << "\t" << itemQueue[i].getWeight() << "\t" << itemQueue[i].getDestination() << endl;
        }
    }
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

void insertDataIntoCSV() {
    // TODO
}

int main() {
    vector<array<string, 2>> destinations = loadDestinations();
    ItemQueue queue;
    queue.createQueue();

    while (true) {
        int selection;
        cout << "1. Add item\n";
        cout << "2. Display queue\n";
        cout << "3. Exit\n";
        cout << "Selection: "; cin >> selection;
        switch(selection){
            case 1:
            {
                string confirmation;
                Item item;

                item.input(); // input barang ke dalam objek
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
                    cout << "Berat (gr)\t: " << weight << endl;
                    cout << "Tambah barang? (Y/N): ";
                    getline(cin, confirmation);
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
            {
                queue.displayQueue();
                break;
            }
            case 3:
            {
                // TODO: SAVE TO CSV
                exit(0);
            }
            default:
                cout << "Input salah!";
                break;
        }
    }
    
    
    return 0;
}
