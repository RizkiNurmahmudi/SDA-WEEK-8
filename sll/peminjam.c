#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "peminjam.h"

/**
 * Fungsi untuk inisialisasi antrian peminjam
 */
void initAntrian(AntrianPeminjam *aq) {
    aq->head = NULL;
    aq->tail = NULL;
    aq->size = 0;
}

/**
 * Fungsi untuk menambahkan peminjam ke antrian (enqueue at tail)
 * Mengembalikan 1 jika berhasil, 0 jika gagal
 */
int enqueue(AntrianPeminjam *aq, const char *nama, Prioritas prioritas, Buku* buku) {
    PeminjamNode* newNode = (PeminjamNode*)malloc(sizeof(PeminjamNode));
    if (newNode == NULL) {
        return 0;
    }
    
    // Salin data peminjam
    strncpy(newNode->nama, nama, MAX_NAMA - 1);
    newNode->nama[MAX_NAMA - 1] = '\0';
    newNode->prioritas = prioritas;
    newNode->buku = buku;
    newNode->next = NULL;
    
    // Tambahkan ke antrian
    if (aq->head == NULL) {
        aq->head = newNode;
        aq->tail = newNode;
    } else {
        aq->tail->next = newNode;
        aq->tail = newNode;
    }
    
    aq->size++;
    return 1;
}

/**
 * Fungsi untuk mengeluarkan peminjam dengan prioritas tertinggi untuk buku tertentu
 * Mengembalikan pointer ke peminjam yang dikeluarkan
 */
PeminjamNode* dequeue(AntrianPeminjam *aq, Buku* buku) {
    if (aq->head == NULL) {
        return NULL;
    }
    
    PeminjamNode* highest = NULL;
    PeminjamNode* prev = NULL;
    PeminjamNode* current = aq->head;
    PeminjamNode* prevHighest = NULL;
    
    // Cari peminjam dengan prioritas tertinggi untuk buku tertentu
    while (current != NULL) {
        if (current->buku == buku) {
            if (highest == NULL || current->prioritas < highest->prioritas) {
                highest = current;
                prevHighest = prev;
            }
        }
        prev = current;
        current = current->next;
    }
    
    if (highest == NULL) {
        return NULL;
    }
    
    // Hapus node dari antrian
    if (prevHighest == NULL) {
        aq->head = highest->next;
    } else {
        prevHighest->next = highest->next;
    }
    
    // Update tail jika perlu
    if (highest == aq->tail) {
        aq->tail = prevHighest;
    }
    
    aq->size--;
    highest->next = NULL;
    return highest;
}

/**
 * Fungsi untuk menampilkan daftar peminjam dalam antrian
 */
void tampilkanAntrian(const AntrianPeminjam *aq) {
    printf("Antrian Peminjam:\n");
    
    if (aq->size == 0) {
        printf("Tidak ada peminjam dalam antrian\n");
        return;
    }
    
    PeminjamNode* current = aq->head;
    
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

/**
 * Fungsi untuk mencari peminjam berdasarkan buku
 * Mengembalikan pointer ke peminjam jika ditemukan
 */
PeminjamNode* cariPeminjamByBuku(AntrianPeminjam *aq, Buku* buku) {
    PeminjamNode* current = aq->head;
    
    while (current != NULL) {
        if (current->buku == buku) {
            return current;
        }
        current = current->next;
    }
    
    return NULL;
}

/**
 * Fungsi untuk menghapus peminjam dari antrian
 * Mengembalikan 1 jika berhasil, 0 jika gagal
 */
int hapusPeminjam(AntrianPeminjam *aq, const char *nama, Buku* buku) {
    PeminjamNode* current = aq->head;
    PeminjamNode* prev = NULL;
    
    while (current != NULL) {
        if (strcmp(current->nama, nama) == 0 && current->buku == buku) {
            if (prev == NULL) {
                aq->head = current->next;
            } else {
                prev->next = current->next;
            }
            
            if (current == aq->tail) {
                aq->tail = prev;
            }
            
            free(current);
            aq->size--;
            return 1;
        }
        prev = current;
        current = current->next;
    }
    
    return 0;
}
