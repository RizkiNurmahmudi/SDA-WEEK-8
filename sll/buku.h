#ifndef BUKU_H
#define BUKU_H

#define MAX_JUDUL 100

typedef struct Buku {
    char judul[MAX_JUDUL];  // Menyimpan judul buku
    int stok;               // Jumlah total stok buku
    int stok_tersedia;      // Jumlah stok yang tersedia untuk dipinjam
    struct Buku *next;      // Pointer ke buku berikutnya
} Buku;

// Struktur untuk mengelola daftar buku
typedef struct {
    Buku *head;     // Pointer ke buku pertama dalam daftar
    int jumlah;     // Jumlah total buku dalam daftar
} DaftarBuku;

// Deklarasi fungsi-fungsi manajemen buku
void initBuku(DaftarBuku *db);
int tambahBuku(DaftarBuku *db, const char *judul, int stok);
Buku* cariBuku(DaftarBuku *db, const char *judul);
void tampilkanBuku(const DaftarBuku *db);
int pinjamBuku(DaftarBuku *db, const char *judul);
int kembalikanBuku(DaftarBuku *db, const char *judul);
void freeBuku(DaftarBuku *db);

#endif
