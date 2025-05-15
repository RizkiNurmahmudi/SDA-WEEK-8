#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "perpustakaan.h"

void initPerpustakaan(Perpustakaan *p) {
    initBuku(&p->db);      // Inisialisasi daftar buku
    initAntrian(&p->aq);   // Inisialisasi antrian peminjam
    p->history_count = 0;  // Inisialisasi jumlah history
}

void tambahBukuPerpus(Perpustakaan *p, const char *judul, int stok) {
    if (!tambahBuku(&p->db, judul, stok)) {
        printf("Gagal menambah buku. Mungkin stok penuh.\n");
        return;
    }
    
    // Catat aktivitas di history
    if (p->history_count < MAX_HISTORY) {
        snprintf(p->history[p->history_count].aksi, 50, "Tambah buku");
        strncpy(p->history[p->history_count].buku, judul, MAX_JUDUL - 1);
        p->history[p->history_count].buku[MAX_JUDUL - 1] = '\0';
        p->history[p->history_count].prioritas = UMUM;
        p->history_count++;
    }
    printf("Buku '%s' berhasil ditambahkan dengan stok %d\n", judul, stok);
}

/**
 * Fungsi untuk mendaftarkan peminjam
 * Mencari buku terlebih dahulu, lalu menambahkan ke antrian
 */
void daftarPeminjam(Perpustakaan *p, const char *nama, Prioritas prioritas, const char *judul_buku) {
    // Cari buku yang ingin dipinjam
    Buku* buku = cariBuku(&p->db, judul_buku);
    if (buku == NULL) {
        printf("Buku '%s' tidak ditemukan!\n", judul_buku);
        return;
    }
    
    // Tambahkan peminjam ke antrian
    if (!enqueue(&p->aq, nama, prioritas, buku)) {
        printf("Antrian peminjam penuh!\n");
        return;
    }
    
    // Catat aktivitas di history
    if (p->history_count < MAX_HISTORY) {
        snprintf(p->history[p->history_count].aksi, 50, "Daftar pinjam");
        strncpy(p->history[p->history_count].nama, nama, MAX_NAMA - 1);
        p->history[p->history_count].nama[MAX_NAMA - 1] = '\0';
        strncpy(p->history[p->history_count].buku, judul_buku, MAX_JUDUL - 1);
        p->history[p->history_count].buku[MAX_JUDUL - 1] = '\0';
        p->history[p->history_count].prioritas = prioritas;
        p->history_count++;
    }
    
    printf("%s berhasil didaftarkan untuk meminjam %s (Prioritas: %d)\n", 
           nama, judul_buku, prioritas);
}

/**
 * Fungsi untuk memproses peminjaman buku
 * Mengeluarkan peminjam dengan prioritas tertinggi dari antrian
 */
void prosesPeminjaman(Perpustakaan *p, const char *judul_buku) {
    // Cari buku yang ingin dipinjam
    Buku* buku = cariBuku(&p->db, judul_buku);
    if (buku == NULL) {
        printf("Buku '%s' tidak ditemukan!\n", judul_buku);
        return;
    }
    
    // Periksa ketersediaan stok
    if (buku->stok_tersedia <= 0) {
        printf("Stok buku '%s' habis!\n", judul_buku);
        return;
    }
    
    // Keluarkan peminjam dengan prioritas tertinggi untuk buku ini
    PeminjamNode* peminjam = dequeue(&p->aq, buku);
    if (peminjam == NULL) {
        printf("Tidak ada peminjam yang mengantri untuk buku '%s'\n", judul_buku);
        return;
    }
    
    // Proses peminjaman
    if (!pinjamBuku(&p->db, judul_buku)) {
        printf("Gagal memproses peminjaman buku '%s'\n", judul_buku);
        // Kembalikan peminjam ke antrian jika gagal
        enqueue(&p->aq, peminjam->nama, peminjam->prioritas, peminjam->buku);
        free(peminjam);
        return;
    }
    
    // Catat aktivitas di history
    if (p->history_count < MAX_HISTORY) {
        snprintf(p->history[p->history_count].aksi, 50, "Pinjam buku");
        strncpy(p->history[p->history_count].nama, peminjam->nama, MAX_NAMA - 1);
        p->history[p->history_count].nama[MAX_NAMA - 1] = '\0';
        strncpy(p->history[p->history_count].buku, judul_buku, MAX_JUDUL - 1);
        p->history[p->history_count].buku[MAX_JUDUL - 1] = '\0';
        p->history[p->history_count].prioritas = peminjam->prioritas;
        p->history_count++;
    }
    
    // Tampilkan pesan sukses
    const char* prioritas_str;
    switch (peminjam->prioritas) {
        case DOSEN: prioritas_str = "Dosen"; break;
        case MAHASISWA: prioritas_str = "Mahasiswa"; break;
        case UMUM: prioritas_str = "Umum"; break;
        default: prioritas_str = "Tidak diketahui";
    }
    
    printf("%s (Prioritas: %s) berhasil meminjam buku %s\n", 
           peminjam->nama, prioritas_str, judul_buku);
    free(peminjam);
}

/**
 * Fungsi untuk memproses pengembalian buku
 * Meningkatkan stok tersedia buku yang dikembalikan
 */
void prosesPengembalian(Perpustakaan *p, const char *nama, const char *judul_buku) {
    // Cari buku yang dikembalikan
    Buku* buku = cariBuku(&p->db, judul_buku);
    if (buku == NULL) {
        printf("Buku '%s' tidak ditemukan!\n", judul_buku);
        return;
    }
    
    // Periksa apakah stok sudah penuh
    if (buku->stok_tersedia >= buku->stok) {
        printf("Stok buku '%s' sudah penuh, tidak bisa menerima pengembalian\n", judul_buku);
        return;
    }
    
    // Proses pengembalian
    if (!kembalikanBuku(&p->db, judul_buku)) {
        printf("Gagal memproses pengembalian buku '%s'\n", judul_buku);
        return;
    }
    
    // Catat aktivitas di history
    if (p->history_count < MAX_HISTORY) {
        snprintf(p->history[p->history_count].aksi, 50, "Kembalikan buku");
        strncpy(p->history[p->history_count].nama, nama, MAX_NAMA - 1);
        p->history[p->history_count].nama[MAX_NAMA - 1] = '\0';
        strncpy(p->history[p->history_count].buku, judul_buku, MAX_JUDUL - 1);
        p->history[p->history_count].buku[MAX_JUDUL - 1] = '\0';
        p->history[p->history_count].prioritas = UMUM;
        p->history_count++;
    }
    
    printf("%s berhasil mengembalikan buku %s\n", nama, judul_buku);
}

/**
 * Fungsi untuk menampilkan status perpustakaan
 * Menampilkan daftar buku, antrian peminjam, dan history terakhir
 */
void tampilkanStatusPerpustakaan(const Perpustakaan *p) {
    printf("\n=== STATUS PERPUSTAKAAN ===\n");
    
    // Tampilkan daftar buku
    printf("\n[Daftar Buku]\n");
    if (p->db.jumlah == 0) {
        printf("Tidak ada buku terdaftar\n");
    } else {
        Buku* current = p->db.head;
        while (current != NULL) {
            printf("- %s (Stok: %d, Tersedia: %d)\n",
                   current->judul,
                   current->stok,
                   current->stok_tersedia);
            current = current->next;
        }
    }
    
    // Tampilkan antrian peminjam
    printf("\n[Antrian Peminjam]\n");
    if (p->aq.size == 0) {
        printf("Tidak ada antrian peminjam\n");
    } else {
        PeminjamNode* current = p->aq.head;
        while (current != NULL) {
            const char* prioritas_str;
            switch (current->prioritas) {
                case DOSEN: prioritas_str = "Dosen"; break;
                case MAHASISWA: prioritas_str = "Mahasiswa"; break;
                case UMUM: prioritas_str = "Umum"; break;
                default: prioritas_str = "Tidak diketahui";
            }
            
            printf("- %s (Prioritas: %s) - Buku: %s\n",
                   current->nama,
                   prioritas_str,
                   current->buku->judul);
            current = current->next;
        }
    }
    
    // Tampilkan history terakhir
    printf("\n[History Terakhir]\n");
    if (p->history_count == 0) {
        printf("Tidak ada history\n");
    } else {
        int start = (p->history_count > 5) ? p->history_count - 5 : 0;
        int i;
        for (i = start; i < p->history_count; i++) {
            printf("- %s: %s - %s\n",
                   p->history[i].aksi,
                   p->history[i].nama,
                   p->history[i].buku);
        }
    }
    
    printf("\n");
}
