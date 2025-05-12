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
    system("clear"); // Untuk Unix/Linux
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
    int i;
    printf("[Daftar Buku]\n");
    if (perpus->db.jumlah == 0) {
        printf("Belum ada buku terdaftar\n");
    } else {
        for (i = 0; i < perpus->db.jumlah; i++) {
            printf("- %s (Total: %d, Tersedia: %d)\n",
                   perpus->db.data[i].judul,
                   perpus->db.data[i].stok,
                   perpus->db.data[i].stok_tersedia);
        }
    }
    
    // Tampilkan antrian peminjam
    printf("\n[Antrian Peminjam]\n");
    if (perpus->aq.size == 0) {
        printf("Tidak ada antrian peminjam\n");
    } else {
        for (i = 0; i < perpus->aq.size; i++) {
            const char *prioritas_str;
            switch (perpus->aq.data[i].prioritas) {
                case DOSEN: prioritas_str = "Dosen"; break;
                case MAHASISWA: prioritas_str = "Mahasiswa"; break;
                case UMUM: prioritas_str = "Umum"; break;
                default: prioritas_str = "Tidak diketahui";
            }
            printf("- ID: %s, Nama: %s (Prioritas: %s) - Buku: %s\n",
                   perpus->aq.data[i].id,
                   perpus->aq.data[i].nama,
                   prioritas_str,
                   perpus->aq.data[i].buku->judul);
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
    printf("6. Tampilkan Status Perpustakaan\n");
    printf("0. Keluar\n");
    printf("Pilihan: ");
}

// Fungsi untuk memproses pilihan menu
void prosesMenu(Perpustakaan *perpus) {
    int pilihan;
    char input[MAX_NAMA], judul[MAX_JUDUL], nama[MAX_NAMA];
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
                printf("Masukkan ID peminjam: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    printf("Error membaca input\n");
                    break;
                }
                input[strcspn(input, "\n")] = '\0';

                prosesPengembalian(perpus, input);
                break;
            }
                
            case 5: { // Batalkan Antrian Peminjam
                printf("Masukkan ID peminjam: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    printf("Error membaca input\n");
                    break;
                }
                input[strcspn(input, "\n")] = '\0';

                batalkanAntrian(perpus, input);
                break;
            }
                
            case 6: { // Tampilkan Status Perpustakaan
                tampilkanStatusPerpustakaan(perpus);
                printf("\nTekan Enter untuk kembali ke menu...");
                clearInputBuffer();
                break;
            }
                  
            case 0: { // Keluar
                printf("Terima kasih telah menggunakan sistem perpustakaan!\n");
                break;
            }
                
            default:
                printf("Pilihan tidak valid! Silakan pilih 0-6.\n");
        }
        
        if (pilihan != 0 && pilihan != 6) {
            printf("\nTekan Enter untuk kembali ke menu...");
            clearInputBuffer();
        }
    } while (pilihan != 0);
}
