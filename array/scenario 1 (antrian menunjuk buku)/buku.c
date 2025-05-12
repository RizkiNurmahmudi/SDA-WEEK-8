#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "buku.h"

// Fungsi untuk inisialisasi daftar buku
void initBuku(DaftarBuku *db) {
    db->jumlah = 0;   // Set jumlah buku ke 0
}

// Fungsi untuk menambahkan buku baru ke daftar
int tambahBuku(DaftarBuku *db, const char *judul, int stok) {
    // Periksa apakah daftar sudah penuh
    if (db->jumlah >= MAX_BUKU) {
        return 0;  // Mengembalikan 0 jika daftar penuh
    }
    
    // Periksa apakah buku sudah ada
    if (cariBuku(db, judul) != NULL) {
        printf("Buku dengan judul '%s' sudah ada!\n", judul);
        return 0;
    }
    
    // Isi data buku baru
    strncpy(db->data[db->jumlah].judul, judul, MAX_JUDUL - 1);
    db->data[db->jumlah].judul[MAX_JUDUL - 1] = '\0'; // Pastikan null-terminated
    db->data[db->jumlah].stok = stok;
    db->data[db->jumlah].stok_tersedia = stok;
    
    db->jumlah++;  // Tambah jumlah buku
    return 1;      // Mengembalikan 1 untuk menandakan sukses
}

// Fungsi untuk mencari buku berdasarkan judul
Buku* cariBuku(DaftarBuku *db, const char *judul) {
    // Iterasi melalui semua buku dalam array
    int i;
    for (i = 0; i < db->jumlah; i++) {
        if (strcmp(db->data[i].judul, judul) == 0) {
            return &db->data[i];  // Kembalikan pointer buku jika ditemukan
        }
    }
    return NULL;  // Kembalikan NULL jika tidak ditemukan
}

// Fungsi untuk menampilkan daftar buku
void tampilkanBuku(const DaftarBuku *db) {
    printf("Daftar Buku:\n");
    if (db->jumlah == 0) {
        printf("Tidak ada buku terdaftar\n");
        return;
    }
    
    // Tampilkan semua buku dalam array
    int i;
    for (i = 0; i < db->jumlah; i++) {
        printf("- %s (Stok: %d, Tersedia: %d)\n", 
               db->data[i].judul, 
               db->data[i].stok, 
               db->data[i].stok_tersedia);
    }
}

// Fungsi untuk memproses peminjaman buku
int pinjamBuku(DaftarBuku *db, const char *judul) {
    Buku* buku = cariBuku(db, judul);
    // Periksa ketersediaan buku dan stok
    if (buku == NULL) {
        printf("Buku '%s' tidak ditemukan!\n", judul);
        return 0;
    }
    if (buku->stok_tersedia <= 0) {
        printf("Stok buku '%s' habis!\n", judul);
        return 0;
    }
    buku->stok_tersedia--;  // Kurangi stok tersedia
    return 1;               // Berhasil meminjam
}

// Fungsi untuk memproses pengembalian buku
int kembalikanBuku(DaftarBuku *db, const char *judul) {
    Buku* buku = cariBuku(db, judul);
    // Periksa ketersediaan buku dan stok
    if (buku == NULL) {
        printf("Buku '%s' tidak ditemukan!\n", judul);
        return 0;
    }
    if (buku->stok_tersedia >= buku->stok) {
        printf("Stok buku '%s' sudah penuh!\n", judul);
        return 0;
    }
    buku->stok_tersedia++;  // Tambah stok tersedia
    return 1;               // Berhasil mengembalikan
}
