#include <stdio.h>
#include <stdbool.h>

#define PAGE_SIZE 1024

struct PageTableEntry {
    int frame_number;
    bool valid;
};

struct SegmentEntry {
    int limit;
    struct PageTableEntry *page_table; // Puntero a la tabla de páginas del segmento
    bool read_permit;
    bool write_permit;
    bool execute_permit;
};

void traduce_direccion(
    struct SegmentEntry segment_table[],
    int seg,
    int page,
    int offset,
    char op) /* 'R' para lectura y 'W' para escritura */
{
    // Validación de Segmento
    if (page >= segment_table[seg].limit) { 
        printf("ERROR: La pagina excede el limite del segmento");
        return;
    }
    
    // Validación de permisos
    if (op == 'R' && !segment_table[seg].read_permit) {
        printf("ERROR: No se cuenta con permisos de lectura");
        return;
    }
    if (op == 'W' && !segment_table[seg].write_permit) {
        printf("ERROR: No se cuenta con permisos de escritura");
        return;
    }
    if (op == 'X' && !segment_table[seg].execute_permit) {
        printf("ERROR: No se cuenta con permisos de ejecución");
        return;
    }

    // Validación de página en RAM
    struct PageTableEntry pte = segment_table[seg].page_table[page];
    if (!pte.valid) {
        printf("ERROR: Página inactiva");
    }

    int physical_address = (pte.frame_number * PAGE_SIZE) + offset;
    printf("[ÉXITO] Dirección Física = %d (Hex: 0x%X)\n", physical_address, physical_address);
    
}