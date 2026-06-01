#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <stdexcept>
#include <sstream>

using namespace std;

// ==========================================
// 1. STRUKTUR KELAS TOKO ELEKTRONIK
// ==========================================
class TokoElektronik {
private:
    // Enkapsulasi: Array berkapasitas 3 elemen untuk barang di etalase
    array<string, 3> etalase;

public:
    // Constructor: Mengisi data awal ke dalam etalase secara otomatis
    TokoElektronik() {
        etalase[0] = "Smartphone Samsung Galaxy";
        etalase[1] = "Laptop Asus ROG";
        etalase[2] = "Smart TV LG 43 Inch";
    }

    // Method Public dengan Exception Handling kustom
    string ambilProduk(size_t nomorRak) {
        try {
            // Wajib menggunakan .at() untuk memicu out_of_range jika indeks tidak valid
            return etalase.at(nomorRak);
        }
        catch (const out_of_range& e) {
            // 2. MEKANISME EXCEPTION HANDLING (THROW KUSTOM)
            string pesanError = "Gagal Mengambil Barang : Rak nomor " + tostring(nomorRak) + " kosong atau tidak tersedia!";
            throw runtime_error(pesanError);
        }
    }
};

// ==========================================
// 3. FITUR MANAJEMEN GUDANG (FILE I/O & CRUD)
// ==========================================
const string FILE_NAME = "gudang.txt";

// Fungsi Helper untuk menampilkan semua barang (Read) secara otomatis
void readGudang() {
    ifstream file(FILE_NAME);
    string baris;
    cout << "\n=== DAFTAR BARANG DI GUDANG (FILE) ===\n";
    
    if (!file.is_open()) {
        cout << "[Info] File gudang belum dibuat atau masih kosong.\n";
        return;
    }

    int nomor = 1;
    bool adaData = false;
    while (getline(file, baris)) {
        if (!baris.empty()) {
            cout << nomor << ". " << baris << endl;
            nomor++;
            adaData = true;
        }
    }
    
    if (!adaData) {
        cout << "(Gudang Kosong)\n";
    }
    file.close();
}

// Fitur Create: Menambah barang baru
void createBarang() {
    ofstream file(FILE_NAME, ios::app); // ios::app untuk append/menambah di akhir
    if (!file.is_open()) {
        cout << "Gagal membuka file gudang!\n";
        return;
    }

    string namaBarang;
    cout << "Masukkan nama barang baru: ";
    cin.ignore();
    getline(cin, namaBarang);

    if (!namaBarang.empty()) {
        file << namaBarang << "\n";
        cout << "Sukses: Barang \"" << namaBarang << "\" berhasil ditambahkan ke gudang.\n";
    }
    file.close();
}

// Fitur Update: Mengubah data barang tertentu
void updateBarang() {
    readGudang();
    ifstream fileIn(FILE_NAME);
    if (!fileIn.is_open()) return;

    string baris;
    array<string, 100> listBarang; // Buffer sementara untuk modifikasi data
    int totalBarang = 0;

    while (getline(fileIn, baris)) {
        if (!baris.empty()) {
            listBarang[totalBarang] = baris;
            totalBarang++;
        }
    }
    fileIn.close();

    if (totalBarang == 0) return;

    int pilihan;
    cout << "Pilih nomor barang yang ingin diubah: ";
    cin >> pilihan;

    if (pilihan > 0 && pilihan <= totalBarang) {
        string namaBaru;
        cout << "Masukkan nama baru untuk \"" << listBarang[pilihan - 1] << "\": ";
        cin.ignore();
        getline(cin, namaBaru);

        listBarang[pilihan - 1] = namaBaru;

        // Tulis ulang file dengan data yang diperbarui
        ofstream fileOut(FILE_NAME);
        for (int i = 0; i < totalBarang; i++) {
            fileOut << listBarang[i] << "\n";
        }
        fileOut.close();
        cout << "Sukses: Data barang berhasil diperbarui!\n";
    } else {
        cout << "Pilihan tidak valid!\n";
    }
}

// Fitur Delete: Menghapus data barang
void deleteBarang() {
    readGudang();
    ifstream fileIn(FILE_NAME);
    if (!fileIn.is_open()) return;

    string baris;
    array<string, 100> listBarang;
    int totalBarang = 0;

    while (getline(fileIn, baris)) {
        if (!baris.empty()) {
            listBarang[totalBarang] = baris;
            totalBarang++;
        }
    }
    fileIn.close();

    if (totalBarang == 0) return;

    int pilihan;
    cout << "Pilih nomor barang yang ingin dihapus: ";
    cin >> pilihan;

    if (pilihan > 0 && pilihan <= totalBarang) {
        cout << "Sukses: Barang \"" << listBarang[pilihan - 1] << "\" berhasil dihapus.\n";

        // Tulis ulang file tanpa menyertakan barang yang dihapus
        ofstream fileOut(FILE_NAME);
        for (int i = 0; i < totalBarang; i++) {
            if (i != (pilihan - 1)) {
                fileOut << listBarang[i] << "\n";
            }
        }
        fileOut.close();
    } else {
        cout << "Pilihan tidak valid!\n";
    }
}

// ==========================================
// 4. SIMULASI ETALASE (PROFILING EXCEPTION)
// ==========================================
void jalankanSimulasiEtalase(TokoElektronik& toko) {
    cout << "\n=== JALANKAN SIMULASI EXCEPTION HANDLING ===\n";
    
    // Skenario 1: Pengambilan barang di rak indeks ke-1 (Harus Sukses)
    cout << "[Skenario 1] Mengambil barang di rak indeks ke-1...\n";
    try {
        string barang = toko.ambilProduk(1);
        cout << "Hasil Skenario 1: Berhasil mengambil -> " << barang << "\n\n";
    } 
    catch (const runtime_error& e) {
        cout << "Hasil Skenario 1 (Gagal Unexpected): " << e.what() << "\n\n";
    }

    // Skenario 2: Pengambilan barang di rak indeks ke-5 (Harus Gagal & Lempar Pesan Kustom)
    cout << "[Skenario 2] Mengambil barang di rak indeks ke-5...\n";
    try {
        string barang = toko.ambilProduk(5);
        cout << "Hasil Skenario 2 (Sukses Unexpected): " << barang << "\n\n";
    } 
    catch (const runtime_error& e) {
        // Menangkap pesan error kustom yang di-throw dari method
        cout << "Hasil Skenario 2 (Diharapkan): " << e.what() << "\n\n";
    }
}

// ==========================================
// MAIN FUNCTION
// ==========================================
int main() {
    TokoElektronik tokoGibran;
    int pilihanMenu;

    do {
        // Sesuai ketentuan: Tampilkan data secara otomatis saat menu dibuka
        readGudang();

        cout << "\n=========================================\n";
        cout << "       SISTEM MANAJEMEN GIBRAN JAYA      \n";
        cout << "=========================================\n";
        cout << "1. Tambah Barang Gudang (Create)\n";
        cout << "2. Ubah Barang Gudang (Update)\n";
        cout << "3. Hapus Barang Gudang (Delete)\n";
        cout << "4. Jalankan Simulasi Etalase (Exception Testing)\n";
        cout << "5. Keluar Aplikasi\n";
        cout << "Pilih menu (1-5): ";
        cin >> pilihanMenu;

        switch (pilihanMenu) {
            case 1:
                createBarang();
                break;
            case 2:
                updateBarang();
                break;
            case 3:
                deleteBarang();
                break;
            case 4:
                jalankanSimulasiEtalase(tokoGibran);
                break;
            case 5:
                cout << "Terima kasih telah menggunakan sistem Gibran Jaya.\n";
                break;
            default:
                cout << "Pilihan tidak valid! Silakan masukkan angka 1-5.\n";
                break;
        }
        
        cout << "\n";
        system("pause"); // Untuk menahan layar pada OS Windows sebelum loop kembali
        system("clear || cls"); // Membersihkan layar agar CLI tetap rapi

    } while (pilihanMenu != 5);

    return 0;
}