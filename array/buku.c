#include <stdio.h>
#include <string.h>
#include "buku.h"

void initBuku(DaftarBuku *db) {
    db->jumlah = 0;
}

int tambahBuku(DaftarBuku *db, const char *judul, int stok) {
    if (db->jumlah >= MAX_BUKU) {
        return 0; // Gagal, array penuh
    }
    
    strncpy(db->daftar[db->jumlah].judul, judul, MAX_JUDUL - 1);
    db->daftar[db->jumlah].judul[MAX_JUDUL - 1] = '\0';
    db->daftar[db->jumlah].stok = stok;
    db->daftar[db->jumlah].stok_tersedia = stok;
    db->jumlah++;
    
    return 1; // Sukses
}

Buku* cariBuku(DaftarBuku *db, const char *judul) {
	int i;
    for (i = 0; i < db->jumlah; i++) {
        if (strcmp(db->daftar[i].judul, judul) == 0) {
            return &db->daftar[i];
        }
    }
    return NULL;
}

void tampilkanBuku(const DaftarBuku *db) {
    printf("Daftar Buku:\n");
    int i;
    for (i = 0; i < db->jumlah; i++) {
        printf("- %s (Stok: %d, Tersedia: %d)\n", 
               db->daftar[i].judul, 
               db->daftar[i].stok, 
               db->daftar[i].stok_tersedia);
    }
}

int pinjamBuku(DaftarBuku *db, const char *judul) {
    Buku* buku = cariBuku(db, judul);
    if (buku == NULL || buku->stok_tersedia <= 0) {
        return 0; // Gagal
    }
    
    buku->stok_tersedia--;
    return 1; // Sukses
}

int kembalikanBuku(DaftarBuku *db, const char *judul) {
    Buku* buku = cariBuku(db, judul);
    if (buku == NULL || buku->stok_tersedia >= buku->stok) {
        return 0; // Gagal
    }
    
    buku->stok_tersedia++;
    return 1; // Sukses
}
