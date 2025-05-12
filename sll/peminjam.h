#ifndef PEMINJAM_H
#define PEMINJAM_H

#include "buku.h"

#define MAX_NAMA 100

// Enum untuk prioritas peminjam
typedef enum {
    DOSEN = 1,      // Prioritas tertinggi (1)
    MAHASISWA = 2,  // Prioritas menengah (2)
    UMUM = 3        // Prioritas terendah (3)
} Prioritas;

// Struktur node untuk menyimpan data peminjam
typedef struct PeminjamNode {
    char nama[MAX_NAMA];    // Nama peminjam
    Prioritas prioritas;    // Prioritas peminjam
    Buku* buku;             // Pointer ke buku yang dipinjam
    struct PeminjamNode *prev;  // Pointer ke node sebelumnya
    struct PeminjamNode *next;  // Pointer ke node berikutnya
} PeminjamNode;

// Struktur untuk mengelola antrian peminjam
typedef struct {
    PeminjamNode *head;  // Pointer ke node pertama dalam antrian
    PeminjamNode *tail;  // Pointer ke node terakhir dalam antrian
    int size;            // Jumlah peminjam dalam antrian
} AntrianPeminjam;

// Deklarasi fungsi-fungsi manajemen peminjam
void initAntrian(AntrianPeminjam *aq);  // Inisialisasi antrian
int enqueue(AntrianPeminjam *aq, const char *nama, Prioritas prioritas, Buku* buku);  // Menambah peminjam ke antrian
PeminjamNode* dequeue(AntrianPeminjam *aq, Buku* buku);  // Mengeluarkan peminjam dari antrian berdasarkan prioritas
void tampilkanAntrian(const AntrianPeminjam *aq);  // Menampilkan daftar peminjam dalam antrian
PeminjamNode* cariPeminjamByBuku(AntrianPeminjam *aq, Buku* buku);  // Mencari peminjam berdasarkan buku
int hapusPeminjam(AntrianPeminjam *aq, const char *nama, Buku* buku);  // Menghapus peminjam dari antrian

#endif
