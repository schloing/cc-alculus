#ifndef INTERFACE_H
#define INTERFACE_H

// x86-64 (amd64)

#define eax_off 0
#define ebx_off 3
#define ecx_off 1
#define edx_off 2

#define esp_off 4
#define esi_off 6
#define edi_off 7

#define mov(offset) 0xb8 + offset

// mov [eax-edi], 0xaa
// ... mov(eax_off) aa 00 00 00

#endif