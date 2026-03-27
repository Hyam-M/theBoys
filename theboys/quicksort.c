#include <stdio.h>
#include "quicksort.h"

int particiona(baseDist v[], int esq, int dir) {
    baseDist pivo = v[esq];   // struct inteiro
    int i = esq + 1;
    int j = dir;
    
    while (1) {

        while (i <= dir && v[i].dist <= pivo.dist) i++;
        while (v[j].dist > pivo.dist) j--;

        if (i >= j) break;

        // troca structs
        baseDist tmp = v[i];
        v[i] = v[j];
        v[j] = tmp;
    }

    v[esq] = v[j];
    v[j] = pivo;
    return j;   
}

void quickSort(baseDist v[], int esq, int dir) {
    if (esq < dir) {
        int p = particiona(v, esq, dir);
        quickSort(v, esq, p - 1);
        quickSort(v, p + 1, dir);
    }
}

