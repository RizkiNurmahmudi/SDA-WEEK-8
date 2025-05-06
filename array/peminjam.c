#include <stdio.h>
#include <string.h>
#include "peminjam.h"

void initAntrian(AntrianPeminjam *aq) {
    aq->depan = 0;
    aq->belakang = 0;
}

int enqueue(AntrianPeminjam *aq, const char *nama, Prioritas prioritas, const char *buku) {
    if (aq->belakang >= MAX_ANTRIAN) {
        return 0; // Gagal, antrian penuh
    }
    
    strncpy(aq->antrian[aq->belakang].nama, nama, MAX_NAMA - 1);
    aq->antrian[aq->belakang].nama[MAX_NAMA - 1] = '\0';
    aq->antrian[aq->belakang].prioritas = prioritas;
    strncpy(aq->antrian[aq->belakang].buku_dipinjam, buku, MAX_NAMA - 1);
    aq->antrian[aq->belakang].buku_dipinjam[MAX_NAMA - 1] = '\0';
    aq->belakang++;
    
    return 1; // Sukses
}

Peminjam* dequeue(AntrianPeminjam *aq) {
    if (aq->depan >= aq->belakang) {
        return NULL; // Antrian kosong
    }
    
    // Cari peminjam dengan prioritas tertinggi
    int highest_priority = -1;
    int highest_index = -1;
    int i;
    
    for (i = aq->depan; i < aq->belakang; i++) {
        if (aq->antrian[i].prioritas > highest_priority) {
            highest_priority = aq->antrian[i].prioritas;
            highest_index = i;
        }
    }
    
    if (highest_index == -1) {
        return NULL;
    }
    
    // Tukar dengan elemen depan
    Peminjam temp = aq->antrian[aq->depan];
    aq->antrian[aq->depan] = aq->antrian[highest_index];
    aq->antrian[highest_index] = temp;
    
    // Kembalikan elemen depan dan increment depan
    return &aq->antrian[aq->depan++];
}

void tampilkanAntrian(const AntrianPeminjam *aq) {
    printf("Antrian Peminjam:\n");
    
    int i;
    for (i = aq->depan; i < aq->belakang; i++) {
        const char* prioritas_str;
        switch (aq->antrian[i].prioritas) {
            case DOSEN: prioritas_str = "Dosen"; break;
            case MAHASISWA: prioritas_str = "Mahasiswa"; break;
            case UMUM: prioritas_str = "Umum"; break;
            default: prioritas_str = "Unknown";
        }
        printf("- %s (Prioritas: %s) - Buku: %s\n", 
               aq->antrian[i].nama, 
               prioritas_str,
               aq->antrian[i].buku_dipinjam);
    }
}

Peminjam* cariPeminjamByBuku(AntrianPeminjam *aq, const char *buku) {
	int i;
    for (i = aq->depan; i < aq->belakang; i++) {
        if (strcmp(aq->antrian[i].buku_dipinjam, buku) == 0) {
            return &aq->antrian[i];
        }
    }
    return NULL;
}

int hapusPeminjam(AntrianPeminjam *aq, const char *nama, const char *buku) {
	int i;
    for (i = aq->depan; i < aq->belakang; i++) {
        if (strcmp(aq->antrian[i].nama, nama) == 0 && 
            strcmp(aq->antrian[i].buku_dipinjam, buku) == 0) {
            // Geser elemen setelahnya ke depan
            int j;
            for (j = i; j < aq->belakang - 1; j++) {
                aq->antrian[j] = aq->antrian[j + 1];
            }
            aq->belakang--;
            return 1; // Sukses
        }
    }
    return 0; // Gagal, tidak ditemukan
}
