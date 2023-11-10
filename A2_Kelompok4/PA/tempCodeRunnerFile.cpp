#include <iostream>
#include <conio.h>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <iomanip>
#include <bits/stdc++.h>
#include <string>

using namespace std;

struct Gudang
{
    int id_barang;
    string nama_barang;
    string jenis_barang;
    int stock_barang;
    int harga_barang;
    string tanggal_masuk;
    string Expired;
    Gudang *next;
    Gudang *prev;
};

struct Akun
{
    string username;
    string password;
    Akun *next = NULL;
    Akun* prev = NULL; // Menambahkan pointer 'prev' untuk double linked list
};

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
    checkout *next = NULL;
};

struct Keranjang
{
    int id_pembeli;
    string username;
    string nama_barang;
    string jenis_barang;
    int harga_barang;
    int jumlah;
    int total;
    Keranjang *next = NULL;
};

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
    riwayat *next = NULL;
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


void clear(Akun **head)
{
    *head = NULL;
    return;
}

int add_id = 0; // penghitung jumlah akun

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

// Login
void Baca_Akun(Akun **head, Akun **tail) {
    ifstream file;
    string data;
    file.open("./A2_Kelompok4/akun.csv", ios::in); // Membuka file dengan mode input

    if (file.is_open()) {
        while (getline(file, data)) {
            Akun* newNode = new Akun;
            stringstream ss(data);
            string namas, usernames, passwords;

            // Memisahkan data dari string yang dibaca berdasarkan pemisah koma
            getline(ss, usernames, ',');
            getline(ss, passwords, '\n');

            // Mengisi struktur Akun dengan data dari file
            newNode->username = usernames;
            newNode->password = passwords;

            // Memasukkan node baru ke linked list
            if (*head == NULL) {
                *head = newNode;
                *tail = newNode;
            } else {
                newNode->prev = *tail;
                (*tail)->next = newNode;
                *tail = newNode;
            }
            add_id++;
        }
        file.close();
    } else {
        cerr << "Unable to open the file Akun.csv" << endl;
    }
}

void Tulis_Akun(Akun* head) {
    ofstream file("./A2_Kelompok4/Akun.csv", ios::out | ios::trunc); // Membuka file untuk penulisan

    if (file.is_open()) {
        Akun* curNode = head;
        // Menulis data ke dalam file CSV
        while (curNode != NULL) {
            file << curNode->username << ",";
            file << curNode->password << endl;
            curNode = curNode->next;
        }
        file.close();
    } else {
        cerr << "Unable to open the file Akun.csv" << endl;
    }
}

// Fungsi untuk menambahkan akun baru ke linked list
void registrasi(Akun **head, Akun  **tail) {
    Akun* node_baru = new Akun;
    string password;
    bool loop = true;
    Akun* cur_akun = *head;

    // Meminta input informasi akun baru
    while (loop) {
        while (true) {
            system("cls");
            cout << "===================================================" << endl;
            cout << "                  Registrasi Akun                  " << endl;
            cout << "===================================================" << endl;
            cout << "Masukkan Username Pribadi/Perusahaan : ";
            fflush(stdin);
            getline(cin, node_baru->username);
            if (node_baru->username.empty()) {
                cout << "\nUsername Tidak Boleh Kosong!\n" << endl;
                getch();
                system("cls");
            } else {
                break;
            }
        }
        cout << "Masukkan Password                    : ";fflush(stdin);
        cin >> password;
        cout << "Konfirmasi Password                  : ";
        cin >> node_baru->password;

        // Memvalidasi data yang dimasukkan
        if (*head == NULL) {
            break;
        } else if (password != node_baru->password) {
            cout << "\nPassword Yang Anda Masukkan Berbeda, Silahkan Input Kembali (Enter)"; 
            getch();
            system("cls");
        } else {
            while (cur_akun != NULL) {
                if (cur_akun->username == node_baru->username) {
                    cout << "\nUsername Telah Digunakan, Silahkan Gunakan Username Yang Lain (Enter)";
                    getch();
                    system("cls");
                    break;
                } else {
                    cur_akun = cur_akun->next;
                }
                if (cur_akun == NULL) {
                    loop = false;
                    break;
                }
            }
        }
    }

    // Menambahkan akun baru ke linked list
    node_baru->next = *head;
    if (*head != NULL) {
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


// Menghitung Keranjang
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

void Baca_gudang(Gudang **head, Gudang **tail)
{
    ifstream file;
    string isi, id_barangs, nama_barangs, jenis_barangs, stock_barangs, harga_barangs, tanggal_masuks, Expireds;
    int id_int, stock_int, harga_int;

    file.open("./A2_Kelompok4/gudang.csv", ios::in);
    if (file.is_open())
    {
        while (getline(file, isi))
        {
            Gudang *newNode = new Gudang;
            stringstream ss(isi);

            getline(ss, id_barangs, ',');
            getline(ss, nama_barangs, ',');
            getline(ss, jenis_barangs, ',');
            getline(ss, stock_barangs, ',');
            getline(ss, harga_barangs, ',');
            getline(ss, tanggal_masuks, ',');
            getline(ss, Expireds, '\n');

            stringstream id_string(id_barangs);
            stringstream stock_string(stock_barangs);
            stringstream harga_string(harga_barangs);

            id_string >> id_int;
            stock_string >> stock_int;
            harga_string >> harga_int;

            newNode->id_barang = id_int;
            newNode->nama_barang = nama_barangs;
            newNode->jenis_barang = jenis_barangs;
            newNode->stock_barang = stock_int;
            newNode->harga_barang = harga_int;
            newNode->tanggal_masuk = tanggal_masuks;
            newNode->Expired = Expireds;

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
    file.close();
}

void Baca_queue(checkout **head)
{
    ifstream file;
    string isi, id_pembelis, namas, nama_barangs, jenis_barangs, harga_barangs, jumlahs, totals, statuses,usernames;
    int pembeli_int, harga_int, jumlah_int, total_int;

    file.open("./A2_Kelompok4/checkout.csv", ios::in);
    if (file.is_open())
    {
        while (getline(file, isi))
        {
            cur_check = *head;
            checkout *newNode = new checkout;
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

            id_string >> pembeli_int;
            harga_string >> harga_int;
            jumlah_string >> jumlah_int;
            total_string >> total_int;

            newNode->id_pembeli = pembeli_int;
            newNode->username = usernames;
            newNode->nama_barang = nama_barangs;
            newNode->jenis_barang = jenis_barangs;
            newNode->harga_barang = harga_int;
            newNode->jumlah = jumlah_int;
            newNode->total = total_int;
            newNode->status = statuses;

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
    file.close();
}

void Baca_riwayat(riwayat **Head)
{
    ifstream file;
    riwayat *temp = *Head;
    string isi, id_pembelis, namas, nama_barangs, jenis_barangs, harga_barangs, jumlahs, totals, statuses,usernames;
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

void display_gudang(Gudang *head)
{
    system("cls");

    if (head == NULL)
    {
        cout << "======================================" << endl;
        cout << "|         Gudang Masih Kosong        |" << endl;
        cout << "======================================" << endl;
        cout << "Masukkan Barang, baru terisi hehe :D";
        getch();
    }
    else
    {
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
        {
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
            tail = head;
            head = head->next;
            i++;
        }
        cout << resetiosflags(ios::adjustfield);
        cout << garis << endl;
        cout << "\nTekan ENTER Untuk Melanjutkan...";
        getch();
    }
}

void add_riwayat(riwayat **Head, checkout *temp, string status)
{
    riwayat *newNode = new riwayat;
    riwayat *add = *Head;
    newNode->id_pembeli = temp->id_pembeli;
    newNode->username = temp->username;
    newNode->nama_barang = temp->nama_barang;
    newNode->jenis_barang = temp->jenis_barang;
    newNode->harga_barang = temp->harga_barang;
    newNode->jumlah = temp->jumlah;
    newNode->total = temp->total;
    newNode->status = status;
    if (*Head == NULL)
    {
        *Head = newNode;
    }
    else
    {
        while (add->next != NULL)
        {
            add = add->next;
        }
        add->next = newNode;
    }
}

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

void Konfirmasi_antrian(checkout **head, riwayat **Head, Gudang *gdg)
{
    Gudang *temp = gdg;
    char confirm;
    system("cls");
    if (*head == NULL)
    {
        cout << "========================================" << endl;
        cout << "|            Antrian Kosong            |" << endl;
        cout << "========================================" << endl;
        getch();
    }
    else
    {
        int i = 1;
        cur_check = *head;
        while (cur_check != NULL)
        {
            while (true)
            {
                system("cls");
                cout << "\n==========================================" << endl;
                cout << "|            Konfirmasi Pesananan         |" << endl;
                cout << "============================================" << endl;
                cout << "| Antrian [" << i << "]  : " << endl;
                cout << "| ID Pembeli     : " << cur_check->id_pembeli << endl;
                cout << "| Nama Pembeli   : " << cur_check->username << endl;
                cout << "| Nama Barang    : " << cur_check->nama_barang << endl;
                cout << "| Jenis Barang   : " << cur_check->jenis_barang<< endl;
                cout << "| Harga Barang   : Rp " << cur_check->harga_barang << endl;
                cout << "| Jumlah Barang  : " << cur_check->jumlah << endl;
                cout << "| Total Harga    : Rp " << cur_check->total << endl;
                cout << "| Status Pesanan : " << cur_check->status << endl;
                cout << "=============================================\n" << endl;
                cout << "\nKetik K jika ingin kembali ke Menu Utama" << endl;
                cout << "\nKonfirmasi Pesanan Sukses (y/n) : ";
                confirm = getch();
                checkout *hapus = cur_check;
                if (confirm == 'y' || confirm == 'Y')
                {
                    *head = (*head)->next;
                    while (temp != NULL)
                    {
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
            if (confirm == 'k' || confirm == 'K')
            {
                menugudang();
            }
            cur_check = cur_check->next;
            i++;
        }
        Tulis_gudang(gdg);
        Tulis_queue(*head);
        Tulis_riwayat(*Head);
    }
}

void enqueue_pesanan(string username, Keranjang **head, checkout **HEAD)
{
    Keranjang *temp_delete = *head;
    cur_check = *HEAD;
    Keranjang *temp = *head;
    int no, tanda = 0;
    display_keranjang(username, *head);
    cout << "\nIngin Checkout Data Keranjang Nomor Berapa : ";
    cin >> no;
    if (no < 0 || no > hitung_keranjang(username, head))
    {
        cout << "\nNomor Yand Anda Masukkan Tidak Sesuai!";
        getch();
    }
    else if (no > 0 && no <= hitung_keranjang(username, head))
    {
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
        checkout *newNode = new checkout;
        newNode->id_pembeli = temp_delete->id_pembeli;
        newNode->username = username;
        newNode->nama_barang = temp_delete->nama_barang;
        newNode->jenis_barang = temp_delete->jenis_barang;
        newNode->harga_barang = temp_delete->harga_barang;
        newNode->jumlah = temp_delete->jumlah;
        newNode->total = temp_delete->total;

        newNode->status = "Menunggu";
        if (*HEAD == NULL)
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
        Tulis_queue(*HEAD);
        if (temp_delete == *head)
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
        delete temp_delete;
        Tulis_keranjang(*head);
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
    {
        cin.clear();
        cin.ignore(100, '\n');
        cout << "\nInputan Anda Salah!";
        getch();
    }
}

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

void clear_riwayat(riwayat *head)
{
    riwayat *temp = head, *hapus;
    while (temp != NULL)
    {
        hapus == temp;
        if (temp->next == NULL)
        {
            head = NULL;
        }
        else
        {
            head = head->next;
        }
        delete hapus;
        temp = temp->next;
    }
}

Gudang *add_gudang(Gudang **head)
{
    bool loop;
    loop = true;
    Gudang *nodeBaru = new Gudang;
    Gudang *temp = *head;
    cout << "==========================================" << endl;
    cout << "|              Tambah Barang             |" << endl;
    cout << "==========================================" << endl;
    while (loop)
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
                loop = false; // Jika ID tersedia, keluar dari loop
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
    while (true)
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
    while (true)
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
    while (true)
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
    while (true)
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
    return nodeBaru;
}

int cek_riwayat_user(string username, riwayat *head)
{
    riwayat *temp = head;
    while (temp != NULL)
    {
        if (temp->username == username)
        {
            return 1;
            break;
        }
        temp = temp->next;
    }
    return -1;
}

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
                cout << "=============================================\n" << endl;
                i++;
            }
            temp = temp->next;
        }
        cout << "\nTekan ENTER Untuk Lanjut!";
        getch();
    }
}

void addfirst(Gudang **head, Gudang **tail)
{
    system("cls");
    Gudang *nodeBaru = add_gudang(head);
    nodeBaru->prev = NULL;
    nodeBaru->next = NULL;
    if (*head == NULL)
    {
        *head = nodeBaru;
        *tail = nodeBaru;
    }
    else
    {
        nodeBaru->prev = NULL;
        nodeBaru->next = *head;
        (*head)->prev = nodeBaru;
        *head = nodeBaru;
    }
    Tulis_gudang(*head);
    cout << "=============================================" << endl;
    cout << "|     Barang berhasil ditambahkan Pertama   |" << endl;
    cout << "=============================================" << endl;
    getch();
}

void addMiddle(Gudang **head, Gudang **tail)
{
    system("cls");
    Gudang *nodeBaru = add_gudang(head);
    int nomor;
    int ulang = 0;
    while (ulang == 0)
    {
        cout << "| Pilih Masukkan Data Ke Nomor : ";
        cin >> nomor;
        if (nomor > 1 && nomor < Jumlah_Data() + 1)
        {
            cur = *head;
            int Nomor = 1;
            while (Nomor < nomor - 1)
            {
                cur = cur->next;
                Nomor++;
            }
            cur->next->prev = nodeBaru;
            nodeBaru->next = cur->next;
            cur->next = nodeBaru;
            nodeBaru->prev = cur;
            cout << endl;
            Tulis_gudang(*head);
            cout << "===========================================" << endl;
            cout << "|    Barang Berhasil Ditambah Ditengah    |" << endl;
            cout << "===========================================";
            getch();
            ulang++;
        }
        else
        {
            cout << "Barang harus ditambah di tengah!" << endl;
        }
    }
}

void addLast(Gudang **head, Gudang **tail)
{
    system("cls");
    Gudang *nodeBaru = add_gudang(head);
    if (Jumlah_Data() >= 1)
    {
        nodeBaru->next = NULL;
        nodeBaru->prev = *tail;
        if (*head == NULL && *tail == NULL)
        {
            *head = nodeBaru;
            *tail = nodeBaru;
            return;
        }
        (*tail)->next = nodeBaru;
        *tail = nodeBaru;
    }
    else
    {
        nodeBaru->next = NULL;
        nodeBaru->prev = NULL;
        *head = nodeBaru;
        *tail = nodeBaru;
    }
    Tulis_gudang(*head);
    cout << "============================================" << endl;
    cout << "|     Barang Berhasil Ditambah DiAkhir     |" << endl;
    cout << "============================================";
    getch();
}

void update(Gudang **head, Gudang **tail)
{
    int No;
    bool loop = true;
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
            cout << "\n=========================================" << endl;
            cout << "|         Update Barang Terbaru         |" << endl;
            cout << "=========================================" << endl;
            while (loop)
            {
                cout << "| Masukkan ID Barang      > ";
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
                                loop = false;
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
                            loop = false;
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
                cout << "| Masukkan Nama Barang       > ";
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
                cout << "| Masukkan Jenis Barang       > ";
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
                cout << "| Masukkan Stock Barang       > ";
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
                cout << "| Masukkan Harga Barang       > ";
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
                cout << "| Masukkan Tanggal Masuk(DD/MM/YYYY)     > ";
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
                cout << "| Masukkan Expired Barang(DD/MM/YYYY)     > ";
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
// QuickSort
struct Gudang *getTailasc(struct Gudang *cur)
{
    while (cur != NULL && cur->next != NULL)
    {
        cur = cur->next;
    }
    return cur;
}

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

void quickSortasc(struct Gudang **headRef)
{
    (*headRef) = quickSortRecurasc(*headRef, getTailasc(*headRef));
    return;
}

// quick Sort desc
struct Gudang *getTaildesc(struct Gudang *cur)
{
    while (cur != NULL && cur->next != NULL)
    {
        cur = cur->next;
    }
    return cur;
}

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

void quickSortdesc(struct Gudang **headRef)
{
    (*headRef) = quickSortRecurdesc(*headRef, getTaildesc(*headRef));
    return;
}

// ===============================================================
// Searching
// Boyer-Moore
const int NO_OF_CHARS = 256;

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

// Berdasarkan Nama Barang
void BoyerMoore(Gudang *head)
{
    string nama;
    cout << "Masukkan Nama yang dicari : ";
    cin >> nama;

    int m = nama.size();
    Gudang *temp = head;
    int getdata = 0;

    while (temp != NULL)
    {
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

    if (getdata == 0)
    {
        cout << "Data tidak ditemukan!!" << endl;
    }
}


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

Gudang *findNode(Gudang *head, int idx)
{
    for (int a = 0; a < idx; a++)
    {
        head = head->next;
    }
    return head;
}

int min(int x, int y) { return (x <= y) ? x : y; }
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

        if (temp->id_barang < x)
        {
            fibM = fibMMm1;
            fibMMm1 = fibMMm2;
            fibMMm2 = fibM - fibMMm1;
            offset = i;
        }
        else if (temp->id_barang > x)
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

    // if (fibMMm1 && temp->id_barang == x) {
    //    Gudang* foundNode = findNode(head, offset + 1);
    //    cout << "\n========================================" << endl;
    //     cout << "|              Data Barang             |" << endl;
    //     cout << "========================================" << endl;
    //     cout << "| ID Barang         > " << foundNode->id_barang << endl;
    //     cout << "| Nama Barang       > " << foundNode->nama_barang << endl;
    //     cout << "| Jenis Barang      > " << foundNode->jenis_barang << endl;
    //     cout << "| Stock Barang      > " << foundNode->stock_barang << endl;
    //     cout << "| Harga Barang      > " << foundNode->harga_barang << endl;
    //     cout << "| Tanggal Masuk     > " << foundNode->tanggal_masuk << endl;
    //     cout << "| Expired Barang    > " << foundNode->Expired << endl;
    //     cout << "========================================" << endl;
    //     getch();

    // }else{

    // cout << x << " isn't present in the array" << endl;
    // getch();
    // }
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

int cek_keranjang(string username, Keranjang *head)
{
    cur_keranjang = head;
    while (cur_keranjang != NULL)
    {
        if (cur_keranjang->username == username)
        {
            return 1;
        }
        else
        {
            cur_keranjang = cur_keranjang->next;
        }
    }
    return -1;
}

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

void tambah_keranjang(string username, Gudang *HEAD, Keranjang **head, checkout **head_check)
{
    string beli;
    int pilih, jumlah, total;
    display_gudang(HEAD);
    Gudang *temp = HEAD;
    cur_check = *head_check;

    cout << "\n===============================================" << endl;
    cout << "|           Form Tambah Pesanan Barang        |" << endl;
    cout << "===============================================" << endl;

    cout << "\nMasukkan Nomor Barang Yang Ingin Dipesan: ";
    cin >> pilih;

    if (pilih > 0 && pilih <= Jumlah_Data())
    {
        for (int i = 1; i < pilih; i++)
        {
            temp = temp->next;
        }

        if (temp->stock_barang < 1)
        {
            cout << "\nStok Barang Masih Kosong...";
            getch();
        }
        else
        {
            system("cls");

            cout << "\n===============================================" << endl;
            cout << "|            Form Tambah Pesanan Barang     |" << endl;
            cout << "===============================================" << endl;
            cout << "ID Barang            : " << temp->id_barang << endl;
            cout << "Nama Barang          : " << temp->nama_barang << endl;
            cout << "Jenis Barang         : " << temp->jenis_barang << endl;
            cout << "Harga                : Rp" << temp->harga_barang << endl;
            cout << "Jumlah Stok          : " << temp->stock_barang << endl;
            cout << "-----------------------------------------------" << endl;

            while (true)
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
            total = temp->harga_barang * jumlah;
            Keranjang *nodeBaru = new Keranjang;
            nodeBaru->username = username;
            nodeBaru->id_pembeli = temp->id_barang;
            nodeBaru->nama_barang = temp->nama_barang;
            nodeBaru->jenis_barang = temp->jenis_barang;
            nodeBaru->harga_barang = temp->harga_barang;
            nodeBaru->jumlah = jumlah;
            nodeBaru->total = total;

            while (true)
            {
                system("cls");

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
                cout << "\n  [1] Langsung Checkout " << endl;
                cout << "  [2] Kembali " << endl;
                cout << "===================================================" << endl;
                cout << "Masukkan Pilihan: ";
                cin >> beli;

                if (beli == "1")
                {
                    checkout *newNode = new checkout;
                    newNode->username = username;
                    newNode->id_pembeli = temp->id_barang;
                    newNode->nama_barang = nodeBaru->nama_barang;
                    newNode->jenis_barang = nodeBaru->jenis_barang;
                    newNode->harga_barang = nodeBaru->harga_barang;
                    newNode->jumlah = nodeBaru->jumlah;
                    newNode->total = nodeBaru->total;
                    newNode->status = "Menunggu";

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

                    Tulis_queue(*head_check);
                    system("cls");

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
                else if (beli == "2")
                {
                    nodeBaru->next = *head;
                    *head = nodeBaru;
                    Tulis_keranjang(*head);
                    break;
                }
                else
                {
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
    {
        cin.clear();
        cin.ignore(100, '\n');
        cout << "\nInputan Anda Salah!";
        getch();
    }
}

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
        display_keranjang(username, *head);
        cout << "\nMasukkan Nomor Data Yang Ingin Diupdate : ";
        cin >> no;
        if (no < 0 || no > hitung_keranjang(username, head))
        {
            cout << "\nNomor Yang Anda Masukkan Tidak Sesuai!";
            getch();
        }
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
                        temp->jumlah = jumlah;
                        temp->total = harga * temp->jumlah;
                        Tulis_keranjang(*head);
                        system("cls");
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
        display_keranjang(username, *head);
        cout << "\nMasukkan Nomor Data Yang Ingin Dihapus : ";
        cin >> no;
        if (no < 0 || no > hitung_keranjang(username, head))
        {
            cout << "\nNomor Yang Anda Masukkan Tidak Sesuai!";
            getch();
        }
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

void Menu_utama()
{
    bool loop = true;
    while (loop)
    {
        system("cls");
        int pilih;
        cout << "====================================" << endl;
        cout << "|        Welcome to IndoChino      |" << endl;
        cout << "====================================" << endl;
        cout << "| <1> Login                        |" << endl;
        cout << "| <2> Registrasi Akun              |" << endl;
        cout << "| <3> Exit Program                 |" << endl;
        cout << "====================================" << endl;
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
                login();
            }
            else if (pilih == 2)
            {
                registrasi(&head_akun,&tail_akun);
            }
            else if (pilih == 3)
            {
                cout << "\n ____________________________________________" << endl;
                cout << "|                                             |" << endl;
                cout << "|              SELAMAT TINGGAL                |" << endl;
                cout << "|                     &                       |" << endl;
                cout << "|               TERIMA KASIH                  |" << endl;
                cout << "|_____________________________________________|" << endl;
                loop = false;
                break;
            }
            else
            {
                cout << "Masukkan Data Dengan benar!" << endl;
                getch();
            }
        }
    }
}

// Menu Login,Register,Exit Program
void login()
{
    bool ulang = true;
    string username, password, nama;
    int id_pembeli;

    while (ulang)
    {
        Akun *cur_akun = head_akun;
        system("cls");
        cout << "==================================================================" << endl;
        cout << "                             Login                                " << endl;
        cout << "==================================================================" << endl;
        cout << "\n[NOTE: Masukkan angka 0 di username untuk kembali ke menu awal]\n"
             << endl;
        cout << "Masukkan Username : ";
        getline(cin, username);

        if (username == "0")
        {
            break;
        }
        if (username.empty())
        {
            cout << "Username tidak boleh kosong. Tekan tombol apapun untuk mencoba lagi...";
            getch();
        }
        else
        {
            cout << "Masukkan Password : ";
            cin >> password;
            if (password.empty())
            {
                cout << "Password tidak boleh kosong. Tekan tombol apapun untuk mencoba lagi...";
                getch();
            }

            while (cur_akun != NULL)
            {
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
                if (username == "admin" && password == "admin")
                {
                    cout << "\n==================================================================" << endl;
                    cout << "                         Login Admin Berhasil                           " << endl;
                    cout << "==================================================================" << endl;
                    menugudang();
                    ulang = false;
                    break;
                }
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

void Menu_Tambah()
{
    int no;
    while (true)
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
            {
                addfirst(&head, &tail);
            }
            else if (no == 2)
            {
                addMiddle(&head, &tail);
            }
            else if (no == 3)
            {
                addLast(&head, &tail);
            }
            else if (no == 4)
            {
                break;
            }
            else
            {
                cout << "Pilihan Anda Tidak Ada Di menu" << endl;
                getch();
            }
        }
    }
}

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
                getch();
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
            {
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
                    cout << "===================================================" << endl;
                    cout << "               DAFTAR KERANJANG KOSONG            " << endl;
                    cout << "===================================================" << endl;
                    cout << "\nSilahkan tambah pesanan terlebih dahulu...";
                    getch();
                }
                else
                {
                    display_keranjang(username, head_keranjang);
                    cout << "\nTekan ENTER Untuk Melanjutkan...";
                    getch();
                }
            }
            else if (pilihpembeli == 5)
            {
                update_keranjang(username, &head_keranjang, &head);
            }
            else if (pilihpembeli == 6)
            {
                hapus_keranjang(username, &head_keranjang);
            }
            else if (pilihpembeli == 7)
            {
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
                    enqueue_pesanan(username, &head_keranjang, &head_check);
                }
            }
            else if (pilihpembeli == 8)
            {
                display_check(username, head_check);
            }
            else if (pilihpembeli == 9)
            {
                display_riwayat_user(username, head_riwayat);
            }
            else if (pilihpembeli == 0)
            {
            cout << "\n=================================================" << endl;
			cout << "                Anda Telah Logout                " << endl; 
			cout << "================================================="; 
			ulang = false;
            }
            else
            {
                cout << "Data tidak dapat ditemukan" << endl;
                getch();
            }
        }
    }
};

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
        cout << "| <7> Confirm Pemesanan (Belum)              |" << endl;
        cout << "| <8> Display Riwayat (Belum)                |" << endl;
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
                getch();
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
                Menu_Tambah();
            }
            else if (pilihmenu == 2)
            {
                Pilihsort_gudang();
            }
            else if (pilihmenu == 3)
            {
                delete_gudang(&head, &tail);
            }
            else if (pilihmenu == 4)
            {
                update(&head, &tail);
            }
            else if (pilihmenu == 5)
            {
                Cari_Gudang();
            }
            else if (pilihmenu == 6)
            {
                display_antrian(head_check);
            }
            else if (pilihmenu == 7)
            {
                Konfirmasi_antrian(&head_check, &head_riwayat, head);
            }
            else if (pilihmenu == 8)
            {
                display_riwayat(head_riwayat);
            }
            else if (pilihmenu == 0)
            {
            cout << "\n=================================================" << endl;
			cout << "                Anda Telah Logout                " << endl; 
			cout << "================================================="; 
			ulang = false;
            }
            else
            {
                cout << "Data tidak dapat ditemukan" << endl;
                getch();
            }
        }
    }
}

int main()
{
    Baca_Akun(&head_akun,&tail_akun); // Memuat data akun dari file
    Baca_gudang(&head, &tail);
    Baca_keranjang(&head_keranjang);
    Baca_queue(&head_check);
    Baca_riwayat(&head_riwayat);
    Menu_utama();
    return 0;
}

