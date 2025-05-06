#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "menu.h"

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void tampilkanMenu() {
    printf("\n=== PERPUSTAKAAN ===\n");
    printf("1. Insert Buku\n");
    printf("2. Display Buku (dan Antrean)\n");
    printf("3. Daftar yang Akan Meminjam\n");
    printf("4. Proses Pinjaman\n");
    printf("5. Batalkan Pinjaman\n");
    printf("6. Proses Pengembalian\n");
    printf("7. Lihat yang Sedang Meminjam\n");
    printf("0. Keluar\n");
    printf("Pilihan: ");
}

void prosesMenu(Perpustakaan *perpus) {
    int pilihan;
    char judul[100], nama[100];
    int stok, prioritas;
    
    do {
        tampilkanMenu();
        scanf("%d", &pilihan);
        clearBuffer();
        
        switch(pilihan) {
            case 1: // Insert Buku
                printf("Masukkan judul buku: ");
                fgets(judul, sizeof(judul), stdin);
                judul[strcspn(judul, "\n")] = '\0';
                
                printf("Masukkan stok buku: ");
                scanf("%d", &stok);
                clearBuffer();
                
                tambahBukuPerpus(perpus, judul, stok);
                printf("Buku berhasil ditambahkan!\n");
                break;
                
            case 2: // Display Buku dan Antrean
                tampilkanStatusPerpustakaan(perpus);
                break;
                
            case 3: // Daftar yang Akan Meminjam
                printf("Masukkan nama peminjam: ");
                fgets(nama, sizeof(nama), stdin);
                nama[strcspn(nama, "\n")] = '\0';
                
                printf("Masukkan judul buku: ");
                fgets(judul, sizeof(judul), stdin);
                judul[strcspn(judul, "\n")] = '\0';
                
                printf("Masukkan prioritas (0=Dosen, 1=Mahasiswa, 2=Umum): ");
                scanf("%d", &prioritas);
                clearBuffer();
                
                if (prioritas < 0 || prioritas > 2) {
                    printf("Prioritas tidak valid!\n");
                    break;
                }
                
                daftarPeminjam(perpus, nama, (Prioritas)prioritas, judul);
                printf("Peminjam berhasil didaftarkan!\n");
                break;
                
            case 4: // Proses Pinjaman
                printf("Masukkan judul buku yang akan dipinjam: ");
                fgets(judul, sizeof(judul), stdin);
                judul[strcspn(judul, "\n")] = '\0';
                
                prosesPeminjaman(perpus, judul);
                break;
                
            case 5: // Batalkan Pinjaman
                batalkanAktivitasTerakhir(perpus);
                break;
                
            case 6: // Proses Pengembalian
                printf("Masukkan nama peminjam: ");
                fgets(nama, sizeof(nama), stdin);
                nama[strcspn(nama, "\n")] = '\0';
                
                printf("Masukkan judul buku yang dikembalikan: ");
                fgets(judul, sizeof(judul), stdin);
                judul[strcspn(judul, "\n")] = '\0';
                
                prosesPengembalian(perpus, nama, judul);
                break;
                
            case 7: // Lihat yang Sedang Meminjam
                printf("Fitur ini belum diimplementasikan\n");
                break;
                
            case 0: // Keluar
                printf("Terima kasih!\n");
                break;
                
            default:
                printf("Pilihan tidak valid!\n");
        }
    } while (pilihan != 0);
}
