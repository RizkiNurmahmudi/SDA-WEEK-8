#ifndef PERPUSTAKAAN_H
#define PERPUSTAKAAN_H

#include "buku.h"
#include "peminjam.h"

#define MAX_HISTORY 100
#define MAX_NAMA 100
#define MAX_JUDUL 100

// Struktur untuk menyimpan history aktivitas
typedef struct {
    char aksi[50];          // Jenis aksi (pinjam, kembali, dll)
    char nama[MAX_NAMA];    // Nama peminjam
    char buku[MAX_JUDUL];   // Judul buku
    Prioritas prioritas;    // Prioritas peminjam
} History;

// Struktur utama perpustakaan
typedef struct {
    DaftarBuku db;              // Daftar buku
    AntrianPeminjam aq;         // Antrian peminjam
    History history[MAX_HISTORY]; // Riwayat aktivitas
    int history_count;           // Jumlah riwayat
} Perpustakaan;

// Deklarasi fungsi-fungsi manajemen perpustakaan
void initPerpustakaan(Perpustakaan *p);  // Inisialisasi perpustakaan
void tambahBukuPerpus(Perpustakaan *p, const char *judul, int stok);  // Menambah buku
void daftarPeminjam(Perpustakaan *p, const char *nama, Prioritas prioritas, const char *buku);  // Mendaftarkan peminjam
void prosesPeminjaman(Perpustakaan *p, const char *buku);  // Memproses peminjaman
void prosesPengembalian(Perpustakaan *p, const char *id);  // Memproses pengembalian
void batalkanAntrian(Perpustakaan *p, const char *id);  // Membatalkan antrian
void tampilkanStatusPerpustakaan(const Perpustakaan *p);  // Menampilkan status perpustakaan

#endif
