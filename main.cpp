#include <iostream>
#include <string>
using namespace std;
// farkhan abdilah - 2507411012
// UAS Proyek 1: Sistem Version Control Sederhana

// Linked List untuk menyimpan file di dalam folder.
struct File {
    int idFile;
    string namaFile;
    int ukuranFile;
    File* next;
};

// Stack untuk menyimpan riwayat aktivitas terakhir.
struct Riwayat {
    string aksi;
    int idFile;
    string namaFileLama;
    string namaFileBaru;
    int ukuranFile;
    string namaFolder;
    Riwayat* next;
};

// Tree folder memakai konsep anak dan saudara.
struct Folder {
    string namaFolder;
    File* daftarFile;
    Folder* anak;
    Folder* saudara;
};

Folder* akar = NULL;
Riwayat* puncakRiwayat = NULL;
int idOtomatis = 1;

bool karakterSpasi(char huruf) {
    return huruf == ' ' || huruf == '\t' || huruf == '\n' || huruf == '\r';
}

string rapikanTeks(string teks) {
    if (teks.length() == 0) {
        return "";
    }

    int awal = 0;
    int akhir = (int)teks.length() - 1;

    while (awal < (int)teks.length() && karakterSpasi(teks[awal])) {
        awal++;
    }

    while (akhir >= awal && karakterSpasi(teks[akhir])) {
        akhir--;
    }

    if (awal > akhir) {
        return "";
    }

    return teks.substr(awal, akhir - awal + 1);
}

bool bacaTeks(string pesan, string& isi) {
    cout << pesan;
    if (!getline(cin, isi)) {
        return false;
    }

    isi = rapikanTeks(isi);

    if (isi == "") {
        cout << "Input tidak boleh kosong." << endl;
        return false;
    }

    return true;
}

bool ubahKeAngka(string teks, int& angka) {
    teks = rapikanTeks(teks);

    if (teks == "") {
        return false;
    }

    long long hasil = 0;

    for (int i = 0; i < (int)teks.length(); i++) {
        if (teks[i] < '0' || teks[i] > '9') {
            return false;
        }

        hasil = hasil * 10 + (teks[i] - '0');
        if (hasil > 2147483647) {
            return false;
        }
    }

    angka = (int)hasil;
    return true;
}

bool bacaAngka(string pesan, int& angka) {
    string teks;

    if (!bacaTeks(pesan, teks)) {
        return false;
    }

    if (!ubahKeAngka(teks, angka)) {
        cout << "Input harus berupa angka." << endl;
        return false;
    }

    return true;
}

// Stack Riwayat
void pushRiwayat(string aksi, int idFile, string namaLama, string namaBaru,
                 int ukuranFile, string namaFolder) {
    Riwayat* baru = new Riwayat;
    baru->aksi = aksi;
    baru->idFile = idFile;
    baru->namaFileLama = namaLama;
    baru->namaFileBaru = namaBaru;
    baru->ukuranFile = ukuranFile;
    baru->namaFolder = namaFolder;
    baru->next = puncakRiwayat;
    puncakRiwayat = baru;
}

bool riwayatKosong() {
    return puncakRiwayat == NULL;
}

Riwayat popRiwayat() {
    Riwayat data = *puncakRiwayat;
    Riwayat* hapus = puncakRiwayat;
    puncakRiwayat = puncakRiwayat->next;
    delete hapus;
    return data;
}

// Linked List File
void tambahFileKeList(File*& kepala, int idFile, string namaFile, int ukuranFile) {
    File* baru = new File;
    baru->idFile = idFile;
    baru->namaFile = namaFile;
    baru->ukuranFile = ukuranFile;
    baru->next = NULL;

    if (kepala == NULL) {
        kepala = baru;
    } else {
        File* bantu = kepala;
        while (bantu->next != NULL) {
            bantu = bantu->next;
        }
        bantu->next = baru;
    }
}

File* cariFileBerdasarkanNama(File* kepala, string namaFile) {
    File* bantu = kepala;

    while (bantu != NULL) {
        if (bantu->namaFile == namaFile) {
            return bantu;
        }
        bantu = bantu->next;
    }

    return NULL;
}

File* cariFileBerdasarkanId(File* kepala, int idFile) {
    File* bantu = kepala;

    while (bantu != NULL) {
        if (bantu->idFile == idFile) {
            return bantu;
        }
        bantu = bantu->next;
    }

    return NULL;
}

bool hapusFileDariList(File*& kepala, string namaFile, int& idFile, int& ukuranFile) {
    File* bantu = kepala;
    File* sebelum = NULL;

    while (bantu != NULL) {
        if (bantu->namaFile == namaFile) {
            idFile = bantu->idFile;
            ukuranFile = bantu->ukuranFile;

            if (sebelum == NULL) {
                kepala = bantu->next;
            } else {
                sebelum->next = bantu->next;
            }

            delete bantu;
            return true;
        }

        sebelum = bantu;
        bantu = bantu->next;
    }

    return false;
}

bool hapusFileBerdasarkanId(File*& kepala, int idFile) {
    File* bantu = kepala;
    File* sebelum = NULL;

    while (bantu != NULL) {
        if (bantu->idFile == idFile) {
            if (sebelum == NULL) {
                kepala = bantu->next;
            } else {
                sebelum->next = bantu->next;
            }

            delete bantu;
            return true;
        }

        sebelum = bantu;
        bantu = bantu->next;
    }

    return false;
}

// Tree folder 
Folder* buatFolder(string namaFolder) {
    Folder* baru = new Folder;
    baru->namaFolder = namaFolder;
    baru->daftarFile = NULL;
    baru->anak = NULL;
    baru->saudara = NULL;
    return baru;
}

void tambahFolder(Folder* induk, Folder* anakBaru) {
    if (induk->anak == NULL) {
        induk->anak = anakBaru;
    } else {
        Folder* bantu = induk->anak;
        while (bantu->saudara != NULL) {
            bantu = bantu->saudara;
        }
        bantu->saudara = anakBaru;
    }
}

Folder* cariFolder(Folder* node, string namaFolder) {
    if (node == NULL) {
        return NULL;
    }

    if (node->namaFolder == namaFolder) {
        return node;
    }

    Folder* hasil = cariFolder(node->anak, namaFolder);
    if (hasil != NULL) {
        return hasil;
    }

    return cariFolder(node->saudara, namaFolder);
}

void tampilStrukturFolder(Folder* node, int level) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < level; i++) {
        cout << "  ";
    }
    cout << "[Folder] " << node->namaFolder << endl;

    File* file = node->daftarFile;
    while (file != NULL) {
        for (int i = 0; i < level + 1; i++) {
            cout << "  ";
        }
        cout << "- " << file->namaFile
             << " (ID: " << file->idFile
             << ", ukuran: " << file->ukuranFile << " KB)" << endl;
        file = file->next;
    }

    tampilStrukturFolder(node->anak, level + 1);
    tampilStrukturFolder(node->saudara, level);
}

void cariFileDiTree(Folder* node, string namaFile, bool& ketemu) {
    if (node == NULL) {
        return;
    }

    File* file = cariFileBerdasarkanNama(node->daftarFile, namaFile);
    if (file != NULL) {
        ketemu = true;
        cout << "File ditemukan di folder " << node->namaFolder
             << " | ID: " << file->idFile
             << " | Ukuran: " << file->ukuranFile << " KB" << endl;
    }

    cariFileDiTree(node->anak, namaFile, ketemu);
    cariFileDiTree(node->saudara, namaFile, ketemu);
}

// Fitur Program
void fiturTambahFile() {
    string namaFolder;
    string namaFile;
    int ukuranFile;

    if (!bacaTeks("Nama folder (UAS/Program/Dokumen/Revisi/Percobaan): ", namaFolder)) {
        return;
    }
    if (!bacaTeks("Nama file: ", namaFile)) {
        return;
    }
    if (!bacaAngka("Ukuran file (KB): ", ukuranFile)) {
        return;
    }

    if (ukuranFile <= 0) {
        cout << "Ukuran file harus lebih dari 0 KB." << endl;
        return;
    }

    Folder* folder = cariFolder(akar, namaFolder);
    if (folder == NULL) {
        cout << "Folder tidak ditemukan." << endl;
        return;
    }

    if (cariFileBerdasarkanNama(folder->daftarFile, namaFile) != NULL) {
        cout << "File dengan nama itu sudah ada di folder tersebut." << endl;
        return;
    }

    tambahFileKeList(folder->daftarFile, idOtomatis, namaFile, ukuranFile);
    pushRiwayat("TAMBAH", idOtomatis, namaFile, "", ukuranFile, namaFolder);

    cout << "File berhasil ditambah dengan ID " << idOtomatis << "." << endl;
    idOtomatis++;
}

void fiturHapusFile() {
    string namaFolder;
    string namaFile;
    int idFile;
    int ukuranFile;

    if (!bacaTeks("Nama folder (UAS/Program/Dokumen/Revisi/Percobaan): ", namaFolder)) {
        return;
    }
    if (!bacaTeks("Nama file yang dihapus: ", namaFile)) {
        return;
    }

    Folder* folder = cariFolder(akar, namaFolder);
    if (folder == NULL) {
        cout << "Folder tidak ditemukan." << endl;
        return;
    }

    if (hapusFileDariList(folder->daftarFile, namaFile, idFile, ukuranFile)) {
        pushRiwayat("HAPUS", idFile, namaFile, "", ukuranFile, namaFolder);
        cout << "File berhasil dihapus." << endl;
    } else {
        cout << "File tidak ditemukan." << endl;
    }
}

void fiturRenameFile() {
    string namaFolder;
    string namaLama;
    string namaBaru;

    if (!bacaTeks("Nama folder (UAS/Program/Dokumen/Revisi/Percobaan): ", namaFolder)) {
        return;
    }
    if (!bacaTeks("Nama file lama: ", namaLama)) {
        return;
    }
    if (!bacaTeks("Nama file baru: ", namaBaru)) {
        return;
    }

    Folder* folder = cariFolder(akar, namaFolder);
    if (folder == NULL) {
        cout << "Folder tidak ditemukan." << endl;
        return;
    }

    File* file = cariFileBerdasarkanNama(folder->daftarFile, namaLama);
    if (file == NULL) {
        cout << "File tidak ditemukan." << endl;
        return;
    }

    if (cariFileBerdasarkanNama(folder->daftarFile, namaBaru) != NULL) {
        cout << "Nama file baru sudah digunakan di folder tersebut." << endl;
        return;
    }

    pushRiwayat("RENAME", file->idFile, namaLama, namaBaru, file->ukuranFile, namaFolder);
    file->namaFile = namaBaru;

    cout << "File berhasil direname." << endl;
}

void fiturUndo() {
    if (riwayatKosong()) {
        cout << "Belum ada aktivitas yang bisa di-undo." << endl;
        return;
    }

    Riwayat data = popRiwayat();
    Folder* folder = cariFolder(akar, data.namaFolder);

    if (folder == NULL) {
        cout << "Undo gagal karena folder tidak ditemukan." << endl;
        return;
    }

    if (data.aksi == "TAMBAH") {
        if (hapusFileBerdasarkanId(folder->daftarFile, data.idFile)) {
            cout << "Undo tambah file berhasil." << endl;
        } else {
            cout << "Undo tambah file gagal." << endl;
        }
    } else if (data.aksi == "HAPUS") {
        if (cariFileBerdasarkanId(folder->daftarFile, data.idFile) != NULL ||
            cariFileBerdasarkanNama(folder->daftarFile, data.namaFileLama) != NULL) {
            cout << "Undo hapus file gagal karena file sudah ada." << endl;
        } else {
            tambahFileKeList(folder->daftarFile, data.idFile, data.namaFileLama, data.ukuranFile);
            cout << "Undo hapus file berhasil." << endl;
        }
    } else if (data.aksi == "RENAME") {
        File* file = cariFileBerdasarkanId(folder->daftarFile, data.idFile);

        if (file == NULL) {
            cout << "Undo rename file gagal karena file tidak ditemukan." << endl;
        } else if (cariFileBerdasarkanNama(folder->daftarFile, data.namaFileLama) != NULL) {
            cout << "Undo rename file gagal karena nama lama sudah dipakai." << endl;
        } else {
            file->namaFile = data.namaFileLama;
            cout << "Undo rename file berhasil." << endl;
        }
    }
}

void fiturCariFile() {
    string namaFile;
    bool ketemu = false;

    if (!bacaTeks("Nama file yang dicari: ", namaFile)) {
        return;
    }

    cariFileDiTree(akar, namaFile, ketemu);

    if (!ketemu) {
        cout << "File tidak ditemukan." << endl;
    }
}

void buatDataFolderAwal() {
    akar = buatFolder("UAS");

    Folder* program = buatFolder("Program");
    Folder* dokumen = buatFolder("Dokumen");
    Folder* revisi = buatFolder("Revisi");
    Folder* percobaan = buatFolder("Percobaan");

    tambahFolder(akar, program);
    tambahFolder(akar, dokumen);
    tambahFolder(akar, revisi);
    tambahFolder(program, percobaan);
}

void tampilMenu() {
    cout << "\n[=== SISTEM VERSION CONTROL SEDERHANA ===]" << endl;
    cout << "Folder yang tersedia: UAS, Program, Dokumen, Revisi, Percobaan" << endl;
    cout << "1. Tambah file" << endl;
    cout << "2. Hapus file" << endl;
    cout << "3. Rename file" << endl;
    cout << "4. Undo aktivitas terakhir" << endl;
    cout << "5. Tampilkan struktur folder" << endl;
    cout << "6. Cari file berdasarkan nama" << endl;
    cout << "0. Keluar" << endl;
    cout << "[========================================]" << endl;
}

int main() {
    int pilihan = -1;
    buatDataFolderAwal();

    do {
        tampilMenu();

        if (!bacaAngka("Pilihan: ", pilihan)) {
            if (cin.eof()) {
                cout << "\nProgram selesai." << endl;
                break;
            }
            continue;
        }

        if (pilihan == 1) {
            fiturTambahFile();
        } else if (pilihan == 2) {
            fiturHapusFile();
        } else if (pilihan == 3) {
            fiturRenameFile();
        } else if (pilihan == 4) {
            fiturUndo();
        } else if (pilihan == 5) {
            tampilStrukturFolder(akar, 0);
        } else if (pilihan == 6) {
            fiturCariFile();
        } else if (pilihan == 0) {
            cout << "Program selesai." << endl;
        } else {
            cout << "Pilihan tidak valid." << endl;
        }
    } while (pilihan != 0);

    return 0;
}
