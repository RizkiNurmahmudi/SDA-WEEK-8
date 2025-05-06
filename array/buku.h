#ifndef BUKU_H
#define BUKU_H

#define MAX_JUDUL 100
#define MAX_BUKU 100

typedef struct {
    char judul[MAX_JUDUL];
    int stok;
    int stok_tersedia;
} Buku;

typedef struct {
    Buku daftar[MAX_BUKU];
    int jumlah;
} DaftarBuku;

void initBuku(DaftarBuku *db);
int tambahBuku(DaftarBuku *db, const char *judul, int stok);
Buku* cariBuku(DaftarBuku *db, const char *judul);
void tampilkanBuku(const DaftarBuku *db);
int pinjamBuku(DaftarBuku *db, const char *judul);
int kembalikanBuku(DaftarBuku *db, const char *judul);

#endif
