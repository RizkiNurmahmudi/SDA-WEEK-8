#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "peminjam.h"

// Fungsi untuk generate ID unik
void generateID(char *id, const char *nama, const char *judul_buku) {
    time_t now;
    time(&now);
    // Format ID: nama-judul-timestamp
    snprintf(id, MAX_ID, "%s-%s-%ld", nama, judul_buku, now);
}

// Fungsi untuk inisialisasi antrian peminjam
void initAntrian(AntrianPeminjam *aq) {
    aq->size = 0;  // Set ukuran antrian ke 0
}

// Fungsi untuk menambahkan peminjam ke antrian
int enqueue(AntrianPeminjam *aq, const char *id, const char *nama, Prioritas prioritas, Buku* buku) {
    // Periksa apakah antrian penuh
    if (aq->size >= MAX_PEMINJAM) {
        return 0;  // Antrian penuh
    }
    
    // Isi data peminjam baru
    strncpy(aq->data[aq->size].id, id, MAX_ID - 1);
    aq->data[aq->size].id[MAX_ID - 1] = '\0';
    strncpy(aq->data[aq->size].nama, nama, MAX_NAMA - 1);
    aq->data[aq->size].nama[MAX_NAMA - 1] = '\0';
    aq->data[aq->size].prioritas = prioritas;
    aq->data[aq->size].buku = buku;
    
    aq->size++;  // Tambah jumlah peminjam
    return 1;    // Berhasil menambahkan
}

// Fungsi untuk mencari peminjam berdasarkan ID
Peminjam* cariPeminjam(AntrianPeminjam *aq, const char *id) {
	int i;
    for (i = 0; i < aq->size; i++) {
        if (strcmp(aq->data[i].id, id) == 0) {
            return &aq->data[i];
        }
    }
    return NULL;
}

// Fungsi untuk mengeluarkan peminjam dengan prioritas tertinggi untuk buku tertentu
Peminjam* dequeue(AntrianPeminjam *aq, Buku* buku) {
    if (aq->size == 0) {
        return NULL;  // Antrian kosong
    }
    
    int highest_index = -1;
    Prioritas highest_priority = UMUM + 1; // Nilai awal
    
    // Cari peminjam dengan prioritas tertinggi untuk buku tertentu
    int i;
    for (i = 0; i < aq->size; i++) {
        if (aq->data[i].buku == buku && aq->data[i].prioritas < highest_priority) {
            highest_priority = aq->data[i].prioritas;
            highest_index = i;
        }
    }
    
    if (highest_index == -1) {
        return NULL; // Tidak ada peminjam untuk buku ini
    }
    
    // Alokasi memori untuk peminjam yang akan dikeluarkan
    Peminjam* result = malloc(sizeof(Peminjam));
    if (result == NULL) return NULL;
    
    *result = aq->data[highest_index]; // Salin data
    
    // Geser elemen array untuk menghapus peminjam
    for (i = highest_index; i < aq->size - 1; i++) {
        aq->data[i] = aq->data[i + 1];
    }
    
    aq->size--; // Kurangi ukuran antrian
    return result;
}

// Fungsi untuk menampilkan antrian peminjam
void tampilkanAntrian(const AntrianPeminjam *aq) {
    printf("Antrian Peminjam:\n");
    
    if (aq->size == 0) {
        printf("Tidak ada peminjam dalam antrian\n");
        return;
    }
    int i;
    // Tampilkan semua peminjam dalam array
    for (i = 0; i < aq->size; i++) {
        const char* prioritas_str;
        switch (aq->data[i].prioritas) {
            case DOSEN: prioritas_str = "Dosen"; break;
            case MAHASISWA: prioritas_str = "Mahasiswa"; break;
            case UMUM: prioritas_str = "Umum"; break;
            default: prioritas_str = "Tidak diketahui";
        }
        
        printf("- ID: %s, Nama: %s (Prioritas: %s) - Buku: %s\n",
               aq->data[i].id,
               aq->data[i].nama,
               prioritas_str,
               aq->data[i].buku->judul);
    }
}

// Fungsi untuk menghapus peminjam dari antrian berdasarkan ID
int hapusPeminjam(AntrianPeminjam *aq, const char *id) {
    int found_index = -1;
    
    // Cari indeks peminjam berdasarkan ID
    int i;
    for (i = 0; i < aq->size; i++) {
        if (strcmp(aq->data[i].id, id) == 0) {
            found_index = i;
            break;
        }
    }
    
    if (found_index == -1) {
        return 0; // Peminjam tidak ditemukan
    }
    
    // Geser elemen array untuk menghapus peminjam
    for (i = found_index; i < aq->size - 1; i++) {
        aq->data[i] = aq->data[i + 1];
    }
    
    aq->size--; // Kurangi ukuran antrian
    return 1;   // Berhasil menghapus
}
