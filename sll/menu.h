#ifndef MENU_H
#define MENU_H

#include "perpustakaan.h"

// Deklarasi fungsi-fungsi menu
void tampilkanMenu();  // Menampilkan menu utama
void prosesMenu(Perpustakaan *perpus);  // Memproses pilihan menu
void clearScreen();  // Membersihkan layar
void clearInputBuffer();  // Membersihkan buffer input
void tampilkanDisplay(const Perpustakaan *perpus); // Menampilkan status perpustakaan

#endif
