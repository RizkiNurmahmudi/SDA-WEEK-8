#include <stdio.h>
#include <stdlib.h>
#include "perpustakaan.h"
#include "menu.h"

int main() {
    Perpustakaan perpus;
    
    // Inisialisasi perpustakaan
    initPerpustakaan(&perpus);
    
    // Jalankan menu interaktif
    prosesMenu(&perpus);
    
    // Bebaskan memori sebelum keluar
    freeBuku(&perpus.db);
    
    return 0;
}
