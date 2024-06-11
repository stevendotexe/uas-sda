#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
#include <cstring>
#include <conio.h>

using namespace std;

// Konten dari item_grouping.cpp
class Item {
public:
    string namaItem;
    string asal;
    string tujuan;
    string provinsi;
    float berat;

    static Item dariCSV(const string& csvLine);
    void tampilkan() const;
};

Item Item::dariCSV(const string& csvLine) {
    Item item;
    stringstream ss(csvLine);
    getline(ss, item.namaItem, ',');
    getline(ss, item.asal, ',');
    getline(ss, item.tujuan, ',');
    getline(ss, item.provinsi, ',');
    string beratStr;
    getline(ss, beratStr, ',');
    item.berat = stof(beratStr);
    return item;
}

void Item::tampilkan() const {
    cout << "Nama Item\t: " << namaItem << endl;
    cout << "Asal\t\t: " << asal << endl;
    cout << "Tujuan\t\t: " << tujuan << endl;
    cout << "Provinsi\t: " << provinsi << endl;
    cout << "Berat (kg)\t: " << berat << endl;
}

void itemGrouping() {
    ifstream inFile("1124.csv");
    if (!inFile.is_open()) {
        cout << "Tidak dapat membuka file" << endl;
        return;
    }

    string line;
    vector<Item> items;
    queue<Item> antrianRuteSelatan;
    queue<Item> antrianRuteUtara;
    queue<Item> antrianRuteBarat;
    vector<string> ruteSelatan = {"Sukabumi", "Bandung", "Tasikmalaya", "Purwokerto", "Yogyakarta"};
    vector<string> ruteUtara = {"Jabodetabek", "Purwakarta", "Indramayu", "Cirebon", "Tegal", "Pekalongan", "Semarang", "Solo", "Ngawi", "Madiun", "Surabaya", "Madura", "Malang", "Banyuwangi", "Bali"};
    vector<string> ruteBarat = {"Serang"};

    while (getline(inFile, line)) {
        Item item = Item::dariCSV(line);
        items.push_back(item);

        if (find(ruteSelatan.begin(), ruteSelatan.end(), item.tujuan) != ruteSelatan.end()) {
            cout << "Item " << item.namaItem << " ditambahkan ke antrian Rute Selatan." << endl;
            antrianRuteSelatan.push(item);
        } else if (find(ruteUtara.begin(), ruteUtara.end(), item.tujuan) != ruteUtara.end()) {
            cout << "Item " << item.namaItem << " ditambahkan ke antrian Rute Utara." << endl;
            antrianRuteUtara.push(item);
        } else if (find(ruteBarat.begin(), ruteBarat.end(), item.tujuan) != ruteBarat.end()) {
            cout << "Item " << item.namaItem << " ditambahkan ke antrian Rute Barat." << endl;
            antrianRuteBarat.push(item);
        }
    }

    inFile.close();

    cout << "\nJumlah item di Antrian Rute Selatan: " << antrianRuteSelatan.size() << endl;
    cout << "Jumlah item di Antrian Rute Utara: " << antrianRuteUtara.size() << endl;
    cout << "Jumlah item di Antrian Rute Barat: " << antrianRuteBarat.size() << endl;
}

// Konten dari item_input.cpp
struct Akun {
    string nama;
    string alamat;
    string nomorTelepon;
    string email;
    string password;
};

// Fungsi untuk mendapatkan password yang diinput tanpa menampilkan di layar
string getHiddenPassword() {
    string password;
    char ch;
    while ((ch = _getch()) != '\r') { // '\r' adalah kode ASCII untuk Enter
        if (ch == '\b') { // '\b' adalah kode ASCII untuk Backspace
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

// Fungsi untuk membuat akun baru
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

// Fungsi untuk login ke akun
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

void menuAkun(Akun &akun) {
    int pilihan;

    do {
        cout << "\nMenu Akun Pengiriman Barang:" << endl;
        cout << "1. Daftar" << endl;
        cout << "2. Login" << endl;
        cout << "3. Keluar" << endl;
        cout << "Masukkan pilihan: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                buatAkunBaru(akun);
                break;
            case 2:
                if (login(akun)) {
                    return; // Keluar dari menu akun jika login berhasil
                }
                break;
            case 3:
                cout << "Terima kasih telah menggunakan aplikasi!" << endl;
                exit(0); // Keluar dari program
            default:
                cout << "Pilihan tidak valid!" << endl;
        }
    } while (true);
}

// Konten dari mst_shipping.cpp
const int MAX_NODES = 100;

class Graph {
public:
    int jumlahNode;
    vector<pair<int, int>> adj[MAX_NODES];

    Graph(int jumlahNode);
    void tambahEdge(int u, int v, int berat);
    void primMST();
};

Graph::Graph(int jumlahNode) {
    this->jumlahNode = jumlahNode;
}

void Graph::tambahEdge(int u, int v, int berat) {
    adj[u].push_back(make_pair(v, berat));
    adj[v].push_back(make_pair(u, berat));
}

void Graph::primMST() {
    vector<int> key(jumlahNode, INT_MAX);
    vector<bool> inMST(jumlahNode, false);
    vector<int> parent(jumlahNode, -1);

    key[0] = 0;

    for (int count = 0; count < jumlahNode - 1; count++) {
        int u = -1;

        for (int i = 0; i < jumlahNode; i++) {
            if (!inMST[i] && (u == -1 || key[i] < key[u])) {
                u = i;
            }
        }

        inMST[u] = true;

        for (auto &edge : adj[u]) {
            int v = edge.first;
            int berat = edge.second;

            if (!inMST[v] && berat < key[v]) {
                key[v] = berat;
                parent[v] = u;
            }
        }
    }

    cout << "Edge \tWeight\n";
    for (int i = 1; i < jumlahNode; i++) {
        cout << parent[i] << " - " << i << " \t" << key[i] << " \n";
    }
}

void jalankanMST() {
    int jumlahNode = 5;
    Graph g(jumlahNode);

    g.tambahEdge(0, 1, 2);
    g.tambahEdge(0, 3, 6);
    g.tambahEdge(1, 2, 3);
    g.tambahEdge(1, 3, 8);
    g.tambahEdge(1, 4, 5);
    g.tambahEdge(2, 4, 7);
    g.tambahEdge(3, 4, 9);

    g.primMST();
}

// Fungsi utama yang memanggil semua fungsi lainnya
int main() {
    Akun akun;
    cout << "Selamat datang di Aplikasi Pengiriman Barang!" << endl;
    menuAkun(akun); // Panggil menu akun terlebih dahulu untuk login

    int pilihan;

    do {
        cout << "\nMenu Utama:" << endl;
        cout << "1. Grupkan Item" << endl;
        cout << "2. Jalankan MST" << endl;
        cout << "3. Keluar" << endl;
        cout << "Masukkan pilihan: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                itemGrouping();
                break;
            case 2:
                jalankanMST();
                break;
            case 3:
                cout << "Terima kasih telah menggunakan aplikasi!" << endl;
                break;
            default:
                cout << "Pilihan tidak valid!" << endl;
        }
    } while (pilihan != 3);

    return 0;
}
