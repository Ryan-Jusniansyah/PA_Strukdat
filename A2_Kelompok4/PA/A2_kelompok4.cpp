#include <iostream>
#include <conio.h>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <iomanip>
#include <bits/stdc++.h>
#include <string>

using namespace std;

// Struct untuk item Gudang (Warehouse)
struct Gudang
{
    // Data untuk sebuah item Gudang
    int id_barang;        // ID unik untuk mengidentifikasi barang
    string nama_barang;   // Nama dari barang
    string jenis_barang;  // Jenis atau kategori barang
    int stock_barang;     // Jumlah stok atau persediaan barang
    int harga_barang;     // Harga satuan dari barang
    string tanggal_masuk; // Tanggal barang masuk ke gudang
    string Expired;       // Tanggal kadaluwarsa barang (jika ada)
    Gudang *next;         // Pointer ke elemen berikutnya dalam linked list
    Gudang *prev;         // Pointer ke elemen sebelumnya dalam linked list (double linkedlist)
};

// Struct untuk akun pengguna
struct Akun
{
    string username;
    string password;
    Akun *next = NULL;
    Akun *prev = NULL; // Menambahkan pointer 'prev' untuk double linked list
};

// Struct untuk item checkout
struct checkout
{
    int id_pembeli;
    string username;
    string nama_barang;
    string jenis_barang;
    int harga_barang;
    int jumlah;
    int total;
    string status;
    checkout *next = NULL; // Pointer ke elemen berikutnya dalam linked list
};

// Struct untuk item dalam keranjang belanja (Keranjang)
struct Keranjang
{
    int id_pembeli;
    string username;
    string nama_barang;
    string jenis_barang;
    int harga_barang;
    int jumlah;
    int total;
    Keranjang *next = NULL; // Pointer ke elemen berikutnya dalam linked list
};

// Struct untuk riwayat pembelian (Riwayat)
struct riwayat
{
    int id_pembeli;
    string username;
    string nama_barang;
    string jenis_barang;
    int harga_barang;
    int jumlah;
    int total;
    string status;
    riwayat *next = NULL; // Pointer ke elemen berikutnya dalam linked list
};

// PROTOTYPE
void login();
void menupembeli(string username, string nama);
void menugudang();
void display_keranjang(string username, Keranjang *head);
void Tulis_keranjang(Keranjang *head);

// variable global
Gudang *head, *tail, *cur, *Node_hapus;
checkout *head_check, *cur_check;
riwayat *head_riwayat;
Keranjang *head_keranjang, *cur_keranjang;
Akun *head_akun, *tail_akun;

// Fungsi untuk menghapus linked list Akun
void clear(Akun **head)
{
    *head = NULL;
    return;
}

int add_id = 0; // penghitung jumlah akun

// Fungsi untuk mendapatkan jumlah data dalam linked list Gudang
int Jumlah_Data()
{
    cur = head;
    int jumlah = 0;
    while (cur != NULL)
    {
        jumlah += 1;
        cur = cur->next;
    }
    return jumlah;
}

// Fungsi untuk membaca data Akun dari file
// Login
void Baca_Akun(Akun **head, Akun **tail)
{
    ifstream file;
    string data;
    file.open("./A2_Kelompok4/akun.csv", ios::in); // Membuka file dengan mode input

    if (file.is_open())
    {
        while (getline(file, data))
        {
            Akun *newNode = new Akun;
            stringstream ss(data);
            string namas, usernames, passwords;

            // Memisahkan data dari string yang dibaca berdasarkan pemisah koma
            getline(ss, usernames, ',');
            getline(ss, passwords, '\n');

            // Mengisi struktur Akun dengan data dari file
            newNode->username = usernames;
            newNode->password = passwords;

            // Memasukkan node baru ke linked list
            if (*head == NULL)
            {
                *head = newNode;
                *tail = newNode;
            }
            else
            {
                newNode->prev = *tail;
                (*tail)->next = newNode;
                *tail = newNode;
            }
            add_id++;
        }
        file.close();
    }
    else
    {
        cerr << "Unable to open the file Akun.csv" << endl;
    }
}

// Fungsi untuk menulis data Akun ke file
void Tulis_Akun(Akun *head)
{
    ofstream file("./A2_Kelompok4/Akun.csv", ios::out | ios::trunc); // Membuka file untuk penulisan

    if (file.is_open())
    {
        Akun *curNode = head;
        // Menulis data ke dalam file CSV
        while (curNode != NULL)
        {
            file << curNode->username << ",";
            file << curNode->password << endl;
            curNode = curNode->next;
        }
        file.close();
    }
    else
    {
        cerr << "Unable to open the file Akun.csv" << endl;
    }
}

// Fungsi untuk menambahkan akun baru ke linked list
void registrasi(Akun **head, Akun **tail)
{
    Akun *node_baru = new Akun;
    string password;
    bool ulang = true;
    Akun *cur_akun = *head;

    // Meminta input informasi akun baru
    while (ulang)
    { // Proses awal perulangan pada variabel ulang = true akan berjalan terus hinggal ulang = false.
        while (true)
        {
            system("cls");
            cout << "===================================================" << endl;
            cout << "                  Registrasi Akun                  " << endl;
            cout << "===================================================" << endl;
            cout << "Masukkan Username Pribadi/Perusahaan : ";
            fflush(stdin);
            getline(cin, node_baru->username);
            if (node_baru->username.empty())
            {
                cout << "\nUsername Tidak Boleh Kosong!\n"
                     << endl;
                getch(); // Menunggu pengguna menekan tombol atau Pause Sementara
                system("cls");
            }
            else
            {
                break;
            }
        }
        cout << "Masukkan Password                    : ";
        fflush(stdin); // membersihkan line eror/buffer
        cin >> password;
        cout << "Konfirmasi Password                  : ";
        cin >> node_baru->password;

        // Memvalidasi data yang dimasukkan
        if (*head == NULL)
        {
            break;
        }
        else if (password != node_baru->password)
        {
            cout << "\nPassword Yang Anda Masukkan Berbeda, Silahkan Input Kembali (Enter)";
            getch();       // Menunggu pengguna menekan tombol atau Pause Sementara
            system("cls"); // Membersikan tampilan sebelumnya
        }
        else
        {
            while (cur_akun != NULL)
            {
                if (cur_akun->username == node_baru->username)
                {
                    cout << "\nUsername Telah Digunakan, Silahkan Gunakan Username Yang Lain (Enter)";
                    getch(); // Menunggu pengguna menekan tombol atau Pause Sementara
                    system("cls");
                    break;
                }
                else
                {
                    cur_akun = cur_akun->next;
                }
                if (cur_akun == NULL)
                {
                    ulang = false; // Perulangan berhenti
                    break;
                }
            }
        }
    }

    // Menambahkan akun baru ke linked list
    node_baru->next = *head;
    if (*head != NULL)
    {
        (*head)->prev = node_baru;
    }
    node_baru->prev = NULL;
    *head = node_baru;
    Tulis_Akun(*head); // Menulis data ke file setelah penambahan akun baru
    cout << "===========================================" << endl;
    cout << "          Berhasil Register Akun!          " << endl;
    cout << "===========================================" << endl;
    getch();
}

// Fungsi untuk menghitung jumlah item dalam keranjang belanja (Keranjang)
int hitung_keranjang(string username, Keranjang **head)
{
    cur_keranjang = *head;
    int hasil = 0;
    while (cur_keranjang != NULL)
    {
        if (cur_keranjang->username == username)
        {
            hasil += 1;
        }
        cur_keranjang = cur_keranjang->next;
    }
    return hasil;
}
// Fungsi untuk membaca data Gudang dari file
void Baca_gudang(Gudang **head, Gudang **tail)
{
    // Membuka file untuk membaca data gudang
    ifstream file;
    string isi, id_barangs, nama_barangs, jenis_barangs, stock_barangs, harga_barangs, tanggal_masuks, Expireds;
    int id_int, stock_int, harga_int;

    file.open("./A2_Kelompok4/gudang.csv", ios::in);
    if (file.is_open())
    {
        // Membaca data per baris dari file
        while (getline(file, isi))
        {
            // Membuat node baru untuk menyimpan data gudang
            Gudang *newNode = new Gudang;
            stringstream ss(isi);

            // Memisahkan data dari string yang dibaca berdasarkan pemisah koma
            getline(ss, id_barangs, ',');
            getline(ss, nama_barangs, ',');
            getline(ss, jenis_barangs, ',');
            getline(ss, stock_barangs, ',');
            getline(ss, harga_barangs, ',');
            getline(ss, tanggal_masuks, ',');
            getline(ss, Expireds, '\n');
            // Mengonversi string ke tipe data yang sesuai
            stringstream id_string(id_barangs);
            stringstream stock_string(stock_barangs);
            stringstream harga_string(harga_barangs);

            id_string >> id_int;
            stock_string >> stock_int;
            harga_string >> harga_int;

            // Mengisi struct Gudang dengan data dari file
            newNode->id_barang = id_int;
            newNode->nama_barang = nama_barangs;
            newNode->jenis_barang = jenis_barangs;
            newNode->stock_barang = stock_int;
            newNode->harga_barang = harga_int;
            newNode->tanggal_masuk = tanggal_masuks;
            newNode->Expired = Expireds;

            // Memasukkan node baru ke linked list
            if (*head == NULL && *tail == NULL)
            {
                newNode->prev = *tail;
                newNode->next = NULL;
                *head = newNode;
                *tail = newNode;
            }
            else
            {
                newNode->prev = *tail;
                newNode->next = NULL;
                (*tail)->next = newNode;
                *tail = newNode;
            }
        }
    }
    file.close(); // Menutup file setelah selesai membaca
}

// Fungsi untuk membaca data checkout dari file
void Baca_queue(checkout **head)
{
    // Membuka file untuk membaca data gudang
    ifstream file;
    string isi, id_pembelis, namas, nama_barangs, jenis_barangs, harga_barangs, jumlahs, totals, statuses, usernames;
    int pembeli_int, harga_int, jumlah_int, total_int;

    file.open("./A2_Kelompok4/checkout.csv", ios::in);
    if (file.is_open())
    {
        while (getline(file, isi))
        { // Membuat node baru untuk menyimpan data gudang
            cur_check = *head;
            checkout *newNode = new checkout;
            stringstream ss(isi);
            // Memisahkan data dari string yang dibaca berdasarkan pemisah koma
            getline(ss, id_pembelis, ',');
            getline(ss, usernames, ',');
            getline(ss, nama_barangs, ',');
            getline(ss, jenis_barangs, ',');
            getline(ss, harga_barangs, ',');
            getline(ss, jumlahs, ',');
            getline(ss, totals, ',');
            getline(ss, statuses, '\n');
            // Mengonversi string ke tipe data yang sesuai
            stringstream id_string(id_pembelis);
            stringstream harga_string(harga_barangs);
            stringstream jumlah_string(jumlahs);
            stringstream total_string(totals);

            id_string >> pembeli_int;
            harga_string >> harga_int;
            jumlah_string >> jumlah_int;
            total_string >> total_int;
            // Mengisi struct Gudang dengan data dari file
            newNode->id_pembeli = pembeli_int;
            newNode->username = usernames;
            newNode->nama_barang = nama_barangs;
            newNode->jenis_barang = jenis_barangs;
            newNode->harga_barang = harga_int;
            newNode->jumlah = jumlah_int;
            newNode->total = total_int;
            newNode->status = statuses;
            // Memasukkan node baru ke linked list
            if (*head == NULL)
            {
                *head = newNode;
            }
            else
            {
                while (cur_check->next != NULL)
                {
                    cur_check = cur_check->next;
                }
                cur_check->next = newNode;
            }
        }
    }
    file.close(); // Menutup file setelah selesai membaca
}

// Fungsi untuk membaca data riwayat pembelian (Riwayat) dari file
void Baca_riwayat(riwayat **Head)
{
    ifstream file;
    riwayat *temp = *Head;
    string isi, id_pembelis, namas, nama_barangs, jenis_barangs, harga_barangs, jumlahs, totals, statuses, usernames;
    int pembeli_int1, harga_int1, jumlah_int1, total_int1;

    file.open("./A2_Kelompok4/riwayat.csv", ios::in);
    if (file.is_open())
    {
        while (getline(file, isi))
        {
            temp = *Head;
            riwayat *newNode = new riwayat;
            stringstream ss(isi);

            getline(ss, id_pembelis, ',');
            getline(ss, usernames, ',');
            getline(ss, nama_barangs, ',');
            getline(ss, jenis_barangs, ',');
            getline(ss, harga_barangs, ',');
            getline(ss, jumlahs, ',');
            getline(ss, totals, ',');
            getline(ss, statuses, '\n');

            stringstream id_string(id_pembelis);
            stringstream harga_string(harga_barangs);
            stringstream jumlah_string(jumlahs);
            stringstream total_string(totals);

            id_string >> pembeli_int1;
            harga_string >> harga_int1;
            jumlah_string >> jumlah_int1;
            total_string >> total_int1;

            newNode->id_pembeli = pembeli_int1;
            newNode->username = usernames;
            newNode->nama_barang = nama_barangs;
            newNode->jenis_barang = jenis_barangs;
            newNode->harga_barang = harga_int1;
            newNode->jumlah = jumlah_int1;
            newNode->total = total_int1;
            newNode->status = statuses;

            if (*Head == NULL)
            {
                *Head = newNode;
            }
            else
            {
                while (temp->next != NULL)
                {
                    temp = temp->next;
                }
                temp->next = newNode;
            }
        }
    }
    file.close();
}

// Fungsi untuk menulis data Gudang ke file
void Tulis_gudang(Gudang *head)
{
    fstream file;
    cur = head;

    file.open("./A2_Kelompok4/gudang.csv", ios::trunc | ios::out | ios::in);
    if (file.is_open())
    {
        while (cur != NULL)
        {
            file << cur->id_barang << ",";
            file << cur->nama_barang << ",";
            file << cur->jenis_barang << ",";
            file << cur->stock_barang << ",";
            file << cur->harga_barang << ",";
            file << cur->tanggal_masuk << ",";
            file << cur->Expired << endl;
            cur = cur->next;
        }
    }
    file.close();
}

// Fungsi untuk menulis data checkout ke file
void Tulis_queue(checkout *head)
{
    fstream file;
    cur_check = head;

    file.open("./A2_Kelompok4/checkout.csv", ios::trunc | ios::out | ios::in);
    if (file.is_open())
    {
        while (cur_check != NULL)
        {
            file << cur_check->id_pembeli << ",";
            file << cur_check->username << ",";
            file << cur_check->nama_barang << ",";
            file << cur_check->jenis_barang << ",";
            file << cur_check->harga_barang << ",";
            file << cur_check->jumlah << ",";
            file << cur_check->total << ",";
            file << cur_check->status << endl;
            cur_check = cur_check->next;
        }
    }
    file.close();
}

// Fungsi untuk menulis data riwayat pembelian (Riwayat) ke file
void Tulis_riwayat(riwayat *head)
{
    fstream file;
    riwayat *temp = head;

    file.open("./A2_Kelompok4/riwayat.csv", ios::trunc | ios::out | ios::in);
    if (file.is_open())
    {
        while (temp != NULL)
        {
            file << temp->id_pembeli << ",";
            file << temp->username << ",";
            file << temp->nama_barang << ",";
            file << temp->jenis_barang << ",";
            file << temp->harga_barang << ",";
            file << temp->jumlah << ",";
            file << temp->total << ",";
            file << temp->status << endl;
            temp = temp->next;
        }
    }
    file.close();
}

// Fungsi untuk menampilkan item Gudang
void display_gudang(Gudang *head)
{
    system("cls");
    // Menampilkan pesan jika gudang masih kosong
    if (head == NULL)
    {
        cout << "======================================" << endl;
        cout << "|         Gudang Masih Kosong        |" << endl;
        cout << "======================================" << endl;
        cout << "Masukkan Barang, baru terisi hehe :D";
        getch();
    }
    else
    { // Menampilkan header daftar barang
        string garis(127, '=');
        cout << garis << endl;
        cout << "|                                                         Daftar Barang                                                       |" << endl;
        cout << garis << endl;
        cout << "|" << setiosflags(ios::left) << setw(1) << "No"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(7) << "ID Barang"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(20) << "Nama Barang"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(20) << "Jenisn Barang"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(12) << "Stock Barang"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(16) << "Harga Barang"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(16) << "Tanggal Masuk"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(16) << "Expired Barang"
             << "|" << endl;
        cout << garis << endl;
        int i = 1;
        while (head != NULL)
        { // Menampilkan data barang
            cout << resetiosflags(ios::adjustfield);
            cout << "|" << setiosflags(ios::left) << setw(2) << i << "|";
            cout << " " << setiosflags(ios::left) << setw(9) << head->id_barang << "|";
            cout << " " << setiosflags(ios::left) << setw(20) << head->nama_barang << "|";
            cout << " " << setiosflags(ios::left) << setw(20) << head->jenis_barang << "|";
            cout << " " << setiosflags(ios::left) << setw(12) << head->stock_barang << "|";
            cout << " " << setiosflags(ios::left) << setw(16) << head->harga_barang << "|";
            cout << " " << setiosflags(ios::left) << setw(16) << head->tanggal_masuk << "|";
            cout << " " << setiosflags(ios::left) << setw(16) << head->Expired << "|";
            cout << endl;
            // Memperbarui pointer head dan counter
            tail = head;
            head = head->next;
            i++;
        }
        // Menampilkan footer dan menunggu pengguna menekan tombol sebelum melanjutkan
        cout << resetiosflags(ios::adjustfield);
        cout << garis << endl;
        cout << "\nTekan ENTER Untuk Melanjutkan...";
        getch(); // Menunggu pengguna menekan tombol sebelum melanjutkan atau Pause sementara
    }
}

// Fungsi untuk menambahkan data riwayat pembelian (Riwayat)
void add_riwayat(riwayat **Head, checkout *temp, string status)
{
    riwayat *newNode = new riwayat; // Membuat node baru untuk menyimpan data riwayat transaksi
    riwayat *add = *Head;           // Inisialisasi pointer riwayat yang digunakan untuk traversal
    // Mengisi data riwayat baru dengan informasi dari data checkout sementara
    newNode->id_pembeli = temp->id_pembeli;
    newNode->username = temp->username;
    newNode->nama_barang = temp->nama_barang;
    newNode->jenis_barang = temp->jenis_barang;
    newNode->harga_barang = temp->harga_barang;
    newNode->jumlah = temp->jumlah;
    newNode->total = temp->total;
    newNode->status = status;
    // Menambahkan node ke linked list riwayat
    if (*Head == NULL)
    { // Jika linked list masih kosong, node baru menjadi head
        *Head = newNode;
    }
    else
    { // Jika linked list sudah berisi, mencari posisi tail dan menambahkan node baru
        while (add->next != NULL)
        {
            add = add->next;
        }
        add->next = newNode;
    }
}

// Fungsi untuk menampilkan antrian checkout
void display_antrian(checkout *head)
{
    cur_check = head;
    system("cls");
    if (head == NULL)
    {
        cout << "=============================================" << endl;
        cout << "|                Antrian Kosong             |" << endl;
        cout << "=============================================" << endl;
        getch();
    }
    else
    {
        string garis(138, '=');
        cout << garis << endl;
        cout << "|                                                                   Daftar Antrian                                                       |" << endl;
        cout << garis << endl;
        cout << "|" << setiosflags(ios::left) << setw(3) << " No"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(13) << " ID Pembeli"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(14) << " Nama Pembeli"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(20) << " Nama Barang"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(16) << " Jenis Barang"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(16) << " Harga Barang"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(15) << " Jumlah Pesanan"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(12) << " Total Harga"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(11) << " Status"
             << "|" << endl;
        cout << garis << endl;
        int i = 1;
        while (cur_check != NULL)
        {
            cout << resetiosflags(ios::adjustfield);
            cout << "|" << setiosflags(ios::left) << setw(3) << i << "|";
            cout << " " << setiosflags(ios::left) << setw(13) << cur_check->id_pembeli << "|";
            cout << " " << setiosflags(ios::left) << setw(14) << cur_check->username << "|";
            cout << " " << setiosflags(ios::left) << setw(20) << cur_check->nama_barang << "|";
            cout << " " << setiosflags(ios::left) << setw(16) << cur_check->jenis_barang << "|";
            cout << " " << setiosflags(ios::left) << setw(16) << cur_check->harga_barang << "|";
            cout << " " << setiosflags(ios::left) << setw(15) << cur_check->jumlah << "|";
            cout << " " << setiosflags(ios::left) << setw(12) << cur_check->total << "|";
            cout << " " << setiosflags(ios::left) << setw(11) << cur_check->status << "|";
            cout << endl;
            cur_check = cur_check->next;
            i++;
        }
        cout << resetiosflags(ios::adjustfield);
        cout << garis << endl;
        cout << "\nJika ingin lanjut tekan ENTER";
        getch();
    }
}

// Fungsi untuk konfirmasi pesanan pada antrian checkout
void Konfirmasi_antrian(checkout **head, riwayat **Head, Gudang *gdg)
{ // Inisialisasi pointer untuk traversal di gudang
    Gudang *temp = gdg;
    char confirm; // Variabel untuk menyimpan konfirmasi pengguna (y/n/k)
    system("cls");
    if (*head == NULL) // Menampilkan pesan jika antrian checkout kosong
    {
        cout << "========================================" << endl;
        cout << "|            Antrian Kosong            |" << endl;
        cout << "========================================" << endl;
        getch(); // Menunggu pengguna menekan tombol sebelum melanjutkan
    }
    else
    {
        int i = 1;
        cur_check = *head; // Pointer cur_check untuk traversal di linked list checkout
        while (cur_check != NULL)
        {
            while (true) // Menampilkan informasi pesanan dan meminta konfirmasi dan perulangan
            {
                system("cls");
                cout << "\n==========================================" << endl;
                cout << "|            Konfirmasi Pesananan         |" << endl;
                cout << "============================================" << endl;
                cout << "| Antrian [" << i << "]  : " << endl;
                cout << "| ID Pembeli     : " << cur_check->id_pembeli << endl;
                cout << "| Nama Pembeli   : " << cur_check->username << endl;
                cout << "| Nama Barang    : " << cur_check->nama_barang << endl;
                cout << "| Jenis Barang   : " << cur_check->jenis_barang << endl;
                cout << "| Harga Barang   : Rp " << cur_check->harga_barang << endl;
                cout << "| Jumlah Barang  : " << cur_check->jumlah << endl;
                cout << "| Total Harga    : Rp " << cur_check->total << endl;
                cout << "| Status Pesanan : " << cur_check->status << endl;
                cout << "=============================================\n"
                     << endl;
                cout << "\nKetik K jika ingin kembali ke Menu Utama" << endl;
                cout << "\nKonfirmasi Pesanan Sukses (y/n) : ";
                confirm = getch();
                checkout *hapus = cur_check; // Menghapus node checkout dan melakukan konfirmasi sesuai input pengguna
                if (confirm == 'y' || confirm == 'Y')
                {
                    *head = (*head)->next;
                    while (temp != NULL)
                    { // Memproses stok barang di gudang sesuai dengan pesanan
                        if (temp->id_barang == cur_check->id_pembeli)
                        {
                            if (temp->stock_barang >= cur_check->jumlah)
                            {
                                temp->stock_barang = temp->stock_barang - cur_check->jumlah;
                                add_riwayat(Head, cur_check, "Sukses");
                            }
                            else
                            {
                                add_riwayat(Head, cur_check, "Stock Kurang");
                            }
                            break;
                        }
                        temp = temp->next;
                    }
                    delete hapus;
                    break;
                }
                else if (confirm == 'n' || confirm == 'N')
                {
                    *head = (*head)->next;
                    add_riwayat(Head, cur_check, "Gagal");
                    delete hapus;
                    break;
                }
                else if (confirm == 'k' || confirm == 'K')
                {
                    break;
                }
                else
                {
                    cout << "\nMasukkan yang benar, sesuaikan dengan perintah yang ada!!!";
                    getch();
                }
            }
            if (confirm == 'k' || confirm == 'K') // Kembali ke menu gudang jika pengguna memilih kembali
            {
                menugudang();
            } // Melanjutkan ke pesanan berikutnya dalam antrian checkout
            cur_check = cur_check->next;
            i++;
        }
        // Menulis perubahan stok dan antrian ke file
        Tulis_gudang(gdg);
        Tulis_queue(*head);
        Tulis_riwayat(*Head);
    }
}

// Fungsi untuk menambahkan pesanan ke dalam antrian checkout
void enqueue_pesanan(string username, Keranjang **head, checkout **HEAD)
{
    Keranjang *temp_delete = *head;     // Inisialisasi pointer keranjang untuk traversal dan penghapusan
    cur_check = *HEAD;                  // Inisialisasi pointer checkout untuk traversal
    Keranjang *temp = *head;            // Inisialisasi pointer keranjang untuk traversal
    int no, tanda = 0;                  // Variabel untuk menyimpan nomor pesanan yang akan di-checkout
    display_keranjang(username, *head); // Menampilkan daftar item dalam keranjang pengguna
    cout << "\nIngin Checkout Data Keranjang Nomor Berapa : ";
    cin >> no;
    if (no < 0 || no > hitung_keranjang(username, head)) // Memproses input nomor pesanan
    {
        cout << "\nNomor Yand Anda Masukkan Tidak Sesuai!";
        getch(); // Menunggu pengguna menekan tombol sebelum melanjutkan
    }
    else if (no > 0 && no <= hitung_keranjang(username, head))
    { // Melakukan traversal hingga menemukan item yang akan di-checkout
        while (tanda != no)
        {
            if (temp_delete->username == username)
            {
                tanda += 1;
            }
            else
            {
                temp_delete = temp_delete->next;
            }
        }
        // Membuat node checkout baru dan mengisi dengan informasi dari item keranjang
        checkout *newNode = new checkout;
        newNode->id_pembeli = temp_delete->id_pembeli;
        newNode->username = username;
        newNode->nama_barang = temp_delete->nama_barang;
        newNode->jenis_barang = temp_delete->jenis_barang;
        newNode->harga_barang = temp_delete->harga_barang;
        newNode->jumlah = temp_delete->jumlah;
        newNode->total = temp_delete->total;
        // Menandai status pesanan sebagai "Menunggu"
        newNode->status = "Menunggu";
        if (*HEAD == NULL) // Menambahkan node checkout ke dalam linked list checkout
        {
            *HEAD = newNode;
        }
        else
        {
            while (cur_check->next != NULL)
            {
                cur_check = cur_check->next;
            }
            cur_check->next = newNode;
        }
        Tulis_queue(*HEAD);       // Menulis perubahan antrian checkout ke file
        if (temp_delete == *head) // Menghapus node checkout dari linked list keranjang
        {
            *head = (*head)->next;
        }
        else
        {
            tanda = 0;
            while (tanda != no - 1)
            {
                if (temp->username == username)
                {
                    tanda += 1;
                }
                else
                {
                    temp = temp->next;
                }
            }
            temp->next = temp_delete->next;
        }
        delete temp_delete;     // Menghapus node checkout yang sudah diambil dari keranjang
        Tulis_keranjang(*head); // Menulis perubahan keranjang ke file
        system("cls");
        cout << "\n================================================" << endl;
        cout << "|          Berhasil Checkout Pesanan           |" << endl;
        cout << "================================================" << endl;
        cout << "| Pesanan atas nama  : " << newNode->username << endl;
        cout << "| Nama Barang        : " << newNode->nama_barang << endl;
        cout << "| Harga Barang       : " << newNode->harga_barang << endl;
        cout << "| Jenis Barang       : " << newNode->jenis_barang << endl;
        cout << "| Jumlah Pesanan     : " << newNode->jumlah << endl;
        cout << "| Total Harga        : " << newNode->total << endl;
        cout << "| Status Pesanan     : " << newNode->status << endl;
        cout << "=================================================" << endl;
        cout << "\nTekan ENTER Untuk Lanjut!";
        getch();
    }
    else
    { // Menangani kesalahan input / eror handling
        cin.clear();
        cin.ignore(100, '\n');
        cout << "\nInputan Anda Salah!";
        getch();
    }
}

// Fungsi untuk mengecek apakah pengguna memiliki pesanan yang sedang dicekout
int cek_chec(string username, checkout *head)
{
    cur_check = head;
    while (cur_check != NULL)
    {
        if (cur_check->username == username)
        {
            return 1;
        }
        else
        {
            cur_check = cur_check->next;
        }
    }
    return -1;
}

// Fungsi untuk menampilkan pesanan yang sedang dicekout oleh pengguna
void display_check(string username, checkout *head)
{
    cur_check = head;
    system("cls");
    if (cek_chec(username, head) == -1)
    {
        cout << "=======================================" << endl;
        cout << "|     Daftar Checkout Anda Kosong     |" << endl;
        cout << "=======================================" << endl;
        cout << "\nPastikan Pesanan anda lagi!!";
        getch();
    }
    else
    {
        string garis(132, '=');
        cout << garis << endl;
        cout << "|                                                            Daftar Checkout Anda                                                  |" << endl;
        cout << garis << endl;
        cout << "|" << setiosflags(ios::left) << setw(3) << "No"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(10) << "ID Pembeli"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(13) << "Nama Pembeli"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(20) << "Nama Barang"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(16) << "Jenis Barang"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(16) << "Harga Barang"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(14) << "Jumlah Pesanan"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(12) << "Total Harga"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(10) << "Status"
             << "|" << endl;
        cout << garis << endl;

        int i = 1;
        while (cur_check != NULL)
        {
            if (cur_check->username == username)
            {
                cout << resetiosflags(ios::adjustfield);
                cout << "|" << setiosflags(ios::left) << setw(3) << i << "|";
                cout << " " << setiosflags(ios::left) << setw(10) << cur_check->id_pembeli << "|";
                cout << " " << setiosflags(ios::left) << setw(13) << cur_check->username << "|";
                cout << " " << setiosflags(ios::left) << setw(20) << cur_check->nama_barang << "|";
                cout << " " << setiosflags(ios::left) << setw(16) << cur_check->jenis_barang << "|";
                cout << " " << setiosflags(ios::left) << setw(16) << cur_check->harga_barang << "|";
                cout << " " << setiosflags(ios::left) << setw(14) << cur_check->jumlah << "|";
                cout << " " << setiosflags(ios::left) << setw(12) << cur_check->total << "|";
                cout << " " << setiosflags(ios::left) << setw(10) << cur_check->status << "|";
                cout << endl;
                i++;
            }
            cur_check = cur_check->next;
        }
        cout << resetiosflags(ios::adjustfield);
        cout << garis << endl;
        cout << "\nTekan ENTER untuk Lanjut!";
        getch();
    }
}

// Fungsi untuk menampilkan riwayat pembelian pengguna
void display_riwayat(riwayat *head)
{
    riwayat *temp = head;
    system("cls");
    if (head == NULL)
    {
        cout << "=========================================" << endl;
        cout << "|     Daftar Riwayat Pesanan Kosong     |" << endl;
        cout << "=========================================" << endl;
        getch();
    }
    else
    {
        string garis(137, '=');
        cout << garis << endl;
        cout << "|                                                               Daftar Riwayat Pesanan                                                  |" << endl;
        cout << garis << endl;
        cout << "|" << setiosflags(ios::left) << setw(3) << "No"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(10) << "ID Pembeli"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(14) << "Nama Pembeli"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(20) << "Nama Barang"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(16) << "Jenis Barang"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(16) << "Harga Barang"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(15) << "Jumlah Pesanan"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(12) << "Total Harga"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(13) << "Status"
             << "|" << endl;
        cout << garis << endl;

        int i = 1;
        while (temp != NULL)
        {
            cout << resetiosflags(ios::adjustfield);
            cout << "|" << setiosflags(ios::left) << setw(3) << i << "|";
            cout << " " << setiosflags(ios::left) << setw(10) << temp->id_pembeli << "|";
            cout << " " << setiosflags(ios::left) << setw(14) << temp->username << "|";
            cout << " " << setiosflags(ios::left) << setw(20) << temp->nama_barang << "|";
            cout << " " << setiosflags(ios::left) << setw(16) << temp->jenis_barang << "|";
            cout << " " << setiosflags(ios::left) << setw(16) << temp->harga_barang << "|";
            cout << " " << setiosflags(ios::left) << setw(15) << temp->jumlah << "|";
            cout << " " << setiosflags(ios::left) << setw(12) << temp->total << "|";
            cout << " " << setiosflags(ios::left) << setw(13) << temp->status << "|";
            cout << endl;
            i++;
            temp = temp->next;
        }
        cout << resetiosflags(ios::adjustfield);
        cout << garis << endl;
        cout << "\nTekan ENTER untuk Lanjut!";
        getch();
    }
}

// Fungsi untuk membersihkan semua riwayat pembelian dari linked list
void clear_riwayat(riwayat *head)
{
    riwayat *temp = head, *hapus; // Inisialisasi pointer temp dan hapus untuk traversal dan penghapusan
    while (temp != NULL)          // Melakukan iterasi hingga seluruh node riwayat dihapus
    {                             // Menandai node yang akan dihapus
        hapus == temp;
        if (temp->next == NULL)
        {
            head = NULL; // Mengatur head baru setelah node dihapus
        }
        else
        {
            head = head->next;
        }
        delete hapus;      // Menghapus node
        temp = temp->next; // Pindah ke node berikutnya
    }
}

// Fungsi untuk menambahkan barang baru ke dalam gudang
Gudang *add_gudang(Gudang **head)
{
    bool ulang;
    ulang = true;
    Gudang *nodeBaru = new Gudang;// Membuat node baru untuk menyimpan data barang
    Gudang *temp = *head; // Inisialisasi pointer untuk traversal
    cout << "==========================================" << endl;
    cout << "|              Tambah Barang             |" << endl;
    cout << "==========================================" << endl;
    while (ulang) // Looping untuk memasukkan ID Barang yang unik
    {
        cout << "| Masukkan ID Barang        > ";
        cin >> nodeBaru->id_barang;

        if (nodeBaru->id_barang > 0)
        {
            temp = *head;            // Set temp kembali ke awal (head)
            bool idAvailable = true; // Tambahkan variabel untuk menandai ketersediaan ID

            while (temp != NULL)
            {
                if (temp->id_barang == nodeBaru->id_barang)
                {
                    idAvailable = false; // ID tidak tersedia
                    cout << "\nID Barang telah digunakan, silahkan ganti ID Barang terbaru.\n"
                         << endl;
                    break;
                }
                temp = temp->next;
            }

            if (idAvailable)
            {
                ulang = false; // Jika ID tersedia, keluar dari ulang
            }
        }
        else
        {
            cin.clear();
            cin.ignore(100, '\n');
            cout << "\nInputan anda salah, Lakukan sesuai perintah!\n"
                 << endl;
        }
    }
    while (true)// Looping untuk memasukkan Nama Barang yang tidak boleh kosong
    {
        cout << "| Masukkan Nama Barang       > ";
        fflush(stdin);
        getline(cin, nodeBaru->nama_barang);
        if (nodeBaru->nama_barang == "")
        {
            cout << "\nNama Barang tidak boleh kosong!\n"
                 << endl;
        }
        else
        {
            break;
        }
    }
    while (true)
    {
        cout << "| Masukkan Jenis Barang       > ";
        fflush(stdin);
        getline(cin, nodeBaru->jenis_barang);
        if (nodeBaru->jenis_barang == "")
        {
            cout << "\nJenis Barang tidak boleh kosong!\n"
                 << endl;
        }
        else
        {
            break;
        }
    }
    while (true)// Looping untuk memasukkan Stock Barang yang harus lebih dari 0
    {
        cout << "| Masukkan Stock Barang       > ";
        cin >> nodeBaru->stock_barang;
        if (nodeBaru->stock_barang > 0)
        {
            break;
        }
        else
        {
            cin.clear();
            cin.ignore(100, '\n');
            cout << "\nInputan anda salah, lakukan sesuai perintah!" << endl;
        }
    }
    while (true) // Looping untuk memasukkan Harga Barang yang harus lebih dari 0
    {
        cout << "| Masukkan Harga Barang       > ";
        cin >> nodeBaru->harga_barang;
        if (nodeBaru->harga_barang > 0)
        {
            break;
        }
        else
        {
            cin.clear();
            cin.ignore(100, '\n');
            cout << "\nInputan anda salah, lakukan sesuai perintah!" << endl;
        }
    }
    while (true)
    {
        cout << "| Masukkan Tanggal Masuk(DD/MM/YYYY)     > ";
        fflush(stdin);
        getline(cin, nodeBaru->tanggal_masuk);
        if (nodeBaru->tanggal_masuk == "")
        {
            cout << "\nTanggal Masuk tidak boleh kosong!\n"
                 << endl;
        }
        else
        {
            break;
        }
    }
    while (true)// Looping untuk memasukkan Tanggal Expired yang tidak boleh kosong
    {
        cout << "| Masukkan Expired Barang(DD/MM/YYYY)     > ";
        fflush(stdin);
        getline(cin, nodeBaru->Expired);
        if (nodeBaru->Expired == "")
        {
            cout << "\nExpired Barang tidak boleh kosong!\n"
                 << endl;
        }
        else
        {
            break;
        }
    }
    cout << endl;
    return nodeBaru;// Mengembalikan node baru yang telah diisi data
}

// Fungsi untuk mengecek apakah pengguna memiliki riwayat pembelian
// Fungsi untuk memeriksa apakah riwayat transaksi pengguna sudah ada dalam linked list riwayat
// Input: username (string) - Nama pengguna yang ingin diperiksa riwayatnya
//        head (riwayat*) - Pointer ke awal linked list riwayat
// Output: 1 - Riwayat pengguna ditemukan
//         -1 - Riwayat pengguna tidak ditemukan
int cek_riwayat_user(string username, riwayat *head)
{
    riwayat *temp = head;// Inisialisasi pointer untuk traversal
    while (temp != NULL)// Iterasi melalui linked list riwayat /Perulangan
    {
        if (temp->username == username) // Memeriksa apakah nama pengguna pada node saat ini sesuai dengan username yang dicari
        {
            return 1;// Riwayat pengguna ditemukan
            break;
        }
        temp = temp->next;  // Pindah ke node berikutnya
    }
    return -1;// Riwayat pengguna tidak ditemukan
}

// Fungsi untuk menampilkan riwayat pembelian pengguna berdasarkan username
void display_riwayat_user(string username, riwayat *head)
{
    riwayat *temp = head;
    system("cls");
    if (cek_riwayat_user(username, head) == -1)
    {
        cout << "===========================================" << endl;
        cout << "|   Daftar Riwayat Pesanan masih kosong   |" << endl;
        cout << "===========================================" << endl;
        cout << "\nTekan ENTER Untuk Lanjut!";
        getch();
    }
    else
    {
        cout << "============================================" << endl;
        cout << "|       Daftar Riwayat Pesanan Barang      |" << endl;
        cout << "============================================" << endl;
        int i = 1;
        while (temp != NULL)
        {
            if (temp->username == username)
            {
                cout << "| No." << i << "   : " << endl;
                cout << "| ID Pembeli     : " << temp->id_pembeli << endl;
                cout << "| Nama Pembeli   : " << temp->username << endl;
                cout << "| Nama Barang    : " << temp->nama_barang << endl;
                cout << "| Jenis Barang   : " << temp->jenis_barang << endl;
                cout << "| Harga Barang   : " << temp->harga_barang << endl;
                cout << "| Jumlah Barang  : " << temp->jumlah << endl;
                cout << "| Total Harga    : " << temp->total << endl;
                cout << "| Status Pesanan : " << temp->status << endl;
                cout << "=============================================\n"
                     << endl;
                i++;
            }
            temp = temp->next;
        }
        cout << "\nTekan ENTER Untuk Lanjut!";
        getch();
    }
}

// Fungsi untuk menambahkan barang ke gudang sebagai elemen pertama
void addfirst(Gudang **head, Gudang **tail)
{
    system("cls");
    Gudang *nodeBaru = add_gudang(head); // Membuat node baru dengan memanggil fungsi add_gudang
    // Mengatur pointer prev dan next pada nodeBaru
    nodeBaru->prev = NULL;
    nodeBaru->next = NULL;
    if (*head == NULL)// Memeriksa apakah linked list Gudang masih kosong
    { // Jika kosong, nodeBaru menjadi elemen pertama dan terakhir
        *head = nodeBaru;
        *tail = nodeBaru;
    }
    else
    {// Jika tidak kosong, nodeBaru menjadi elemen pertama
        nodeBaru->prev = NULL;
        nodeBaru->next = *head;
        (*head)->prev = nodeBaru;
        *head = nodeBaru;
    }
    Tulis_gudang(*head); // Menulis perubahan linked list Gudang ke file
    cout << "=============================================" << endl;
    cout << "|     Barang berhasil ditambahkan Pertama   |" << endl;
    cout << "=============================================" << endl;
    getch();// Menunggu pengguna menekan tombol sebelum melanjutkan
}

// Fungsi untuk menambahkan barang ke gudang di tengah-tengah sebagai elemen baru
void addMiddle(Gudang **head, Gudang **tail)
{
    system("cls");
    Gudang *nodeBaru = add_gudang(head);// Membuat node baru dengan memanggil fungsi add_gudang
    int nomor;// Variabel untuk menyimpan nomor posisi tengah yang diinginkan oleh pengguna
    int ulang = 0;// Variabel untuk menandai apakah proses input nomor sudah selesai
    while (ulang == 0) // Looping untuk memastikan nomor posisi tengah valid
    {
        cout << "| Pilih Masukkan Data Ke Nomor : ";
        cin >> nomor;
        if (nomor > 1 && nomor < Jumlah_Data() + 1)// Memeriksa apakah nomor posisi tengah valid
        { // Inisialisasi pointer cur untuk traversal
            cur = *head;
            int Nomor = 1;
            while (Nomor < nomor - 1)// Traversal hingga posisi sebelum posisi tengah yang diinginkan
            {
                cur = cur->next;
                Nomor++;
            }
            // Menyusun pointer next dan prev pada nodeBaru dan node sekitarnya
            cur->next->prev = nodeBaru;
            nodeBaru->next = cur->next;
            cur->next = nodeBaru;
            nodeBaru->prev = cur;
            cout << endl;
            Tulis_gudang(*head); // Menulis perubahan linked list Gudang ke file
            cout << "===========================================" << endl;
            cout << "|    Barang Berhasil Ditambah Ditengah    |" << endl;
            cout << "===========================================";
            getch();
            ulang++;// Keluar dari loop karena proses sudah selesai
        }
        else
        {
            cout << "Barang harus ditambah di tengah!" << endl;
        }
    }
}

// Fungsi untuk menambahkan barang ke gudang sebagai elemen terakhir
void addLast(Gudang **head, Gudang **tail)
{
    system("cls");
    Gudang *nodeBaru = add_gudang(head);  // Membuat node baru dengan memanggil fungsi add_gudang
    if (Jumlah_Data() >= 1)// Memeriksa apakah sudah ada data barang dalam linked list
    {
        nodeBaru->next = NULL;// Jika sudah ada, menyesuaikan pointer next dan prev pada nodeBaru dan node terakhir
        nodeBaru->prev = *tail;
        if (*head == NULL && *tail == NULL)  // Memeriksa apakah linked list masih kosong
        {
            *head = nodeBaru;
            *tail = nodeBaru;
            return;
        }
        (*tail)->next = nodeBaru;// Menyesuaikan pointer next pada node terakhir ke nodeBaru
        *tail = nodeBaru;// Menjadikan nodeBaru sebagai elemen terakhir
    }
    else
    {// Jika linked list masih kosong, nodeBaru menjadi elemen pertama dan terakhir
        nodeBaru->next = NULL;
        nodeBaru->prev = NULL;
        *head = nodeBaru;
        *tail = nodeBaru;
    }
    Tulis_gudang(*head); // Menulis perubahan linked list Gudang ke file
    cout << "============================================" << endl;
    cout << "|     Barang Berhasil Ditambah DiAkhir     |" << endl;
    cout << "============================================";
    getch();
}

// Fungsi untuk mengupdate data barang di gudang
void update(Gudang **head, Gudang **tail)
{
    int No;
    bool ulang = true;
    Gudang *temp = *head;
    if (*head == NULL && *tail == NULL)
    {
        cout << "==========================================" << endl;
        cout << "|       Daftar Barang Masih Kosong       |" << endl;
        cout << "==========================================" << endl;
        cout << "\nSilahkan Tambah Barang terlebih dahulu!";
        getch();
    }
    else
    {
        display_gudang(*head);
        cout << "\n===========================================" << endl;
        cout << "|           Update Data Barang            |" << endl;
        cout << "===========================================" << endl;
        cout << "Pilih Data Barang Yang Mau Diupdate : ";
        cin >> No;
        if (No < 1 || No > Jumlah_Data())
        {
            cout << "\n| Nomor yang anda pilih tidak sesuai! |";
            getch();
        }
        else if (No > 0 && No <= Jumlah_Data())
        {
            cur = *head;
            for (int i = 1; i < No; i++)
            {
                cur = cur->next;
            }
            system("cls");
            cout << "| ID Barang         > " << cur->id_barang << endl;
            cout << "| Nama Barang       > " << cur->nama_barang << endl;
            cout << "| Jenis Barang      > " << cur->jenis_barang << endl;
            cout << "| Stock Barang      > " << cur->stock_barang << endl;
            cout << "| Harga Barang      > " << cur->harga_barang << endl;
            cout << "| Tanggal Masuk     > " << cur->tanggal_masuk << endl;
            cout << "| Expired Barang    > " << cur->Expired << endl;
            cout << "\n===============================================================" << endl;
            cout << "|                       Update Barang Terbaru                  |" << endl;
            cout << "================================================================" << endl;
            while (ulang)
            {
                cout << "| Masukkan ID Barang                   > ";
                cin >> cur->id_barang;
                if (cur->id_barang > 0)
                {
                    int cek = 1;
                    while (temp != NULL)
                    {
                        if (temp->id_barang == cur->id_barang)
                        {
                            if (cek == No)
                            {
                                ulang = false;
                            }
                            else
                            {
                                cout << "\nID Barang Telah Digunakan, Silahkan Gunakan ID lain!" << endl;
                            }
                            break;
                        }
                        else
                        {
                            temp = temp->next;
                            cek += 1;
                        }
                        if (temp == NULL)
                        {
                            ulang = false;
                        }
                    }
                }
                else
                {
                    cin.clear();
                    cin.ignore(100, '\n');
                    cout << "\nInputan Anda Salah, Ikuti sesuai perintah!" << endl;
                }
            }
            while (true)
            {
                cout << "| Masukkan Nama Barang                 > ";
                fflush(stdin);
                getline(cin, cur->nama_barang);
                if (cur->nama_barang == "")
                {
                    cout << "\nNama Barang tidak boleh kosong!\n"
                         << endl;
                }
                else
                {
                    break;
                }
            }
            while (true)
            {
                cout << "| Masukkan Jenis Barang                > ";
                fflush(stdin);
                getline(cin, cur->jenis_barang);
                if (cur->jenis_barang == "")
                {
                    cout << "\nJenis Barang tidak boleh kosong!\n"
                         << endl;
                }
                else
                {
                    break;
                }
            }
            while (true)
            {
                cout << "| Masukkan Stock Barang                > ";
                cin >> cur->stock_barang;
                if (cur->stock_barang > 0)
                {
                    break;
                }
                else
                {
                    cin.clear();
                    cin.ignore(100, '\n');
                    cout << "\nInputan anda salah, lakukan sesuai perintah!" << endl;
                }
            }
            while (true)
            {
                cout << "| Masukkan Harga Barang                > ";
                cin >> cur->harga_barang;
                if (cur->harga_barang > 0)
                {
                    break;
                }
                else
                {
                    cin.clear();
                    cin.ignore(100, '\n');
                    cout << "\nInputan anda salah, lakukan sesuai perintah!" << endl;
                }
            }
            while (true)
            {
                cout << "| Masukkan Tanggal Masuk(DD/MM/YYYY)   > ";
                fflush(stdin);
                getline(cin, cur->tanggal_masuk);
                if (cur->tanggal_masuk == "")
                {
                    cout << "\nTanggal Masuk tidak boleh kosong!\n"
                         << endl;
                }
                else
                {
                    break;
                }
            }
            while (true)
            {
                cout << "| Masukkan Expired Barang(DD/MM/YYYY)  > ";
                fflush(stdin);
                getline(cin, cur->Expired);
                if (cur->Expired == "")
                {
                    cout << "\nExpired Barang tidak boleh kosong!\n"
                         << endl;
                }
                else
                {
                    break;
                }
            }
            system("cls");
            cout << "\n========================================" << endl;
            cout << "|    Data Barang Berhasil Diupdate       |" << endl;
            cout << "==========================================" << endl;
            cout << "| ID Barang         > " << cur->id_barang << endl;
            cout << "| Nama Barang       > " << cur->nama_barang << endl;
            cout << "| Jenis Barang      > " << cur->jenis_barang << endl;
            cout << "| Stock Barang      > " << cur->stock_barang << endl;
            cout << "| Harga Barang      > " << cur->harga_barang << endl;
            cout << "| Tanggal Masuk     > " << cur->tanggal_masuk << endl;
            cout << "| Expired Barang    > " << cur->Expired << endl;
            cout << "========================================" << endl;
            Tulis_gudang(*head);
            cout << "\nTekan ENTER Untuk Lanjut!";
            getch();
        }
        else
        {
            cin.clear();
            cin.ignore(100, '\n');
            cout << "\nInputan Anda Salah, Lakukan Sesuai Perintah!";
            getch();
        }
    }
}

// Fungsi untuk menghapus barang dari gudang
void delete_gudang(Gudang **head, Gudang **tail)
{
    int No;
    if (*head == NULL && *tail == NULL)
    {
        cout << "\n=========================================" << endl;
        cout << "|          Daftar Barang Kosong         |" << endl;
        cout << "\n=========================================" << endl;
    }
    else
    {
        Gudang *temp = *head;
        display_gudang(temp);
        cout << "\n\n=================================================" << endl;
        cout << "| Masukkan Nomor Barang Yang Mau Dihapus : ";
        cin >> No;
        if (No < 0 || No > Jumlah_Data())
        {
            cout << "\nNomor yang anda pilih tidak sesuai!";
            getch();
        }
        else if (No > 0 && No <= Jumlah_Data())
        {
            if (No == 1 && No == Jumlah_Data())
            {
                *head = NULL;
                *tail = NULL;
            }
            else if (No == 1)
            {
                *head = (*head)->next;
                (*head)->prev = NULL;
            }
            else if (No == Jumlah_Data())
            {
                *tail = (*tail)->prev;
                (*tail)->next = NULL;
            }
            else
            {
                cur = *head;
                for (int i = 1; i < No; i++)
                {
                    cur = cur->next;
                }
                cur->prev->next = cur->next;
                cur->next->prev = cur->prev;
            }
            Tulis_gudang(*head);
            cout << "\n=========================================" << endl;
            cout << "| Berhasil Hapus Barang Yang Diinginkan |" << endl;
            cout << "=========================================" << endl;
            cout << "\nTekan ENTER Untuk Lanjut!";
            getch();
        }
        else
        {
            cin.clear();
            cin.ignore(100, '\n');
            cout << "\nInputan Anda Salah, Lakukan Sesuai Perintah!";
            getch();
        }
    }
}

// Sorting
// QuickSort ascending

// Fungsi untuk mendapatkan ekor (tail) dari linked list
struct Gudang *getTailasc(struct Gudang *cur)
{
    while (cur != NULL && cur->next != NULL)
    {
        cur = cur->next;
    }
    return cur;
}

// Fungsi untuk membagi linked list untuk quicksort ascending
struct Gudang *partitionasc(struct Gudang *head, struct Gudang *end, struct Gudang **newHead, struct Gudang **newEnd)
{
    struct Gudang *pivot = end;
    struct Gudang *prev = NULL, *cur = head, *tail = pivot;
    while (cur != pivot)
    {
        if (cur->id_barang < pivot->id_barang)
        {
            if ((*newHead) == NULL)
            {
                (*newHead) = cur;
            }
            prev = cur;
            cur = cur->next;
        }
        else
        {
            if (prev)
            {
                prev->next = cur->next;
            }
            struct Gudang *tmp = cur->next;
            cur->next = NULL;
            tail->next = cur;
            tail = cur;
            cur = tmp;
        }
    }
    if ((*newHead) == NULL)
    {
        (*newHead) = pivot;
    }
    (*newEnd) = tail;
    return pivot;
}

// Fungsi rekursif untuk melakukan quicksort ascending
struct Gudang *quickSortRecurasc(struct Gudang *head, struct Gudang *end)
{
    if (!head || head == end)
    {
        return head;
    }
    Gudang *newHead = NULL, *newEnd = NULL;
    struct Gudang *pivot = partitionasc(head, end, &newHead, &newEnd);
    if (newHead != pivot)
    {
        struct Gudang *tmp = newHead;
        while (tmp->next != pivot)
        {
            tmp = tmp->next;
        }
        tmp->next = NULL;
        newHead = quickSortRecurasc(newHead, tmp);
        tmp = getTailasc(newHead);
        tmp->next = pivot;
    }
    pivot->next = quickSortRecurasc(pivot->next, newEnd);
    return newHead;
}

// Fungsi utama untuk melakukan quicksort ascending
void quickSortasc(struct Gudang **headRef)
{
    (*headRef) = quickSortRecurasc(*headRef, getTailasc(*headRef));
    return;
}

// QuickSort descending
// Fungsi untuk mendapatkan ekor (tail) dari linked lis
struct Gudang *getTaildesc(struct Gudang *cur)
{
    while (cur != NULL && cur->next != NULL)
    {
        cur = cur->next;
    }
    return cur;
}

// Fungsi untuk membagi linked list untuk quicksort descending
struct Gudang *partitiondesc(struct Gudang *head, struct Gudang *end, struct Gudang **newHead, struct Gudang **newEnd)
{
    struct Gudang *pivot = end;
    struct Gudang *prev = NULL, *cur = head, *tail = pivot;
    while (cur != pivot)
    {
        if (cur->id_barang > pivot->id_barang)
        {
            if ((*newHead) == NULL)
            {
                (*newHead) = cur;
            }
            prev = cur;
            cur = cur->next;
        }
        else
        {
            if (prev)
            {
                prev->next = cur->next;
            }
            struct Gudang *tmp = cur->next;
            cur->next = NULL;
            tail->next = cur;
            tail = cur;
            cur = tmp;
        }
    }
    if ((*newHead) == NULL)
    {
        (*newHead) = pivot;
    }
    (*newEnd) = tail;
    return pivot;
}

// Fungsi rekursif untuk melakukan quicksort descending
struct Gudang *quickSortRecurdesc(struct Gudang *head, struct Gudang *end)
{
    if (!head || head == end)
    {
        return head;
    }
    Gudang *newHead = NULL, *newEnd = NULL;
    struct Gudang *pivot = partitiondesc(head, end, &newHead, &newEnd);
    if (newHead != pivot)
    {
        struct Gudang *tmp = newHead;
        while (tmp->next != pivot)
        {
            tmp = tmp->next;
        }
        tmp->next = NULL;
        newHead = quickSortRecurdesc(newHead, tmp);
        tmp = getTaildesc(newHead);
        tmp->next = pivot;
    }
    pivot->next = quickSortRecurdesc(pivot->next, newEnd);
    return newHead;
}

// Fungsi utama untuk melakukan quicksort descending
void quickSortdesc(struct Gudang **headRef)
{
    (*headRef) = quickSortRecurdesc(*headRef, getTaildesc(*headRef));
    return;
}

// ===============================================================
// Searching
// Boyer-Moore

// Konstanta untuk jumlah karakter ASCII
const int NO_OF_CHARS = 256;

// Fungsi untuk menginisialisasi heuristik karakter yang buruk pada algoritma Boyer-Moore
void badCharHeuristic(string str, int size, int badChar[NO_OF_CHARS])
{
    for (int i = 0; i < NO_OF_CHARS; i++)
    {
        badChar[i] = -1;
    }

    for (int i = 0; i < size; i++)
    {
        badChar[(int)str[i]] = i;
    }
}

// Algoritma Boyer-Moore untuk mencari data berdasarkan nama barang
void BoyerMoore(Gudang *head)
{
    string nama;
    cout << "Masukkan Nama yang dicari : ";
    cin >> nama;

    // int m disini berguna untuk mencari berdasarkan panjang ukuran dari nama yang dicari
    int m = nama.size();
    // *temp menyimpan data yang ada pada struct gudang ke dalam head
    Gudang *temp = head;
    int getdata = 0;

    while (temp != NULL)
    {
        // untuk mencari bad char agar bisa menyesuaikan berdasarkan character yang sesuai pada pencarian
        int n = temp->nama_barang.size();
        int badChar[NO_OF_CHARS];
        badCharHeuristic(nama, m, badChar);
        int s = 0;

        while (s <= (n - m))
        {
            int j = m - 1;

            while (j >= 0 && nama[j] == temp->nama_barang[s + j])
            {
                j--;
            }

            if (j < 0)
            {
                // Data ditemukan, cetak informasi
                cout << "\n========================================" << endl;
                cout << "|              Data Barang             |" << endl;
                cout << "========================================" << endl;
                cout << "| ID Barang         > " << temp->id_barang << endl;
                cout << "| Nama Barang       > " << temp->nama_barang << endl;
                cout << "| Jenis Barang      > " << temp->jenis_barang << endl;
                cout << "| Stock Barang      > " << temp->stock_barang << endl;
                cout << "| Harga Barang      > " << temp->harga_barang << endl;
                cout << "| Tanggal Masuk     > " << temp->tanggal_masuk << endl;
                cout << "| Expired Barang    > " << temp->Expired << endl;
                cout << "========================================" << endl;
                getdata++;
                getch();
                break;
            }
            else
            {
                s += max(1, j - badChar[temp->nama_barang[s + j]]);
            }
        }

        temp = temp->next;
    }
    // jika jumlah data 0 (kosong) dan belum ditambahkan maka data tidak ditemukan
    if (getdata == 0)
    {
        cout << "Data tidak ditemukan!!" << endl;
    }
}

// Fungsi untuk menghitung panjang linked list pada struct gudang
int length(Gudang *head)
{
    int panjang = 0;
    Gudang *temp = head;
    while (temp != NULL)
    {
        temp = temp->next;
        panjang++;
    }
    return panjang;
}

// Fungsi untuk menemukan node di indeks tertentu pada linked list
Gudang *findNode(Gudang *head, int idx)
{
    for (int a = 0; a < idx; a++)
    {
        head = head->next;
    }
    return head;
}

int min(int x, int y) { return (x <= y) ? x : y; }

// Fungsi untuk melakukan pencarian menggunakan algoritma Fibonacci
void fibonachiSearch(Gudang *head)
{
    int x;
    cout << "Masukkan ID yang dicari : ";
    cin >> x;

    int n = length(head); // Implement length function to find the length of the linked list

    Gudang *temp = head;
    int fibMMm2 = 0;
    int fibMMm1 = 1;
    int fibM = fibMMm2 + fibMMm1;

    while (fibM < n)
    {
        fibMMm2 = fibMMm1;
        fibMMm1 = fibM;
        fibM = fibMMm2 + fibMMm1;
    }

    int offset = -1;

    while (fibM > 1)
    {
        int i = min(offset + fibMMm2, n - 1);

        temp = head;
        for (int j = 0; j < i; j++)
        {
            temp = temp->next;
        }

        if (temp->id_barang > x)
        {
            fibM = fibMMm1;
            fibMMm1 = fibMMm2;
            fibMMm2 = fibM - fibMMm1;
            offset = i;
        }
        else if (temp->id_barang < x)
        {
            fibM = fibMMm2;
            fibMMm1 = fibMMm1 - fibMMm2;
            fibMMm2 = fibM - fibMMm1;
        }
        else
        {
            Gudang *foundNode = findNode(head, i);
            cout << "\n========================================" << endl;
            cout << "|              Data Barang             |" << endl;
            cout << "========================================" << endl;
            cout << "| ID Barang         > " << foundNode->id_barang << endl;
            cout << "| Nama Barang       > " << foundNode->nama_barang << endl;
            cout << "| Jenis Barang      > " << foundNode->jenis_barang << endl;
            cout << "| Stock Barang      > " << foundNode->stock_barang << endl;
            cout << "| Harga Barang      > " << foundNode->harga_barang << endl;
            cout << "| Tanggal Masuk     > " << foundNode->tanggal_masuk << endl;
            cout << "| Expired Barang    > " << foundNode->Expired << endl;
            cout << "========================================" << endl;
            getch();
            break;
        }
    }

    temp = head;
    for (int j = 0; j < offset + 1; j++)
    {
        temp = temp->next;
    }
}

// =============================================================
// Menu Search Data Barang
void Cari_Gudang()
{
    int pilih;
    while (true)
    {
        system("cls");
        cout << "============================================" << endl;
        cout << "|            Data Gudang Search            |" << endl;
        cout << "============================================" << endl;
        cout << "| <1> Cari Barang Berdasarkan Nama Barang  |" << endl;
        cout << "| <2> Cari Barang berdasarkan ID Barang    |" << endl;
        cout << "| <3> Kembali                              |" << endl;
        cout << "============================================" << endl;
        cout << "| Masukkan Pilihan Anda > ";
        try
        {

            if (!(cin >> pilih))
            {
                cin.clear();
                cin.ignore();
                fflush(stdin);
                cout << "input salah\n";
                getch();
            }
            else
            {
                throw pilih;
            }
        }
        catch (int pilih)
        {
            if (pilih == 1)
            {
                BoyerMoore(head);
            }
            else if (pilih == 2)
            {
                fibonachiSearch(head);
            }
            else if (pilih == 3)
            {
                break;
            }
            else
            {
                cout << "Pilihan Tidak Ditemukan!" << endl;
                getch();
            }
        }
    }
}

// Fungsi untuk menampilkan menu sorting gudang
void Pilihsort_gudang()
{
    int No;
    while (true)
    {
        system("cls");
        cout << "==========================================" << endl;
        cout << "|          Display Barang Secara         |" << endl;
        cout << "==========================================" << endl;
        cout << "| <1> Ascending (ID_Barang)              |" << endl;
        cout << "| <2> Descending (ID_Barang)             |" << endl;
        cout << "| <3> Kembali                            |" << endl;
        cout << "==========================================" << endl;
        cout << "| Masukkan Pilihan Anda > ";
        try
        {
            if (!(cin >> No))
            {
                cin.clear();
                cin.ignore();
                fflush(stdin);
                cout << "input salah\n";
                getch();
            }
            else
            {
                throw No;
            }
        }
        catch (int No)
        {
            if (No == 1)
            {
                quickSortasc(&head);
                display_gudang(head);
            }
            else if (No == 2)
            {
                quickSortdesc(&head);
                display_gudang(head);
            }
            else if (No == 3)
            {
                menugudang();
            }
            else
            {
                cout << "Pilihan Tidak Ditemukan!" << endl;
                getch();
            }
        }
    }
}

// Pesanan

// Fungsi untuk menulis data keranjang ke file CSV
void Tulis_keranjang(Keranjang *head)
{
    fstream file;
    cur_keranjang = head;
    // membuka file untuk menulis data ke file
    file.open("./A2_Kelompok4/keranjang.csv", ios::trunc | ios::out | ios::in);
    if (file.is_open())
    {
        // traversal
        while (cur_keranjang != NULL)
        {
            // Menuliskan data ke file
            file << cur_keranjang->username << ",";
            file << cur_keranjang->id_pembeli << ",";
            file << cur_keranjang->nama_barang << ",";
            file << cur_keranjang->jenis_barang << ",";
            file << cur_keranjang->harga_barang << ",";
            file << cur_keranjang->jumlah << ",";
            file << cur_keranjang->total << endl;
            cur_keranjang = cur_keranjang->next;
        }
    }
    // menutup file
    file.close();
}

// Fungsi untuk membaca data keranjang dari file CSV
void Baca_keranjang(Keranjang **head)
{
    ifstream file;
    string isi, username, id_pembeli, nama, nama_barang, jumlah, total, jenis_barang, harga;
    int id_int, jumlah_int, total_int, harga_int;
    // membuka file untuk membaca file ke program
    file.open("./A2_Kelompok4/keranjang.csv", ios::in);
    if (file.is_open())
    {
        // proses membaca file
        while (getline(file, isi))
        {
            cur_keranjang = *head;
            Keranjang *newNode = new Keranjang;
            stringstream ss(isi);

            // mengubah isi data pada file menjadi string semua
            getline(ss, id_pembeli, ',');
            getline(ss, username, ',');
            getline(ss, nama, ',');
            getline(ss, nama_barang, ',');
            getline(ss, jenis_barang, ',');
            getline(ss, harga, ',');
            getline(ss, jumlah, ',');
            getline(ss, total, '\n');

            stringstream id_string(id_pembeli);
            stringstream jumlah_string(jumlah);
            stringstream total_string(total);
            stringstream harga_string(harga);

            // mengembalikkan nilai yang sebelumnya string menjadi integer kembali
            id_string >> id_int;
            jumlah_string >> jumlah_int;
            total_string >> total_int;
            harga_string >> harga_int;

            // memasukkan data kedalam linked list
            newNode->id_pembeli = id_int;
            newNode->username = username;
            newNode->nama_barang = nama_barang;
            newNode->jenis_barang = jenis_barang;
            newNode->harga_barang = harga_int;
            newNode->jumlah = jumlah_int;
            newNode->total = total_int;
            if (*head == NULL)
            {
                *head = newNode;
            }
            else
            {
                while (cur_keranjang->next != NULL)
                {
                    cur_keranjang = cur_keranjang->next;
                }
                cur_keranjang->next = newNode;
            }
        }
    }
    // menutup file
    file.close();
}


// Fungsi untuk memeriksa apakah pengguna sudah memiliki keranjang belanja
// Input: username (string) - Nama pengguna yang ingin diperiksa keranjangnya
//        head (Keranjang*) - Pointer ke awal linked list Keranjang
// Output: 1 - Pengguna memiliki keranjang belanja
//         -1 - Pengguna tidak memiliki keranjang belanja
int cek_keranjang(string username, Keranjang *head)
{
    cur_keranjang = head; // Inisialisasi pointer cur_keranjang untuk traversal
    while (cur_keranjang != NULL) // Iterasi melalui linked list Keranjang /Perulangan
    {
        if (cur_keranjang->username == username) // Memeriksa apakah nama pengguna pada node saat ini sesuai dengan username yang dicari
        {
            return 1;
        }
        else
        {
            cur_keranjang = cur_keranjang->next;// Pindah ke node berikutnya jika nama pengguna tidak sesuai
        }
    }
    return -1;
}

// Fungsi untuk menampilkan isi keranjang pengguna
void display_keranjang(string username, Keranjang *head)
{
    cur_keranjang = head;
    system("cls");
    // UNTUK MENGECEK APAKAH LINKED LIST NYA SUDAH ADA ISINYA ATAU BELUM
    if (cek_keranjang(username, head) == -1)
    {
        cout << "===================================================" << endl;
        cout << "              DAFTAR KERANJANG KOSONG              " << endl;
        cout << "===================================================" << endl;
    }
    else
    {
        string baris(73, '=');
        cout << baris << endl;
        cout << "|                     Daftar Keranjang Pesanan Anda                     |" << endl;
        cout << baris << endl;
        cout << "| " << setiosflags(ios::left) << setw(5) << "No"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(9) << "ID Pembeli"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(20) << "Nama Barang"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(15) << "Jumlah Pesanan"
             << "|";
        cout << " " << setiosflags(ios::left) << setw(12) << "Total Harga"
             << "|" << endl;
        cout << baris << endl;
        // VARIABEL i DIGUNAKAN UNTUK MENAMPILKAN URUTAN INDEKS NYA
        int i = 1;
        while (cur_keranjang != NULL)
        {
            if (cur_keranjang->username == username)
            {
                cout << resetiosflags(ios::adjustfield);
                cout << "| [" << i << setiosflags(ios::left) << setw(3) << "]"
                     << "|";
                cout << " " << setiosflags(ios::left) << setw(10) << cur_keranjang->id_pembeli << "|";
                cout << " " << setiosflags(ios::left) << setw(20) << cur_keranjang->nama_barang << "|";
                cout << " " << setiosflags(ios::left) << setw(15) << cur_keranjang->jumlah << "|";
                cout << " Rp. " << setiosflags(ios::left) << setw(8) << cur_keranjang->total << "|";
                cout << endl;
                i++;
            }
            cur_keranjang = cur_keranjang->next;
        }
        cout << resetiosflags(ios::adjustfield);
        cout << baris << endl;
    }
}

/// Fungsi untuk menambahkan barang ke keranjang belanja pengguna
// Input: username (string) - Nama pengguna yang akan menambahkan barang ke keranjang
//        HEAD (Gudang*) - Pointer ke awal linked list Gudang yang berisi data barang
//        head (Keranjang**) - Pointer ke awal linked list Keranjang yang berisi data keranjang belanja
//        head_check (checkout**) - Pointer ke awal linked list checkout yang berisi data checkout
// Output: Menambahkan barang ke keranjang belanja pengguna dan memberikan opsi untuk checkout
void tambah_keranjang(string username, Gudang *HEAD, Keranjang **head, checkout **head_check)
{// Deklarasi variabel-variabel yang diperlukan
    string beli;
    int pilih, jumlah, total;
    // Menampilkan data gudang untuk dipilih oleh pengguna
    display_gudang(HEAD);
     // Inisialisasi pointer dan variabel untuk traversal dan input pengguna
    Gudang *temp = HEAD;
    cur_check = *head_check;

    cout << "\n===============================================" << endl;
    cout << "|           Form Tambah Pesanan Barang        |" << endl;
    cout << "===============================================" << endl;

    cout << "\nMasukkan Nomor Barang Yang Ingin Dipesan: ";
    cin >> pilih;
    // Pengecekan apakah nomor barang valid
    if (pilih > 0 && pilih <= Jumlah_Data())
    {
        // Melakukan ulang untuk menampilkan data gudang dan Traversal ke node gudang yang sesuai dengan nomor yang diinputkan
        for (int i = 1; i < pilih; i++)
        {
            temp = temp->next;
        }

        if (temp->stock_barang < 1)  // Pengecekan stok barang di gudang
        {
            cout << "\nStok Barang Masih Kosong...";
            getch();
        }
        else
        {
            system("cls");

            cout << "\n===============================================" << endl;
            cout << "|            Form Tambah Pesanan Barang       |" << endl;
            cout << "===============================================" << endl;
            cout << "ID Barang            : " << temp->id_barang << endl;
            cout << "Nama Barang          : " << temp->nama_barang << endl;
            cout << "Jenis Barang         : " << temp->jenis_barang << endl;
            cout << "Harga                : Rp" << temp->harga_barang << endl;
            cout << "Jumlah Stok          : " << temp->stock_barang << endl;
            cout << "-----------------------------------------------" << endl;
            // Input jumlah barang yang ingin dibeli oleh pengguna
            while (true) //Perulangan
            {
                cout << "\nIngin Pesan Berapa Banyak Barang: ";
                cin >> jumlah;

                if (jumlah > 0 && jumlah <= temp->stock_barang)
                {
                    break;
                }
                else if (jumlah > temp->stock_barang)
                {
                    cout << "\nStok Data Barang Tidak Mencukupi" << endl;
                }
                else if (jumlah < 1 && jumlah > 0)
                {
                    cout << "\nMinimal Pesan Satu Barang" << endl;
                }
                else
                {
                    cin.clear();
                    cin.ignore(100, '\n');
                    cout << "\nInputan Anda Salah, Silahkan Coba Lagi!" << endl;
                }
            }
            total = temp->harga_barang * jumlah; // Menghitung total harga
            // Menambahkan barang ke keranjang pengguna
            Keranjang *nodeBaru = new Keranjang;
            nodeBaru->username = username;
            nodeBaru->id_pembeli = temp->id_barang;
            nodeBaru->nama_barang = temp->nama_barang;
            nodeBaru->jenis_barang = temp->jenis_barang;
            nodeBaru->harga_barang = temp->harga_barang;
            nodeBaru->jumlah = jumlah;
            nodeBaru->total = total;

            while (true) // Loop untuk menampilkan informasi barang yang ditambahkan ke keranjang
            {
                system("cls");
                // Menampilkan informasi barang yang ditambahkan ke keranjang
                cout << "===============================================" << endl;
                cout << "|        Berhasil Menambah Ke Keranjang!      |" << endl;
                cout << "===============================================" << endl;
                cout << "Nama Pembeli      : " << nodeBaru->username << endl;
                cout << "Nama Barang       : " << nodeBaru->nama_barang << endl;
                cout << "Jenis Barang      : " << nodeBaru->jenis_barang << endl;
                cout << "Harga Barang      : Rp" << nodeBaru->harga_barang << endl;
                cout << "Jumlah Pesanan    : " << nodeBaru->jumlah << endl;
                cout << "Total Harga       : Rp" << nodeBaru->total << endl;
                cout << "===================================================" << endl;
                // Menampilkan pilihan untuk checkout atau kembali
                cout << "\n  [1] Langsung Checkout " << endl;
                cout << "  [2] Kembali " << endl;
                cout << "===================================================" << endl;
                cout << "Masukkan Pilihan: ";
                cin >> beli;

                // Jika pengguna memilih untuk checkout
                if (beli == "1")
                {
                    // Menambahkan data checkout ke linked list checkout
                    checkout *newNode = new checkout;
                    newNode->username = username;
                    newNode->id_pembeli = temp->id_barang;
                    newNode->nama_barang = nodeBaru->nama_barang;
                    newNode->jenis_barang = nodeBaru->jenis_barang;
                    newNode->harga_barang = nodeBaru->harga_barang;
                    newNode->jumlah = nodeBaru->jumlah;
                    newNode->total = nodeBaru->total;
                    newNode->status = "Menunggu";
                     // Pengecekan apakah linked list checkout kosong atau tidak
                    if (*head_check == NULL)
                    {
                        *head_check = newNode;
                    }
                    else
                    {
                        while (cur_check->next != NULL)
                        {
                            cur_check = cur_check->next;
                        }
                        cur_check->next = newNode;
                    }

                    // Menulis data checkout ke file
                    Tulis_queue(*head_check);
                    system("cls");
                    // Menampilkan informasi pesanan yang di-checkout
                    cout << "\n===============================================" << endl;
                    cout << "|      Berhasil Checkout Pesanan!    |" << endl;
                    cout << "===============================================" << endl;
                    cout << "ID Pembeli        : " << newNode->id_pembeli << endl;
                    cout << "Pesanan Atas Nama : " << newNode->username << endl;
                    cout << "Nama Barang       : " << newNode->nama_barang << endl;
                    cout << "Jenis Barang      : " << newNode->jenis_barang << endl;
                    cout << "Harga Barang      : Rp" << newNode->harga_barang << endl;
                    cout << "Jumlah Pesanan    : " << newNode->jumlah << endl;
                    cout << "Total Harga       : Rp" << newNode->total << endl;
                    cout << "Status Pesanan    : " << newNode->status << endl;
                    cout << "==================================================" << endl;
                    cout << "\nTekan ENTER Untuk Melanjutkan...";
                    getch();
                    break;
                }

                // Jika pengguna memilih untuk kembali
                else if (beli == "2")
                {
                    // Menambahkan data ke linked list keranjang
                    nodeBaru->next = *head;
                    *head = nodeBaru;
                    // Menulis data keranjang ke file
                    Tulis_keranjang(*head);
                    break;
                }
                else
                {
                    // Jika pilihan tidak valid
                    cout << "\nPilihan Menu Tidak Ditemukan" << endl;
                    getch();
                }
            }
        }
    }
    else if (pilih < 0 || pilih > Jumlah_Data())
    {
        cout << "\nNomor Yang Anda Masukkan Diluar Range Data Barang!" << endl;
        cout << "\nTekan ENTER Untuk Melanjutkan...";
        getch();
    }
    else
    {// Eror Handling
        cin.clear();
        cin.ignore(100, '\n');
        cout << "\nInputan Anda Salah!";
        getch();
    }
}

// Fungsi untuk mengupdate jumlah pesanan di keranjang pengguna
void update_keranjang(string username, Keranjang **head, Gudang **Gd)
{
    Keranjang *temp = *head;
    Gudang *cek = *Gd;
    int no, harga, jumlah;
    system("cls");
    if (cek_keranjang(username, *head) == -1)
    {
        cout << "===================================================" << endl;
        cout << "              DAFTAR KERANJANG KOSONG            " << endl;
        cout << "===================================================" << endl;
        cout << "\nSilahkan tambah pesanan terlebih dahulu...";
        getch();
    }
    else
    {
        // Menampilkan keranjang pengguna
        display_keranjang(username, *head);

        // Meminta pengguna memasukkan nomor data yang ingin diupdate
        cout << "\nMasukkan Nomor Data Yang Ingin Diupdate : ";
        cin >> no;

        // Memeriksa kevalidan nomor data yang dimasukkan
        if (no < 0 || no > hitung_keranjang(username, head))
        {
            cout << "\nNomor Yang Anda Masukkan Tidak Sesuai!";
            getch();
        }
        // Jika nomor valid
        else if (no > 0 && no <= hitung_keranjang(username, head))
        {
            for (int i = 1; i < no; i++)
            {
                temp = temp->next;
            }
            while (cek->id_barang != temp->id_pembeli)
            {
                cek = cek->next;
            }
            while (true)
            {
                system("cls");
                cout << "Nomor [" << no << "]: " << endl;
                cout << "ID pembeli      : " << temp->id_pembeli << endl;
                cout << "Nama Barang     : " << temp->nama_barang << endl;
                cout << "Jumlah Pesanan : " << temp->jumlah << endl;
                cout << "Total Harga    : Rp" << temp->total << endl;
                harga = temp->total / temp->jumlah;
                cout << "_______________________________________________\n"
                     << endl;
                cout << "===============================================" << endl;
                cout << "    Ubah Data Keranjang Menjadi     " << endl;
                cout << "===============================================" << endl;
                cout << "Jumlah Pesanan : ";
                cin >> jumlah;
                if (jumlah > 0)
                {
                    if (jumlah <= cek->stock_barang)
                    {
                        // Mengupdate jumlah pesanan di keranjang
                        temp->jumlah = jumlah;
                        temp->total = harga * temp->jumlah;
                        // Menulis data keranjang ke file
                        Tulis_keranjang(*head);
                        system("cls");
                        // Menampilkan informasi setelah update
                        cout << "\n===============================================" << endl;
                        cout << "  Berhasil Update Data Keranjang!   " << endl;
                        cout << "===============================================" << endl;
                        cout << "Nomor [" << no << "]: " << endl;
                        cout << "ID Pembeli       : " << temp->id_pembeli << endl;
                        cout << "Nama Barang    : " << temp->nama_barang << endl;
                        cout << "Jumlah Pesanan : " << temp->jumlah << endl;
                        cout << "Total Harga    : Rp" << temp->total << endl;
                        cout << "_______________________________________________" << endl;
                        cout << "\nTekan ENTER Untuk Melanjutkan...";
                        getch();
                    }
                    else
                    {
                        cout << "\nJumlah Stok Barang Tidak Mencukupi!";
                        getch();
                    }
                    break;
                }
                // Jika nomor tidak valid
                else
                {
                    cin.clear();
                    cin.ignore(100, '\n');
                    cout << "\nInputan Anda Salah, Silahkan Coba Lagi!";
                    getch();
                }
            }
        }
        else
        {
            cin.clear();
            cin.ignore(100, '\n');
            cout << "\nInputan Anda Salah!";
            getch();
        }
    }
}

// Fungsi untuk menghapus barang dari keranjang pengguna
void hapus_keranjang(string username, Keranjang **head)
{
    Keranjang *temp = *head;
    Keranjang *temp_hapus = *head;
    int no, tanda = 0;
    system("cls");
    if (cek_keranjang(username, *head) == -1)
    {
        cout << "===================================================" << endl;
        cout << "            DAFTAR KERANJANG KOSONG            " << endl;
        cout << "===================================================" << endl;
        cout << "\nSilahkan tambah pesanan terlebih dahulu...";
        getch();
    }
    else
    {
        // Menampilkan keranjang pengguna
        display_keranjang(username, *head);
        // Meminta pengguna memasukkan nomor data yang ingin dihapus
        cout << "\nMasukkan Nomor Data Yang Ingin Dihapus : ";
        cin >> no;
        // Memeriksa kevalidan nomor data yang dimasukkan
        if (no < 0 || no > hitung_keranjang(username, head))
        {
            cout << "\nNomor Yang Anda Masukkan Tidak Sesuai!";
            getch();
        }
        // Jika nomor valid
        else if (no > 0 && no <= hitung_keranjang(username, head))
        {
            while (tanda != no)
            {
                if (temp_hapus->username == username)
                {
                    tanda += 1;
                }
                else
                {
                    temp_hapus = temp_hapus->next;
                }
            }
            if (temp_hapus == *head)
            {
                *head = (*head)->next;
            }
            else
            {
                tanda = 0;
                while (tanda != no - 1)
                {
                    if (temp->username == username)
                    {
                        tanda += 1;
                    }
                    else
                    {
                        temp = temp->next;
                    }
                }
                temp->next = temp_hapus->next;
            }
            // Menghapus data dari linked list keranjang
            delete temp_hapus;
            Tulis_keranjang(*head);
            cout << "\n===============================================" << endl;
            cout << "   Berhasil Hapus Data Keranjang!   " << endl;
            cout << "===============================================" << endl;
            cout << "\nTekan ENTER Untuk Melanjutkan...";
            getch();
        }
        else
        {
            cin.clear();
            cin.ignore(100, '\n');
            cout << "\nInputan Anda Salah!";
            getch();
        }
    }
}

// Fungsi untum menampilkan menu utama
void Menu_utama()
{
    // Inisialisasi variabel untuk ulang menu
    bool ulang = true;
    while (ulang) // Perulangan pada menu Utama yang dimana ulang = true akan berjalan terus sampai variabel ulangnya = false.
    {
        system("cls");
        int pilih;
        // Menampilkan menu utama
        cout << "====================================" << endl;
        cout << "|        Welcome to IndoChino      |" << endl;
        cout << "====================================" << endl;
        cout << "| <1> Login                        |" << endl;
        cout << "| <2> Registrasi Akun              |" << endl;
        cout << "| <3> Exit Program                 |" << endl;
        cout << "====================================" << endl;
        cout << "| Masukkan Pilihan Anda > ";
        // Tangani input tidak valid dengan menggunakan try-catch/ Eror Handling
        try
        {
            if (!(cin >> pilih))
            {
                cin.clear();
                cin.ignore();
                fflush(stdin);
                // Menangani input yang salah
                cout << "input salah\n";
                getch();
            }
            else
            {
                // Gunakan throw untuk memproses pilihan
                throw pilih;
            }
        }
        catch (int pilih)
        {
            if (pilih == 1)
            {
                login();
            }
            else if (pilih == 2)
            {
                registrasi(&head_akun, &tail_akun);
            }
            else if (pilih == 3)
            {
                // Menampilkan pesan selamat tinggal
                cout << "\n ____________________________________________" << endl;
                cout << "|                                             |" << endl;
                cout << "|              SELAMAT TINGGAL                |" << endl;
                cout << "|                     &                       |" << endl;
                cout << "|               TERIMA KASIH                  |" << endl;
                cout << "|_____________________________________________|" << endl;
                ulang = false;
                break;
            }
            else
            {
                // Menampilkan pesan kesalahan untuk input yang tidak valid
                cout << "Masukkan Data Dengan benar!" << endl;
                getch();
            }
        }
    }
}

// Fungsi untuk melakukan proses login
void login()
{
    // Inisialisasi variabel dan pointer
    bool ulang = true;
    string username, password, nama;
    int id_pembeli;
    // Perulangan pada fungsi login yang dimana ulang = true akan berjalan terus sampai variabel ulangnya = false.
    while (ulang)
    {
        // Iterasi melalui daftar akun
        Akun *cur_akun = head_akun;
        system("cls");
        cout << "==================================================================" << endl;
        cout << "                             Login                                " << endl;
        cout << "==================================================================" << endl;
        cout << "\n[NOTE: Masukkan angka 0 di username untuk kembali ke menu awal]\n"
             << endl;
        cout << "Masukkan Username : ";
        getline(cin, username);
        // Cek apakah user ingin kembali ke menu utama
        if (username == "0")
        {
            break;
        }
        // Cek apakah username kosong
        if (username.empty())
        {
            cout << "Username tidak boleh kosong. Tekan tombol apapun untuk mencoba lagi...";
            getch();
        }
        else
        {
            cout << "Masukkan Password : ";
            cin >> password;
            // Cek apakah password kosong
            if (password.empty())
            {
                cout << "Password tidak boleh kosong. Tekan tombol apapun untuk mencoba lagi...";
                getch();
            }

            while (cur_akun != NULL)
            {
                // Cek apakah akun valid
                if (cur_akun->username == username && cur_akun->password == password)
                {
                    cout << "\n==================================================================" << endl;
                    cout << "                         Login User Berhasil                           " << endl;
                    cout << "==================================================================" << endl;
                    menupembeli(username, cur_akun->username);
                    ulang = false;
                    break;
                }
                else
                {
                    cur_akun = cur_akun->next;
                }
                // Cek apakah login sebagai admin
                if (username == "admin" && password == "admin")
                {
                    cout << "\n==================================================================" << endl;
                    cout << "                         Login Admin Berhasil                           " << endl;
                    cout << "==================================================================" << endl;
                    menugudang();
                    ulang = false;
                    break;
                }
                // Cek apakah akun tidak ditemukan
                if (cur_akun == NULL)
                {
                    cout << "\n==================================================================" << endl;
                    cout << "         Username Atau Password Yang Anda Masukkan Salah!         " << endl;
                    cout << "==================================================================" << endl;
                    cout << "Tekan tombol apapun untuk mencoba lagi...";
                    getch();
                    break;
                }
            }
        }
    }
}

// Fungsi untuk menampilkan menu tambah barang
void Menu_Tambah()
{
    int no;
    while (true) // Perulangan pada menu tambah data barang
    {
        system("cls");
        cout << "==========================================" << endl;
        cout << "|           Menu Tambah Barang           |" << endl;
        cout << "==========================================" << endl;
        cout << "| <1> Tambah Barang Dipertama            |" << endl;
        cout << "| <2> Tambah Barang Ditengah             |" << endl;
        cout << "| <3> Tambah Barang Diakhir              |" << endl;
        cout << "| <4> Kembali                            |" << endl;
        cout << "==========================================" << endl;
        cout << "| Masukkan Pilihan Anda > ";
        // Tangani input tidak valid dengan menggunakan try-catch/ Eror Handling
        try
        {
            if (!(cin >> no))
            {
                cin.clear();
                cin.ignore();
                fflush(stdin);
                cout << "input salah\n";
                getch();
            }
            else
            {
                throw no;
            }
        }
        catch (int no)
        {
            if (no == 1)
            { // Panggil fungsi pengecekan keranjang
                addfirst(&head, &tail);
            }
            else if (no == 2)
            { // Panggil fungsi pengecekan keranjang
                addMiddle(&head, &tail);
            }
            else if (no == 3)
            { // Panggil fungsi pengecekan keranjang
                addLast(&head, &tail);
            }
            else if (no == 4)
            {
                break; // Kembali kemenu sebelumnya
            }
            else
            {
                cout << "Pilihan Anda Tidak Ada Di menu" << endl;
                getch(); // Menunggu pengguna menekan tombol/ melakukan Pause sementara
            }
        }
    }
}

// Fungsi untuk menampilkan menu pembeli
void menupembeli(string username, string nama)
{
    bool ulang = true;
    int pilihpembeli;
    while (ulang)
    {
        system("cls");
        cout << "==============================================" << endl;
        cout << "|                Menu Pembeli                |" << endl;
        cout << "==============================================" << endl;
        cout << "| <1> Display Data Barang                    |" << endl;
        cout << "| <2> Search Data Barang                     |" << endl;
        cout << "| <3> Create Pembelian Barang                |" << endl;
        cout << "| <4> Display Pembelian                      |" << endl;
        cout << "| <5> Update Pembelian                       |" << endl;
        cout << "| <6> Delete Pembelian                       |" << endl;
        cout << "| <7> Checkout Pembelian                     |" << endl;
        cout << "| <8> Display Checkout                       |" << endl;
        cout << "| <9> Display Riwayat Transaksi              |" << endl;
        cout << "| <0> Exit                                   |" << endl;
        cout << "==============================================" << endl;
        cout << "Masukkan Pilihan anda: ";
        try
        {
            if (!(cin >> pilihpembeli))
            {
                cin.clear();
                cin.ignore();
                fflush(stdin);
                cout << "input salah\n";
                getch(); // Menunggu pengguna menekan tombol/ melakukan Pause sementara
            }
            else
            {
                throw pilihpembeli;
            }
        }
        catch (int pilihpembeli)
        {
            if (pilihpembeli == 1)
            {
                display_gudang(head);
            }
            else if (pilihpembeli == 2)
            { // Melakukan Sorting secara ascending dan Panggil fungsi cari barang di gudang
                quickSortdesc(&head);
                Cari_Gudang();
            }
            else if (pilihpembeli == 3)
            {
                tambah_keranjang(username, head, &head_keranjang, &head_check);
            }
            else if (pilihpembeli == 4)
            {
                if (cek_keranjang(username, head_keranjang) == -1)
                {
                    system("cls");
                    // Tampilkan pesan jika keranjang kosong
                    cout << "===================================================" << endl;
                    cout << "               DAFTAR KERANJANG KOSONG            " << endl;
                    cout << "===================================================" << endl;
                    cout << "\nSilahkan tambah pesanan terlebih dahulu...";
                    getch();
                }
                else
                {
                    // Tampilkan keranjang dan instruksi
                    display_keranjang(username, head_keranjang);
                    cout << "\nTekan ENTER Untuk Melanjutkan...";
                    getch();
                }
            }
            else if (pilihpembeli == 5)
            { // Panggil fungsi menu update barang
                update_keranjang(username, &head_keranjang, &head);
            }
            else if (pilihpembeli == 6)
            { // Panggil fungsi menu hapus data barang
                hapus_keranjang(username, &head_keranjang);
            }
            else if (pilihpembeli == 7)
            { // Panggil fungsi pengecekan keranjang
                if (cek_keranjang(username, head_keranjang) == -1)
                {
                    system("cls");
                    cout << "=======================================" << endl;
                    cout << "|        Daftar CheckOut Kosong       |" << endl;
                    cout << "=======================================" << endl;
                    cout << "\nSilahkan Checkout pesanan anda dulu!" << endl;
                }
                else
                {
                    // Proses checkout
                    enqueue_pesanan(username, &head_keranjang, &head_check);
                }
            }
            else if (pilihpembeli == 8)
            {
                // Tampilkan data checkout
                display_check(username, head_check);
            }
            else if (pilihpembeli == 9)
            {
                // Tampilkan riwayat transaksi pembeli
                display_riwayat_user(username, head_riwayat);
            }
            else if (pilihpembeli == 0)
            {
                cout << "\n=================================================" << endl;
                cout << "                Anda Telah Logout                " << endl;
                cout << "=================================================";
                ulang = false; // Perulangan akan berhenti
            }
            else
            {
                cout << "Data tidak dapat ditemukan" << endl;
                getch(); // Menunggu pengguna menekan tombol/ melakukan Pause sementara
            }
        }
    }
};

// Fungsi untuk menampilkan menu gudang
void menugudang()
{
    bool ulang = true;
    int pilihmenu;
    while (ulang)
    {
        system("cls");
        cout << "==============================================" << endl;
        cout << "|                  Menu Admin                |" << endl;
        cout << "==============================================" << endl;
        cout << "| <1> Add Barang                             |" << endl;
        cout << "| <2> Display Barang                         |" << endl;
        cout << "| <3> Delete Barang                          |" << endl;
        cout << "| <4> Update Barang                          |" << endl;
        cout << "| <5> Search Barang                          |" << endl;
        cout << "| <6> Display Antrian                        |" << endl;
        cout << "| <7> Confirm Pemesanan                      |" << endl;
        cout << "| <8> Display Riwayat                        |" << endl;
        cout << "| <0> Exit                                   |" << endl;
        cout << "==============================================" << endl;
        cout << "| Masukkan pilihan anda: ";
        try
        {
            if (!(cin >> pilihmenu))
            {
                cin.clear();
                cin.ignore();
                fflush(stdin);
                cout << "input salah\n";
                getch(); // Menunggu pengguna menekan tombol/ melakukan Pause sementara
            }
            else
            {
                throw pilihmenu;
            }
        }
        catch (int pilihmenu)
        {
            if (pilihmenu == 1)
            {
                // Panggil fungsi menu tambah barang
                Menu_Tambah();
            }
            else if (pilihmenu == 2)
            {
                // Panggil fungsi pilih sort gudang
                Pilihsort_gudang();
            }
            else if (pilihmenu == 3)
            {
                // Panggil fungsi delete barang dari gudang
                delete_gudang(&head, &tail);
            }
            else if (pilihmenu == 4)
            {
                // Panggil fungsi update data barang dari gudang
                update(&head, &tail);
            }
            else if (pilihmenu == 5)
            {
                // Melakukan Sorting secara ascending dan Panggil fungsi cari barang di gudang
                quickSortdesc(&head);
                Cari_Gudang();
            }
            else if (pilihmenu == 6)
            {
                // Panggil fungsi display antrian checkout
                display_antrian(head_check);
            }
            else if (pilihmenu == 7)
            {
                // Panggil fungsi untuk konfirmasi pesanan pada antrian checkout
                Konfirmasi_antrian(&head_check, &head_riwayat, head);
            }
            else if (pilihmenu == 8)
            {
                // Panggil fungsi  untuk menampilkan riwayat pembelian pengguna
                display_riwayat(head_riwayat);
            }
            else if (pilihmenu == 0)
            {
                cout << "\n=================================================" << endl;
                cout << "                Anda Telah Logout                " << endl;
                cout << "=================================================";
                ulang = false; // Perulangan berhenti
            }
            else
            {
                cout << "Data tidak dapat ditemukan" << endl;
                getch(); // Menunggu pengguna menekan tombol/ melakukan Pause sementara
            }
        }
    }
}

// Awal Program
int main()
{
    Baca_Akun(&head_akun, &tail_akun); // Memuat data akun dari file
    Baca_gudang(&head, &tail);         // Memuat data barang dari file
    Baca_keranjang(&head_keranjang);   // Memuat data keranjang pembeli dari file
    Baca_queue(&head_check);           // Memuat data antrian checkout dari file
    Baca_riwayat(&head_riwayat);       // Memuat data riwayat transaksi dari file
    Menu_utama();                      // Memanggil fungsi untuk menampilkan menu utama
    return 0;                          // Program berakhir
}
