#include <iostream>
#include <array>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class Item{
    string itemName;
    string origin;
    string destination;
    string province;
    float weight;
};

vector<array<string, 2>> loadDestinations() { // returns a 2D vector of all the destinations
    vector<array<string, 2>> destinations;

    ifstream cityFiles("data/destinations.csv");
    if (!cityFiles.is_open()) {
        cout << "Destination files failed to load." << endl;
    } else {
        string line;
        while (getline(cityFiles, line)) {
            stringstream ss(line);
            string province, city;
            if (getline(ss, province, ',') && getline(ss, city, ',')) { // Use comma as delimiter
                destinations.push_back({province, city});
            }
        }
    }
    cityFiles.close();
    return destinations;
}

int main(){
    vector<array<string, 2>> destinations = loadDestinations();

    
}