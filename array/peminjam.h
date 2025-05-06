#ifndef PEMINJAM_H
#define PEMINJAM_H

#define MAX_NAMA 100
#define MAX_ANTRIAN 100

typedef enum {
    MAHASISWA,
    DOSEN,
    UMUM
} Prioritas;

typedef struct {
    char nama[MAX_NAMA];
    Prioritas prioritas;
    char buku_dipinjam[MAX_NAMA];
} Peminjam;

typedef struct {
    Peminjam antrian[MAX_ANTRIAN];
    int depan;
    int belakang;
} AntrianPeminjam;

void initAntrian(AntrianPeminjam *aq);
int enqueue(AntrianPeminjam *aq, const char *nama, Prioritas prioritas, const char *buku);
Peminjam* dequeue(AntrianPeminjam *aq);
void tampilkanAntrian(const AntrianPeminjam *aq);
Peminjam* cariPeminjamByBuku(AntrianPeminjam *aq, const char *buku);
int hapusPeminjam(AntrianPeminjam *aq, const char *nama, const char *buku);

#endif
