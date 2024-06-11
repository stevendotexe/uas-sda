#include <iostream>
#include <string>
#include <conio.h>

using namespace std;

struct Akun {
    string nama;
    string alamat;
    string nomorTelepon;
    string email;
    string password;
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
void login(Akun &akun) {
    string email, passwordMasukan;

    cin.ignore(); // Membersihkan buffer
    cout << "Masukkan email: ";
    getline(cin, email);
    cout << "Masukkan password: ";
    passwordMasukan = getHiddenPassword();

    if (email == akun.email && passwordMasukan == akun.password) {
        cout << "Login berhasil!" << endl;
    } else {
        cout << "Email atau password salah!" << endl;
    }
}

// Fungsi utama
int main() {
    Akun akun;
    int pilihan;

    do {
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
                login(akun);
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

