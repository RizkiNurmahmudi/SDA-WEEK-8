#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "perpustakaan.h"

// Fungsi untuk inisialisasi perpustakaan
void initPerpustakaan(Perpustakaan *p) {
    initBuku(&p->db);      // Inisialisasi daftar buku
    initAntrian(&p->aq);   // Inisialisasi antrian peminjam
    p->history_count = 0;  // Inisialisasi jumlah history
}

// Fungsi untuk menambah buku ke perpustakaan
void tambahBukuPerpus(Perpustakaan *p, const char *judul, int stok) {
    if (!tambahBuku(&p->db, judul, stok)) {
        printf("Gagal menambah buku. Mungkin daftar buku penuh atau buku sudah ada.\n");
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

// Fungsi untuk mendaftarkan peminjam
void daftarPeminjam(Perpustakaan *p, const char *nama, Prioritas prioritas, const char *judul_buku) {
    // Cari buku yang ingin dipinjam
    Buku* buku = cariBuku(&p->db, judul_buku);
    if (buku == NULL) {
        printf("Buku '%s' tidak ditemukan!\n", judul_buku);
        return;
    }
    
    // Generate ID unik untuk peminjam
    char id[MAX_ID];
    generateID(id, nama, judul_buku);
    
    // Tambahkan peminjam ke antrian
    if (!enqueue(&p->aq, id, nama, prioritas, buku)) {
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
    
    printf("%s (ID: %s) berhasil didaftarkan untuk meminjam %s (Prioritas: %d)\n", 
           nama, id, judul_buku, prioritas);
}

// Fungsi untuk memproses peminjaman buku
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
    Peminjam* peminjam = dequeue(&p->aq, buku);
    if (peminjam == NULL) {
        printf("Tidak ada peminjam yang mengantri untuk buku '%s'\n", judul_buku);
        return;
    }
    
    // Proses peminjaman
    if (!pinjamBuku(&p->db, judul_buku)) {
        printf("Gagal memproses peminjaman buku '%s'\n", judul_buku);
        // Kembalikan peminjam ke antrian jika gagal
        enqueue(&p->aq, peminjam->id, peminjam->nama, peminjam->prioritas, peminjam->buku);
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
    
    printf("%s (ID: %s, Prioritas: %s) berhasil meminjam buku %s\n", 
           peminjam->nama, peminjam->id, prioritas_str, judul_buku);
    free(peminjam);
}

// Fungsi untuk memproses pengembalian buku
void prosesPengembalian(Perpustakaan *p, const char *id) {
    // Cari peminjam berdasarkan ID
    Peminjam* peminjam = cariPeminjam(&p->aq, id);
    if (peminjam == NULL) {
        printf("Peminjam dengan ID %s tidak ditemukan!\n", id);
        return;
    }
    
    // Proses pengembalian buku
    if (!kembalikanBuku(&p->db, peminjam->buku->judul)) {
        printf("Gagal memproses pengembalian buku\n");
        return;
    }
    
    // Catat aktivitas di history
    if (p->history_count < MAX_HISTORY) {
        snprintf(p->history[p->history_count].aksi, 50, "Kembalikan buku");
        strncpy(p->history[p->history_count].nama, peminjam->nama, MAX_NAMA - 1);
        p->history[p->history_count].nama[MAX_NAMA - 1] = '\0';
        strncpy(p->history[p->history_count].buku, peminjam->buku->judul, MAX_JUDUL - 1);
        p->history[p->history_count].buku[MAX_JUDUL - 1] = '\0';
        p->history[p->history_count].prioritas = peminjam->prioritas;
        p->history_count++;
    }
    
    printf("%s (ID: %s) berhasil mengembalikan buku %s\n", 
           peminjam->nama, id, peminjam->buku->judul);
}

// Fungsi untuk membatalkan antrian peminjam
void batalkanAntrian(Perpustakaan *p, const char *id) {
    // Cari peminjam berdasarkan ID
    Peminjam* peminjam = cariPeminjam(&p->aq, id);
    if (peminjam == NULL) {
        printf("Peminjam dengan ID %s tidak ditemukan!\n", id);
        return;
    }
    
    // Hapus peminjam dari antrian
    if (hapusPeminjam(&p->aq, id)) {
        printf("Antrian peminjam %s (ID: %s) untuk buku %s berhasil dibatalkan\n", 
               peminjam->nama, id, peminjam->buku->judul);
        
        // Catat aktivitas di history
        if (p->history_count < MAX_HISTORY) {
            snprintf(p->history[p->history_count].aksi, 50, "Batalkan antrian");
            strncpy(p->history[p->history_count].nama, peminjam->nama, MAX_NAMA - 1);
            p->history[p->history_count].nama[MAX_NAMA - 1] = '\0';
            strncpy(p->history[p->history_count].buku, peminjam->buku->judul, MAX_JUDUL - 1);
            p->history[p->history_count].buku[MAX_JUDUL - 1] = '\0';
            p->history[p->history_count].prioritas = peminjam->prioritas;
            p->history_count++;
        }
    }
}

// Fungsi untuk menampilkan status perpustakaan
void tampilkanStatusPerpustakaan(const Perpustakaan *p) {
    printf("\n=== STATUS PERPUSTAKAAN ===\n");
    
    // Tampilkan daftar buku
    int i;
    printf("\n[Daftar Buku]\n");
    if (p->db.jumlah == 0) {
        printf("Tidak ada buku terdaftar\n");
    } else {
        for (i = 0; i < p->db.jumlah; i++) {
            printf("- %s (Stok: %d, Tersedia: %d)\n",
                   p->db.data[i].judul,
                   p->db.data[i].stok,
                   p->db.data[i].stok_tersedia);
        }
    }
    
    // Tampilkan antrian peminjam
    printf("\n[Antrian Peminjam]\n");
    if (p->aq.size == 0) {
        printf("Tidak ada antrian peminjam\n");
    } else {
        for (i = 0; i < p->aq.size; i++) {
            const char* prioritas_str;
            switch (p->aq.data[i].prioritas) {
                case DOSEN: prioritas_str = "Dosen"; break;
                case MAHASISWA: prioritas_str = "Mahasiswa"; break;
                case UMUM: prioritas_str = "Umum"; break;
                default: prioritas_str = "Tidak diketahui";
            }
            
            printf("- ID: %s, Nama: %s (Prioritas: %s) - Buku: %s\n",
                   p->aq.data[i].id,
                   p->aq.data[i].nama,
                   prioritas_str,
                   p->aq.data[i].buku->judul);
        }
    }
    
    // Tampilkan history terakhir
    printf("\n[History Terakhir]\n");
    if (p->history_count == 0) {
        printf("Tidak ada history\n");
    } else {
        int start = (p->history_count > 5) ? p->history_count - 5 : 0;
        for (i = start; i < p->history_count; i++) {
            printf("- %s: %s - %s\n",
                   p->history[i].aksi,
                   p->history[i].nama,
                   p->history[i].buku);
        }
    }
    
    printf("\n");
}
