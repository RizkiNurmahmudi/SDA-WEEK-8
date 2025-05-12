#ifndef BUKU_H
#define BUKU_H

#define MAX_JUDUL 100
#define MAX_BUKU 100 // Batas maksimum buku yang dapat disimpan

// Struktur data untuk menyimpan informasi buku
typedef struct Buku {
    char judul[MAX_JUDUL];  // Menyimpan judul buku
    int stok;               // Jumlah total stok buku
    int stok_tersedia;      // Jumlah stok yang tersedia untuk dipinjam
} Buku;

// Struktur untuk mengelola daftar buku
typedef struct {
    Buku data[MAX_BUKU];    // Array untuk menyimpan buku
    int jumlah;             // Jumlah total buku dalam daftar
} DaftarBuku;

// Deklarasi fungsi-fungsi manajemen buku
void initBuku(DaftarBuku *db);  // Inisialisasi daftar buku
int tambahBuku(DaftarBuku *db, const char *judul, int stok);  // Menambah buku baru
Buku* cariBuku(DaftarBuku *db, const char *judul);  // Mencari buku berdasarkan judul
void tampilkanBuku(const DaftarBuku *db);  // Menampilkan daftar buku
int pinjamBuku(DaftarBuku *db, const char *judul);  // Memproses peminjaman buku
int kembalikanBuku(DaftarBuku *db, const char *judul);  // Memproses pengembalian buku

#endif
