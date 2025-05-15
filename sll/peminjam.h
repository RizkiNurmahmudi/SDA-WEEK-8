#ifndef PEMINJAM_H
#define PEMINJAM_H

#include "buku.h"

#define MAX_NAMA 100

// Enum untuk prioritas peminjam
typedef enum {
    DOSEN = 1,
    MAHASISWA = 2,
    UMUM = 3
} Prioritas;

typedef struct PeminjamNode {
    char nama[MAX_NAMA];    // Nama peminjam
    Prioritas prioritas;    // Prioritas peminjam
    Buku* buku;             // Pointer ke buku yang dipinjam
    struct PeminjamNode *next;  // Pointer ke node berikutnya
} PeminjamNode;

// Struktur untuk mengelola antrian peminjam
typedef struct {
    PeminjamNode *head;  // Pointer ke node pertama dalam antrian
    PeminjamNode *tail;  // Pointer ke node terakhir dalam antrian
    int size;            // Jumlah peminjam dalam antrian
} AntrianPeminjam;

// Deklarasi fungsi-fungsi manajemen peminjam
void initAntrian(AntrianPeminjam *aq);
int enqueue(AntrianPeminjam *aq, const char *nama, Prioritas prioritas, Buku* buku);
PeminjamNode* dequeue(AntrianPeminjam *aq, Buku* buku);
void tampilkanAntrian(const AntrianPeminjam *aq);
PeminjamNode* cariPeminjamByBuku(AntrianPeminjam *aq, Buku* buku);
int hapusPeminjam(AntrianPeminjam *aq, const char *nama, Buku* buku);

#endif
