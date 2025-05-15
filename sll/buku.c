#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buku.h"


void initBuku(DaftarBuku *db) {
    db->head = NULL;
    db->jumlah = 0;
}

/**
 * Fungsi untuk menambahkan buku baru ke daftar (insert at end)
 * Mengembalikan 1 jika berhasil, 0 jika gagal
 */
int tambahBuku(DaftarBuku *db, const char *judul, int stok) {
    // Alokasi memori untuk buku baru
    Buku* newBuku = (Buku*)malloc(sizeof(Buku));
    if (newBuku == NULL) {
        return 0;  // Alokasi gagal
    }
    
    // Mengisi data buku baru
    strncpy(newBuku->judul, judul, MAX_JUDUL - 1);
    newBuku->judul[MAX_JUDUL - 1] = '\0';
    newBuku->stok = stok;
    newBuku->stok_tersedia = stok;
    newBuku->next = NULL;
    
    // Menambahkan buku ke daftar
    if (db->head == NULL) {
        // Jika daftar kosong, buku baru menjadi head
        db->head = newBuku;
    } else {
        // Jika daftar tidak kosong, cari node terakhir
        Buku* current = db->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newBuku;  // Tambahkan di akhir
    }
    
    db->jumlah++;
    return 1;
}

/**
 * Fungsi untuk mencari buku berdasarkan judul
 * Mengembalikan pointer ke buku jika ditemukan, NULL jika tidak
 */
Buku* cariBuku(DaftarBuku *db, const char *judul) {
    Buku* current = db->head;
    
    while (current != NULL) {
        if (strcmp(current->judul, judul) == 0) {
            return current;
        }
        current = current->next;
    }
    
    return NULL;
}

/**
 * Fungsi untuk menampilkan daftar buku
 */
void tampilkanBuku(const DaftarBuku *db) {
    printf("Daftar Buku:\n");
    Buku* current = db->head;
    
    while (current != NULL) {
        printf("- %s (Stok: %d, Tersedia: %d)\n", 
               current->judul, 
               current->stok, 
               current->stok_tersedia);
        current = current->next;
    }
}

/**
 * Fungsi untuk memproses peminjaman buku
 * Mengurangi stok tersedia jika berhasil
 * Mengembalikan 1 jika berhasil, 0 jika gagal
 */
int pinjamBuku(DaftarBuku *db, const char *judul) {
    Buku* buku = cariBuku(db, judul);
    
    if (buku == NULL || buku->stok_tersedia <= 0) {
        return 0;
    }
    
    buku->stok_tersedia--;
    return 1;
}

/**
 * Fungsi untuk memproses pengembalian buku
 * Menambah stok tersedia jika berhasil
 * Mengembalikan 1 jika berhasil, 0 jika gagal
 */
int kembalikanBuku(DaftarBuku *db, const char *judul) {
    Buku* buku = cariBuku(db, judul);
    
    if (buku == NULL || buku->stok_tersedia >= buku->stok) {
        return 0;
    }
    
    buku->stok_tersedia++;
    return 1;
}

/**
 * Fungsi untuk membebaskan memori yang digunakan daftar buku
 */
void freeBuku(DaftarBuku *db) {
    Buku* current = db->head;
    
    while (current != NULL) {
        Buku* temp = current;
        current = current->next;
        free(temp);
    }
    
    db->head = NULL;
    db->jumlah = 0;
}
