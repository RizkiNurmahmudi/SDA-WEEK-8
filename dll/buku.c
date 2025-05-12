#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buku.h"

// Fungsi untuk inisialisasi daftar buku
void initBuku(DaftarBuku *db) {
    db->head = NULL;  // Set head ke NULL (daftar kosong)
    db->tail = NULL;  // Set tail ke NULL (daftar kosong)
    db->jumlah = 0;   // Inisialisasi jumlah buku ke 0
}

// Fungsi untuk menambahkan buku baru ke daftar
int tambahBuku(DaftarBuku *db, const char *judul, int stok) {
    // Alokasi memori untuk buku baru
    Buku* newBuku = (Buku*)malloc(sizeof(Buku));
    if (newBuku == NULL) {
        return 0;  // Mengembalikan 0 jika alokasi gagal
    }
    
    // Mengisi data buku baru
    strncpy(newBuku->judul, judul, MAX_JUDUL - 1);  // Salin judul buku
    newBuku->judul[MAX_JUDUL - 1] = '\0';           // Pastikan string diakhiri NULL
    newBuku->stok = stok;                           // Set stok total
    newBuku->stok_tersedia = stok;                  // Set stok tersedia
    newBuku->prev = NULL;                           // Inisialisasi pointer prev
    newBuku->next = NULL;                           // Inisialisasi pointer next
    
    // Menambahkan buku ke daftar
    if (db->head == NULL) {
        // Jika daftar kosong, buku baru menjadi head dan tail
        db->head = newBuku;
        db->tail = newBuku;
    } else {
        // Jika daftar tidak kosong, tambahkan di akhir
        db->tail->next = newBuku;  // Link dari tail ke buku baru
        newBuku->prev = db->tail;  // Link prev buku baru ke tail lama
        db->tail = newBuku;        // Update tail ke buku baru
    }
    
    db->jumlah++;  // Increment jumlah buku
    return 1;      // Mengembalikan 1 untuk menandakan sukses
}

// Fungsi untuk mencari buku berdasarkan judul
Buku* cariBuku(DaftarBuku *db, const char *judul) {
    Buku* current = db->head;  // Mulai pencarian dari head
    
    // Iterasi melalui semua buku dalam daftar
    while (current != NULL) {
        // Bandingkan judul buku dengan yang dicari
        if (strcmp(current->judul, judul) == 0) {
            return current;  // Mengembalikan pointer buku jika ditemukan
        }
        current = current->next;  // Pindah ke buku berikutnya
    }
    
    return NULL;  // Mengembalikan NULL jika tidak ditemukan
}

// Fungsi untuk menampilkan daftar buku
void tampilkanBuku(const DaftarBuku *db) {
    printf("Daftar Buku:\n");
    Buku* current = db->head;  // Mulai dari head
    
    // Iterasi dan tampilkan semua buku
    while (current != NULL) {
        printf("- %s (Stok: %d, Tersedia: %d)\n", 
               current->judul, 
               current->stok, 
               current->stok_tersedia);
        current = current->next;  // Pindah ke buku berikutnya
    }
}

// Fungsi untuk memproses peminjaman buku
int pinjamBuku(DaftarBuku *db, const char *judul) {
    // Cari buku berdasarkan judul
    Buku* buku = cariBuku(db, judul);
    
    // Periksa apakah buku ditemukan dan stok tersedia
    if (buku == NULL || buku->stok_tersedia <= 0) {
        return 0;  // Mengembalikan 0 jika gagal
    }
    
    buku->stok_tersedia--;  // Kurangi stok tersedia
    return 1;               // Mengembalikan 1 untuk menandakan sukses
}

// Fungsi untuk memproses pengembalian buku
int kembalikanBuku(DaftarBuku *db, const char *judul) {
    // Cari buku berdasarkan judul
    Buku* buku = cariBuku(db, judul);
    
    // Periksa apakah buku ditemukan dan stok belum penuh
    if (buku == NULL || buku->stok_tersedia >= buku->stok) {
        return 0;  // Mengembalikan 0 jika gagal
    }
    
    buku->stok_tersedia++;  // Tambah stok tersedia
    return 1;               // Mengembalikan 1 untuk menandakan sukses
}

// Fungsi untuk membebaskan memori yang digunakan daftar buku
void freeBuku(DaftarBuku *db) {
    Buku* current = db->head;  // Mulai dari head
    
    // Iterasi dan bebaskan semua buku
    while (current != NULL) {
        Buku* temp = current;       // Simpan pointer buku saat ini
        current = current->next;    // Pindah ke buku berikutnya
        free(temp);                 // Bebaskan memori buku saat ini
    }
    
    // Reset daftar buku
    db->head = NULL;
    db->tail = NULL;
    db->jumlah = 0;
}
