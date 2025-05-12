#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "peminjam.h"

// Fungsi untuk inisialisasi antrian peminjam
void initAntrian(AntrianPeminjam *aq) {
    aq->head = NULL;  // Set head ke NULL (antrian kosong)
    aq->tail = NULL;  // Set tail ke NULL (antrian kosong)
    aq->size = 0;     // Inisialisasi ukuran antrian ke 0
}

// Fungsi untuk menambahkan peminjam ke antrian
int enqueue(AntrianPeminjam *aq, const char *nama, Prioritas prioritas, Buku* buku) {
    // Alokasi memori untuk peminjam baru
    PeminjamNode* newNode = (PeminjamNode*)malloc(sizeof(PeminjamNode));
    if (newNode == NULL) {
        return 0;  // Mengembalikan 0 jika alokasi gagal
    }
    
    // Mengisi data peminjam baru
    strncpy(newNode->nama, nama, MAX_NAMA - 1);  // Salin nama peminjam
    newNode->nama[MAX_NAMA - 1] = '\0';          // Pastikan string diakhiri NULL
    newNode->prioritas = prioritas;              // Set prioritas peminjam
    newNode->buku = buku;                        // Set pointer ke buku
    newNode->prev = NULL;                        // Inisialisasi pointer prev
    newNode->next = NULL;                        // Inisialisasi pointer next
    
    // Menambahkan peminjam ke antrian
    if (aq->head == NULL) {
        // Jika antrian kosong, peminjam baru menjadi head dan tail
        aq->head = newNode;
        aq->tail = newNode;
    } else {
        // Jika antrian tidak kosong, tambahkan di akhir
        aq->tail->next = newNode;  // Link dari tail ke peminjam baru
        newNode->prev = aq->tail;  // Link prev peminjam baru ke tail lama
        aq->tail = newNode;        // Update tail ke peminjam baru
    }
    
    aq->size++;  // Increment ukuran antrian
    return 1;    // Mengembalikan 1 untuk menandakan sukses
}

// Fungsi untuk mengeluarkan peminjam dengan prioritas tertinggi untuk buku tertentu
PeminjamNode* dequeue(AntrianPeminjam *aq, Buku* buku) {
    if (aq->head == NULL) {
        return NULL;  // Mengembalikan NULL jika antrian kosong
    }
    
    // Variabel untuk menyimpan peminjam dengan prioritas tertinggi
    PeminjamNode* highest = NULL;
    PeminjamNode* current = aq->head;
    
    // Iterasi untuk mencari peminjam dengan prioritas tertinggi untuk buku tertentu
    while (current != NULL) {
        if (current->buku == buku) {
            if (highest == NULL || current->prioritas < highest->prioritas) {
                highest = current;  // Update highest jika ditemukan prioritas lebih tinggi
            }
        }
        current = current->next;
    }
    
    if (highest == NULL) {
        return NULL;  // Tidak ada peminjam untuk buku ini
    }
    
    // Hapus peminjam dari antrian
    if (highest->prev != NULL) {
        highest->prev->next = highest->next;
    } else {
        aq->head = highest->next;
    }
    
    if (highest->next != NULL) {
        highest->next->prev = highest->prev;
    } else {
        aq->tail = highest->prev;
    }
    
    aq->size--;           // Decrement ukuran antrian
    highest->prev = NULL; // Putus link prev
    highest->next = NULL; // Putus link next
    return highest;       // Mengembalikan peminjam yang dikeluarkan
}

// Fungsi untuk menampilkan daftar peminjam dalam antrian
void tampilkanAntrian(const AntrianPeminjam *aq) {
    printf("Antrian Peminjam:\n");
    
    if (aq->size == 0) {
        printf("Tidak ada peminjam dalam antrian\n");
        return;
    }
    
    PeminjamNode* current = aq->head;
    
    // Iterasi dan tampilkan semua peminjam
    while (current != NULL) {
        const char* prioritas_str;
        // Konversi prioritas ke string
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

// Fungsi untuk mencari peminjam berdasarkan buku
PeminjamNode* cariPeminjamByBuku(AntrianPeminjam *aq, Buku* buku) {
    PeminjamNode* current = aq->head;
    
    // Iterasi untuk mencari peminjam yang meminjam buku tertentu
    while (current != NULL) {
        if (current->buku == buku) {
            return current;  // Mengembalikan peminjam jika ditemukan
        }
        current = current->next;
    }
    
    return NULL;  // Mengembalikan NULL jika tidak ditemukan
}

// Fungsi untuk menghapus peminjam dari antrian
int hapusPeminjam(AntrianPeminjam *aq, const char *nama, Buku* buku) {
    PeminjamNode* current = aq->head;
    
    // Iterasi untuk mencari peminjam yang akan dihapus
    while (current != NULL) {
        if (strcmp(current->nama, nama) == 0 && current->buku == buku) {
            // Hapus peminjam dari antrian
            if (current->prev != NULL) {
                current->prev->next = current->next;
            } else {
                aq->head = current->next;
            }
            
            if (current->next != NULL) {
                current->next->prev = current->prev;
            } else {
                aq->tail = current->prev;
            }
            
            free(current);  // Bebaskan memori peminjam
            aq->size--;     // Decrement ukuran antrian
            return 1;       // Mengembalikan 1 untuk menandakan sukses
        }
        current = current->next;
    }
    
    return 0;  // Mengembalikan 0 jika peminjam tidak ditemukan
}
