#ifndef PERPUSTAKAAN_H
#define PERPUSTAKAAN_H

#include "buku.h"
#include "peminjam.h"

#define MAX_HISTORY 100

typedef struct {
    char aksi[50];
    char nama[MAX_NAMA];
    char buku[MAX_JUDUL];
} History;

typedef struct {
    DaftarBuku db;
    AntrianPeminjam aq;
    History history[MAX_HISTORY];
    int history_count;
} Perpustakaan;

void initPerpustakaan(Perpustakaan *p);
void tambahBukuPerpus(Perpustakaan *p, const char *judul, int stok);
void daftarPeminjam(Perpustakaan *p, const char *nama, Prioritas prioritas, const char *buku);
void prosesPeminjaman(Perpustakaan *p, const char *buku);
void prosesPengembalian(Perpustakaan *p, const char *nama, const char *buku);
void batalkanAktivitasTerakhir(Perpustakaan *p);
void tampilkanStatusPerpustakaan(const Perpustakaan *p);

#endif
