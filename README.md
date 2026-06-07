# Sistem Version Control Sederhana

Program ini dibuat untuk tugas UAS struktur data. Idenya seperti platform version control sederhana untuk menyimpan daftar file, riwayat perubahan terakhir, dan struktur folder.

Kode utama ada di `main.cpp`.

## Cara Menjalankan Program

Compile:

```bash
g++ main.cpp -o main
```

Jalankan:

```bash
./main
```

## Library Yang Dipakai

Program ini hanya memakai 2 library:

```cpp
#include <iostream>
#include <string>
```

Penjelasannya:

- `iostream` dipakai untuk input dan output, contohnya `cin`, `cout`, dan `endl`.
- `string` dipakai supaya program bisa menyimpan teks, contohnya nama file, nama folder, dan nama aksi.

## Gambaran Program

Program ini punya 6 fitur utama:

1. Tambah file
2. Hapus file
3. Rename file
4. Undo aktivitas terakhir
5. Tampilkan struktur folder
6. Cari file berdasarkan nama

Folder yang sudah dibuat dari awal:

```text
UAS
  Program
    Percobaan
  Dokumen
  Revisi
```

Jadi kalau program meminta nama folder, isi dengan salah satu ini:

```text
UAS
Program
Dokumen
Revisi
Percobaan
```

Huruf besar dan kecil harus sama. Misalnya `Program` benar, tapi `program` dianggap berbeda.

## ADT Yang Dipakai

### 1. Linked List

Linked List dipakai untuk menyimpan daftar file di dalam folder.

Struct yang dipakai:

```cpp
struct File {
    int idFile;
    string namaFile;
    int ukuranFile;
    File* next;
};
```

Artinya:

- `idFile` untuk nomor unik file.
- `namaFile` untuk nama file.
- `ukuranFile` untuk ukuran file dalam KB.
- `next` untuk menunjuk ke file berikutnya.

Contoh bentuk linked list:

```text
Cpp -> Laporan -> Catatan -> NULL
```

Setiap folder punya linked list file sendiri.

### 2. Stack

Stack dipakai untuk fitur undo. Stack memakai konsep LIFO, yaitu data yang terakhir masuk akan keluar duluan.

Struct yang dipakai:

```cpp
struct Riwayat {
    string aksi;
    int idFile;
    string namaFileLama;
    string namaFileBaru;
    int ukuranFile;
    string namaFolder;
    Riwayat* prev;
};
```

Artinya:

- `aksi` menyimpan jenis aktivitas, contohnya `TAMBAH`, `HAPUS`, atau `RENAME`.
- `idFile` menyimpan id file yang berubah.
- `namaFileLama` menyimpan nama file sebelum diubah.
- `namaFileBaru` menyimpan nama file setelah rename.
- `ukuranFile` menyimpan ukuran file.
- `namaFolder` menyimpan folder tempat file berada.
- `prev` menunjuk ke riwayat sebelumnya.

Contoh stack:

```text
RENAME
HAPUS
TAMBAH
NULL
```

Kalau user memilih undo, yang diambil adalah aktivitas paling atas.

### 3. Tree

Tree dipakai untuk menyimpan struktur folder.

Struct yang dipakai:

```cpp
struct Folder {
    string namaFolder;
    File* daftarFile;
    Folder* anak;
    Folder* saudara;
};
```

Artinya:

- `namaFolder` untuk nama folder.
- `daftarFile` menunjuk ke linked list file di folder itu.
- `anak` menunjuk ke folder di dalam folder tersebut.
- `saudara` menunjuk ke folder yang posisinya sejajar.

Program ini memakai model anak dan saudara supaya tree bisa dibuat pakai pointer sederhana.

Contohnya:

```text
UAS
  Program
    Percobaan
  Dokumen
  Revisi
```

`Program`, `Dokumen`, dan `Revisi` adalah anak dari `UAS`.

`Percobaan` adalah anak dari `Program`.

## Tipe Data Yang Dipakai

Tipe data dasar:

- `int` untuk angka, seperti `idFile`, `ukuranFile`, dan `pilihan`.
- `string` untuk teks, seperti `namaFile`, `namaFolder`, dan `aksi`.
- `bool` untuk nilai benar atau salah, seperti `ketemu`.
- `char` untuk satu karakter, dipakai pada fungsi pengecekan spasi.
- `long long` untuk menampung angka sementara saat validasi input angka.

Tipe data pointer:

- `File*` untuk menunjuk node file.
- `Riwayat*` untuk menunjuk node stack riwayat.
- `Folder*` untuk menunjuk node folder.

Tipe data buatan sendiri:

- `struct File`
- `struct Riwayat`
- `struct Folder`

## Variabel Global

```cpp
Folder* akar = NULL;
Riwayat* puncakRiwayat = NULL;
int idOtomatis = 1;
```

Penjelasan:

- `akar` adalah folder paling atas dalam tree. Di program ini nilainya adalah folder `UAS`.
- `puncakRiwayat` adalah bagian paling atas dari stack undo.
- `idOtomatis` dipakai untuk memberi ID file secara otomatis setiap tambah file.

Kenapa dibuat global?

Karena data ini dipakai oleh banyak fungsi. Jadi lebih mudah dipanggil dari mana saja dalam program.

## Penjelasan Function

### `karakterSpasi`

```cpp
bool karakterSpasi(char huruf)
```

Fungsi ini mengecek apakah sebuah karakter termasuk spasi.

Dipakai oleh `rapikanTeks`.

### `rapikanTeks`

```cpp
string rapikanTeks(string teks)
```

Fungsi ini menghapus spasi di awal dan akhir input.

Contoh:

```text
"  Program  "
```

menjadi:

```text
"Program"
```

### `bacaTeks`

```cpp
bool bacaTeks(string pesan, string& isi)
```

Fungsi ini dipakai untuk membaca input teks dari user.

Contoh pemakaian:

```cpp
bacaTeks("Nama file: ", namaFile);
```

Kalau input kosong, fungsi ini mengembalikan `false`.

### `ubahKeAngka`

```cpp
bool ubahKeAngka(string teks, int& angka)
```

Fungsi ini mengubah input teks menjadi angka.

Contoh:

```text
"8000"
```

menjadi:

```text
8000
```

Kalau input berisi huruf seperti `8abc`, maka dianggap salah.

### `bacaAngka`

```cpp
bool bacaAngka(string pesan, int& angka)
```

Fungsi ini membaca input angka dari user.

Dipakai saat memilih menu dan mengisi ukuran file.

## Function Stack

### `pushRiwayat`

```cpp
void pushRiwayat(...)
```

Fungsi ini memasukkan aktivitas baru ke stack undo.

Dipakai setelah:

- tambah file
- hapus file
- rename file

Aktivitas terbaru selalu masuk ke paling atas stack.

### `riwayatKosong`

```cpp
bool riwayatKosong()
```

Fungsi ini mengecek apakah stack undo kosong.

Kalau `puncakRiwayat == NULL`, berarti belum ada aktivitas yang bisa di-undo.

### `popRiwayat`

```cpp
Riwayat popRiwayat()
```

Fungsi ini mengambil aktivitas paling atas dari stack.

Dipakai saat user memilih menu undo.

## Function Linked List

### `tambahFileKeList`

```cpp
void tambahFileKeList(File*& kepala, int idFile, string namaFile, int ukuranFile)
```

Fungsi ini menambah file baru ke linked list file.

Data yang dimasukkan:

- id file
- nama file
- ukuran file

File baru dimasukkan ke bagian akhir linked list.

### `cariFileBerdasarkanNama`

```cpp
File* cariFileBerdasarkanNama(File* kepala, string namaFile)
```

Fungsi ini mencari file berdasarkan nama di dalam satu linked list.

Kalau ketemu, fungsi mengembalikan alamat file.

Kalau tidak ketemu, fungsi mengembalikan `NULL`.

### `cariFileBerdasarkanId`

```cpp
File* cariFileBerdasarkanId(File* kepala, int idFile)
```

Fungsi ini mencari file berdasarkan ID.

Fungsi ini penting untuk undo karena nama file bisa berubah, tapi ID tetap sama.

### `hapusFileDariList`

```cpp
bool hapusFileDariList(File*& kepala, string namaFile, int& idFile, int& ukuranFile)
```

Fungsi ini menghapus file dari linked list berdasarkan nama file.

Sebelum file dihapus, ID dan ukuran file disimpan dulu supaya bisa dipakai untuk undo.

### `hapusFileBerdasarkanId`

```cpp
bool hapusFileBerdasarkanId(File*& kepala, int idFile)
```

Fungsi ini menghapus file berdasarkan ID.

Dipakai untuk undo aktivitas tambah file.

Kalau user menambah file lalu undo, file itu dihapus lagi berdasarkan ID.

## Function Tree

### `buatFolder`

```cpp
Folder* buatFolder(string namaFolder)
```

Fungsi ini membuat node folder baru.

Isi awal folder:

- nama folder sesuai input
- daftar file masih kosong
- anak masih `NULL`
- saudara masih `NULL`

### `tambahFolder`

```cpp
void tambahFolder(Folder* induk, Folder* anakBaru)
```

Fungsi ini menambahkan folder baru sebagai anak dari folder induk.

Contoh:

```cpp
tambahFolder(akar, program);
```

Artinya folder `Program` dimasukkan ke dalam folder `UAS`.

### `cariFolder`

```cpp
Folder* cariFolder(Folder* node, string namaFolder)
```

Fungsi ini mencari folder di dalam tree.

Fungsi ini memakai rekursi, karena tree punya cabang.

Alurnya:

1. Cek folder sekarang.
2. Kalau belum ketemu, cari ke anak.
3. Kalau belum ketemu juga, cari ke saudara.

### `tampilStrukturFolder`

```cpp
void tampilStrukturFolder(Folder* node, int level)
```

Fungsi ini menampilkan struktur folder dan file.

Parameter `level` dipakai untuk mengatur jarak spasi supaya tampilannya terlihat seperti tree.

### `cariFileDiTree`

```cpp
void cariFileDiTree(Folder* node, string namaFile, bool& ketemu)
```

Fungsi ini mencari file di seluruh folder dalam tree.

Fungsi ini juga memakai rekursi.

Setiap folder dicek linked list filenya, lalu lanjut ke anak dan saudara.

## Function Fitur Program

### `fiturTambahFile`

Fungsi ini menjalankan menu tambah file.

Alurnya:

1. User memasukkan nama folder.
2. User memasukkan nama file.
3. User memasukkan ukuran file.
4. Program mencari folder.
5. Program mengecek apakah nama file sudah ada.
6. File ditambahkan ke linked list.
7. Aktivitas disimpan ke stack undo.

### `fiturHapusFile`

Fungsi ini menjalankan menu hapus file.

Alurnya:

1. User memasukkan nama folder.
2. User memasukkan nama file yang ingin dihapus.
3. Program mencari folder.
4. Program menghapus file dari linked list.
5. Aktivitas hapus disimpan ke stack undo.

### `fiturRenameFile`

Fungsi ini menjalankan menu rename file.

Alurnya:

1. User memasukkan nama folder.
2. User memasukkan nama file lama.
3. User memasukkan nama file baru.
4. Program mencari folder.
5. Program mencari file lama.
6. Program mengecek apakah nama baru sudah dipakai.
7. Nama file diganti.
8. Aktivitas rename disimpan ke stack undo.

### `fiturUndo`

Fungsi ini menjalankan menu undo.

Kalau aktivitas terakhir adalah:

- `TAMBAH`, maka undo akan menghapus file yang baru ditambahkan.
- `HAPUS`, maka undo akan mengembalikan file yang dihapus.
- `RENAME`, maka undo akan mengubah nama file kembali ke nama lama.

### `fiturCariFile`

Fungsi ini menjalankan menu cari file.

User cukup memasukkan nama file, lalu program mencari file itu di semua folder.

### `buatDataFolderAwal`

Fungsi ini membuat folder awal saat program pertama dijalankan.

Strukturnya:

```text
UAS
  Program
    Percobaan
  Dokumen
  Revisi
```

### `tampilMenu`

Fungsi ini menampilkan daftar menu ke layar.

### `main`

Fungsi utama program.

Alurnya:

1. Membuat folder awal.
2. Menampilkan menu.
3. Membaca pilihan user.
4. Menjalankan fitur sesuai pilihan.
5. Mengulang terus sampai user memilih `0`.

## Looping Yang Dipakai

### 1. `while` di `rapikanTeks`

Dipakai untuk menghapus spasi di awal dan akhir teks.

```cpp
while (awal < (int)teks.length() && karakterSpasi(teks[awal]))
```

dan:

```cpp
while (akhir >= awal && karakterSpasi(teks[akhir]))
```

### 2. `for` di `ubahKeAngka`

Dipakai untuk mengecek setiap karakter input angka.

```cpp
for (int i = 0; i < (int)teks.length(); i++)
```

Kalau ada karakter selain angka, input dianggap tidak valid.

### 3. `while` di linked list

Dipakai untuk menelusuri node file satu per satu.

Contohnya ada di:

- `tambahFileKeList`
- `cariFileBerdasarkanNama`
- `cariFileBerdasarkanId`
- `hapusFileDariList`
- `hapusFileBerdasarkanId`

Contoh:

```cpp
while (bantu != NULL)
```

Artinya selama node masih ada, program terus maju ke node berikutnya.

### 4. `while` di `tambahFolder`

Dipakai untuk mencari saudara folder terakhir.

```cpp
while (bantu->saudara != NULL)
```

Kalau sudah sampai folder terakhir, folder baru disambungkan ke sana.

### 5. `for` di `tampilStrukturFolder`

Dipakai untuk membuat jarak spasi sesuai level folder.

```cpp
for (int i = 0; i < level; i++)
```

Semakin dalam foldernya, semakin banyak spasinya.

### 6. `while` di `tampilStrukturFolder`

Dipakai untuk menampilkan semua file dalam satu folder.

```cpp
while (file != NULL)
```

### 7. `do while` di `main`

Dipakai supaya menu selalu muncul sampai user memilih keluar.

```cpp
do {
    ...
} while (pilihan != 0);
```

Program berhenti kalau user memilih angka `0`.

## Percabangan Yang Dipakai

Program banyak memakai `if`, `else if`, dan `else`.

Contohnya di `main`:

```cpp
if (pilihan == 1) {
    fiturTambahFile();
} else if (pilihan == 2) {
    fiturHapusFile();
}
```

Artinya program menjalankan fitur sesuai pilihan user.

Percabangan juga dipakai untuk validasi, misalnya:

- folder tidak ditemukan
- file tidak ditemukan
- nama file sudah ada
- ukuran file tidak valid
- stack undo masih kosong

## Rekursi Yang Dipakai

Rekursi adalah fungsi yang memanggil dirinya sendiri.

Program ini memakai rekursi di bagian tree.

Fungsi yang memakai rekursi:

- `cariFolder`
- `tampilStrukturFolder`
- `cariFileDiTree`

Kenapa perlu rekursi?

Karena tree punya cabang. Jadi program harus bisa masuk ke anak folder dan lanjut ke saudara folder.

## Contoh Penggunaan Program

### Tambah File

Input:

```text
1
Program
Cpp
8000
```

Hasil:

```text
File berhasil ditambah dengan ID 1.
```

### Tampilkan Struktur Folder

Input:

```text
5
```

Contoh hasil:

```text
[Folder] UAS
  [Folder] Program
    - Cpp (ID: 1, ukuran: 8000 KB)
    [Folder] Percobaan
  [Folder] Dokumen
  [Folder] Revisi
```

### Cari File

Input:

```text
6
Cpp
```

Hasil:

```text
File ditemukan di folder Program | ID: 1 | Ukuran: 8000 KB
```

### Rename File

Input:

```text
3
Program
Cpp
CppFinal
```

Hasil:

```text
File berhasil direname.
```

### Hapus File

Input:

```text
2
Program
CppFinal
```

Hasil:

```text
File berhasil dihapus.
```

### Undo

Input:

```text
4
```

Kalau aktivitas terakhir adalah hapus file, file akan dikembalikan.

## Catatan Penting

- Nama folder harus sama persis dengan yang tersedia.
- Ukuran file diisi angka saja, contohnya `8000`.
- Program belum menyimpan data ke file permanen, jadi kalau program ditutup, data akan hilang.
- Undo hanya mengambil aktivitas terakhir, karena memakai stack.
- ID file dibuat otomatis dari angka 1 dan terus bertambah.

## Kesimpulan

Program ini memakai:

- Linked List untuk daftar file.
- Stack untuk undo aktivitas.
- Tree untuk struktur folder.
- Rekursi untuk menelusuri folder.
- Looping untuk menelusuri linked list, input angka, dan menu.
