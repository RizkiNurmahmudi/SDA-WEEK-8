#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "perpustakaan.h"
#include "menu.h"

// Fungsi untuk membersihkan layar
void clearScreen() {
#ifdef _WIN32
    system("cls");  // Untuk Windows
#else
    system("clear");  // Untuk Unix/Linux
#endif
}

// Fungsi untuk membersihkan buffer input
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Fungsi untuk menampilkan status perpustakaan
void tampilkanDisplay(const Perpustakaan *perpus) {
    printf("=== SISTEM PERPUSTAKAAN ===\n\n");
    
    // Tampilkan daftar buku
    printf("[Daftar Buku]\n");
    if (perpus->db.jumlah == 0) {
        printf("Belum ada buku terdaftar\n");
    } else {
        Buku* current = perpus->db.head;
        while (current != NULL) {
            printf("- %s (Total: %d, Tersedia: %d)\n",
                   current->judul,
                   current->stok,
                   current->stok_tersedia);
            current = current->next;
        }
    }
    
    // Tampilkan antrian peminjam
    printf("\n[Antrian Peminjam]\n");
    if (perpus->aq.size == 0) {
        printf("Tidak ada antrian peminjam\n");
    } else {
        PeminjamNode* current = perpus->aq.head;
        while (current != NULL) {
            const char *prioritas_str;
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
    printf("\n");
}

// Fungsi untuk menampilkan menu utama
void tampilkanMenu() {
    printf("=== MENU UTAMA ===\n");
    printf("1. Tambah Buku Baru\n");
    printf("2. Daftarkan Peminjam\n");
    printf("3. Proses Peminjaman Buku\n");
    printf("4. Proses Pengembalian Buku\n");
    printf("5. Batalkan Antrian Peminjam\n");
    printf("0. Keluar\n");
    printf("Pilihan: ");
}

// Fungsi untuk memproses pilihan menu
void prosesMenu(Perpustakaan *perpus) {
    int pilihan;
    char judul[MAX_JUDUL], nama[MAX_NAMA];
    int stok, prioritas;

    do {
        clearScreen();
        tampilkanDisplay(perpus);
        tampilkanMenu();
        
        // Baca pilihan menu
        if (scanf("%d", &pilihan) != 1) {
            printf("Input tidak valid! Harap masukkan angka.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        
        clearScreen();
        tampilkanDisplay(perpus);

        switch (pilihan) {
            case 1: { // Tambah Buku Baru
                printf("Masukkan judul buku: ");
                if (fgets(judul, sizeof(judul), stdin) == NULL) {
                    printf("Error membaca input\n");
                    break;
                }
                judul[strcspn(judul, "\n")] = '\0';

                printf("Masukkan jumlah stok: ");
                if (scanf("%d", &stok) != 1 || stok <= 0) {
                    printf("Stok harus bilangan positif!\n");
                    clearInputBuffer();
                    break;
                }
                clearInputBuffer();

                tambahBukuPerpus(perpus, judul, stok);
                break;
            }
                
            case 2: { // Daftarkan Peminjam
                printf("Masukkan nama peminjam: ");
                if (fgets(nama, sizeof(nama), stdin) == NULL) {
                    printf("Error membaca input\n");
                    break;
                }
                nama[strcspn(nama, "\n")] = '\0';

                printf("Masukkan judul buku yang ingin dipinjam: ");
                if (fgets(judul, sizeof(judul), stdin) == NULL) {
                    printf("Error membaca input\n");
                    break;
                }
                judul[strcspn(judul, "\n")] = '\0';

                printf("Masukkan prioritas (1=Dosen, 2=Mahasiswa, 3=Umum): ");
                if (scanf("%d", &prioritas) != 1) {
                    printf("Input tidak valid!\n");
                    clearInputBuffer();
                    break;
                }
                clearInputBuffer();

                if (prioritas < 1 || prioritas > 3) {
                    printf("Prioritas harus antara 1-3!\n");
                    break;
                }

                daftarPeminjam(perpus, nama, (Prioritas)prioritas, judul);
                break;
            }
                
            case 3: { // Proses Peminjaman Buku
                printf("Masukkan judul buku yang akan dipinjam: ");
                if (fgets(judul, sizeof(judul), stdin) == NULL) {
                    printf("Error membaca input\n");
                    break;
                }
                judul[strcspn(judul, "\n")] = '\0';

                prosesPeminjaman(perpus, judul);
                break;
            }
                
            case 4: { // Proses Pengembalian Buku
                printf("Masukkan nama peminjam: ");
                if (fgets(nama, sizeof(nama), stdin) == NULL) {
                    printf("Error membaca input\n");
                    break;
                }
                nama[strcspn(nama, "\n")] = '\0';

                printf("Masukkan judul buku yang dikembalikan: ");
                if (fgets(judul, sizeof(judul), stdin) == NULL) {
                    printf("Error membaca input\n");
                    break;
                }
                judul[strcspn(judul, "\n")] = '\0';

                prosesPengembalian(perpus, nama, judul);
                break;
            }
                
            case 5: { // Batalkan Antrian Peminjam
                printf("Masukkan nama peminjam: ");
                if (fgets(nama, sizeof(nama), stdin) == NULL) {
                    printf("Error membaca input\n");
                    break;
                }
                nama[strcspn(nama, "\n")] = '\0';

                printf("Masukkan judul buku: ");
                if (fgets(judul, sizeof(judul), stdin) == NULL) {
                    printf("Error membaca input\n");
                    break;
                }
                judul[strcspn(judul, "\n")] = '\0';

                Buku* buku = cariBuku(&perpus->db, judul);
                if (buku == NULL) {
                    printf("Buku tidak ditemukan!\n");
                    break;
                }

                if (hapusPeminjam(&perpus->aq, nama, buku)) {
                    printf("Antrian peminjam %s untuk buku %s berhasil dibatalkan\n", nama, judul);
                    
                    // Catat aktivitas di history
                    if (perpus->history_count < MAX_HISTORY) {
                        snprintf(perpus->history[perpus->history_count].aksi, 50, "Batalkan antrian");
                        strncpy(perpus->history[perpus->history_count].nama, nama, MAX_NAMA - 1);
                        strncpy(perpus->history[perpus->history_count].buku, judul, MAX_JUDUL - 1);
                        perpus->history[perpus->history_count].prioritas = UMUM;
                        perpus->history_count++;
                    }
                } else {
                    printf("Peminjam tidak ditemukan dalam antrian untuk buku tersebut\n");
                }
                break;
            }
                  
            case 0: { // Keluar
                printf("Terima kasih telah menggunakan sistem perpustakaan!\n");
                break;
            }
                
            default:
                printf("Pilihan tidak valid! Silakan pilih 0-6.\n");
        }
        
        if (pilihan != 0) {
            printf("\nTekan Enter untuk kembali ke menu...");
            clearInputBuffer();
        }
    } while (pilihan != 0);
}
