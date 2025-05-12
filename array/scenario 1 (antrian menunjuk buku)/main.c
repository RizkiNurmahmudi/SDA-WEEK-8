#include <stdio.h>
#include <stdlib.h>
#include "perpustakaan.h"
#include "menu.h"

// Fungsi utama program
int main() {
    Perpustakaan perpus;
    
    // Inisialisasi perpustakaan
    initPerpustakaan(&perpus);
    
    // Jalankan menu interaktif
    prosesMenu(&perpus);
    
    return 0;
}
