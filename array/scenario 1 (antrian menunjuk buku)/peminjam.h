#ifndef PEMINJAM_H
#define PEMINJAM_H

#include "buku.h"

#define MAX_NAMA 100
#define MAX_PEMINJAM 100 // Batas maksimum peminjam
#define MAX_ID 20        // Panjang maksimum ID unik peminjam

// Enum untuk prioritas peminjam
typedef enum {
    DOSEN = 1,      // Prioritas tertinggi (1)
    MAHASISWA = 2,  // Prioritas menengah (2)
    UMUM = 3        // Prioritas terendah (3)
} Prioritas;

// Struktur untuk menyimpan data peminjam
typedef struct {
    char id[MAX_ID];       // ID unik peminjam
    char nama[MAX_NAMA];    // Nama peminjam
    Prioritas prioritas;    // Prioritas peminjam
    Buku* buku;             // Pointer ke buku yang dipinjam
} Peminjam;

// Struktur untuk mengelola antrian peminjam
typedef struct {
    Peminjam data[MAX_PEMINJAM]; // Array untuk menyimpan peminjam
    int size;                    // Jumlah peminjam dalam antrian
} AntrianPeminjam;

// Deklarasi fungsi-fungsi manajemen peminjam
void initAntrian(AntrianPeminjam *aq);  // Inisialisasi antrian
int enqueue(AntrianPeminjam *aq, const char *id, const char *nama, Prioritas prioritas, Buku* buku);  // Menambah peminjam ke antrian
Peminjam* dequeue(AntrianPeminjam *aq, Buku* buku);  // Mengeluarkan peminjam berdasarkan prioritas
void tampilkanAntrian(const AntrianPeminjam *aq);  // Menampilkan daftar peminjam
int hapusPeminjam(AntrianPeminjam *aq, const char *id);  // Menghapus peminjam dari antrian berdasarkan ID
Peminjam* cariPeminjam(AntrianPeminjam *aq, const char *id); // Mencari peminjam berdasarkan ID
void generateID(char *id, const char *nama, const char *judul_buku); // Generate ID unik

#endif
