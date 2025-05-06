#include <stdio.h>
#include <string.h>
#include "perpustakaan.h"

void initPerpustakaan(Perpustakaan *p) {
    initBuku(&p->db);
    initAntrian(&p->aq);
    p->history_count = 0;
}

void tambahBukuPerpus(Perpustakaan *p, const char *judul, int stok) {
    tambahBuku(&p->db, judul, stok);
    
    // Catat history
    if (p->history_count < MAX_HISTORY) {
        snprintf(p->history[p->history_count].aksi, 50, "Tambah buku");
        strncpy(p->history[p->history_count].buku, judul, MAX_JUDUL - 1);
        p->history[p->history_count].buku[MAX_JUDUL - 1] = '\0';
        p->history_count++;
    }
}

void daftarPeminjam(Perpustakaan *p, const char *nama, Prioritas prioritas, const char *buku) {
    enqueue(&p->aq, nama, prioritas, buku);
    
    // Catat history
    if (p->history_count < MAX_HISTORY) {
        snprintf(p->history[p->history_count].aksi, 50, "Daftar pinjam");
        strncpy(p->history[p->history_count].nama, nama, MAX_NAMA - 1);
        p->history[p->history_count].nama[MAX_NAMA - 1] = '\0';
        strncpy(p->history[p->history_count].buku, buku, MAX_JUDUL - 1);
        p->history[p->history_count].buku[MAX_JUDUL - 1] = '\0';
        p->history_count++;
    }
}

void prosesPeminjaman(Perpustakaan *p, const char *buku) {
    Buku* b = cariBuku(&p->db, buku);
    if (b == NULL || b->stok_tersedia <= 0) {
        printf("Tidak ada stok buku %s yang tersedia\n", buku);
        return;
    }
    
    Peminjam* peminjam = dequeue(&p->aq);
    if (peminjam == NULL) {
        printf("Tidak ada peminjam untuk buku %s\n", buku);
        return;
    }
    
    if (strcmp(peminjam->buku_dipinjam, buku) != 0) {
        printf("Peminjam tidak sesuai dengan buku yang diminta\n");
        return;
    }
    
    pinjamBuku(&p->db, buku);
    
    // Catat history
    if (p->history_count < MAX_HISTORY) {
        snprintf(p->history[p->history_count].aksi, 50, "Pinjam buku");
        strncpy(p->history[p->history_count].nama, peminjam->nama, MAX_NAMA - 1);
        p->history[p->history_count].nama[MAX_NAMA - 1] = '\0';
        strncpy(p->history[p->history_count].buku, buku, MAX_JUDUL - 1);
        p->history[p->history_count].buku[MAX_JUDUL - 1] = '\0';
        p->history_count++;
    }
    
    printf("%s (Prioritas: %d) berhasil meminjam buku %s\n", 
           peminjam->nama, peminjam->prioritas, buku);
}

void prosesPengembalian(Perpustakaan *p, const char *nama, const char *buku) {
    if (kembalikanBuku(&p->db, buku)) {
        // Catat history
        if (p->history_count < MAX_HISTORY) {
            snprintf(p->history[p->history_count].aksi, 50, "Kembalikan buku");
            strncpy(p->history[p->history_count].nama, nama, MAX_NAMA - 1);
            p->history[p->history_count].nama[MAX_NAMA - 1] = '\0';
            strncpy(p->history[p->history_count].buku, buku, MAX_JUDUL - 1);
            p->history[p->history_count].buku[MAX_JUDUL - 1] = '\0';
            p->history_count++;
        }
        
        printf("%s berhasil mengembalikan buku %s\n", nama, buku);
        
        // Proses peminjaman berikutnya jika ada antrian
        Peminjam* next = cariPeminjamByBuku(&p->aq, buku);
        if (next != NULL) {
            printf("Memproses peminjaman berikutnya untuk buku %s...\n", buku);
            prosesPeminjaman(p, buku);
        }
    } else {
        printf("Gagal mengembalikan buku %s\n", buku);
    }
}

void batalkanAktivitasTerakhir(Perpustakaan *p) {
    if (p->history_count <= 0) {
        printf("Tidak ada aktivitas untuk dibatalkan\n");
        return;
    }
    
    History last = p->history[--p->history_count];
    
    if (strcmp(last.aksi, "Daftar pinjam") == 0) {
        hapusPeminjam(&p->aq, last.nama, last.buku);
        printf("Pendaftaran peminjaman %s oleh %s dibatalkan\n", last.buku, last.nama);
    } else if (strcmp(last.aksi, "Pinjam buku") == 0) {
        enqueue(&p->aq, last.nama, DOSEN, last.buku); // Asumsi prioritas dosen
        kembalikanBuku(&p->db, last.buku);
        printf("Peminjaman %s oleh %s dibatalkan\n", last.buku, last.nama);
    } else if (strcmp(last.aksi, "Kembalikan buku") == 0) {
        pinjamBuku(&p->db, last.buku);
        printf("Pengembalian %s oleh %s dibatalkan\n", last.buku, last.nama);
    } else {
        printf("Aktivitas terakhir tidak dapat dibatalkan\n");
    }
}

void tampilkanStatusPerpustakaan(const Perpustakaan *p) {
    printf("\n=== Status Perpustakaan ===\n");
    tampilkanBuku(&p->db);
    tampilkanAntrian(&p->aq);
    printf("\n");
}
