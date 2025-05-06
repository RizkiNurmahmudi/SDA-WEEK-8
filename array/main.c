#include <stdio.h>
#include "perpustakaan.h"
#include "menu.h"

int main() {
    Perpustakaan perpus;
    initPerpustakaan(&perpus);
    
    // Jalankan menu interaktif
    prosesMenu(&perpus);
    
    return 0;
}
