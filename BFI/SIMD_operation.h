#ifndef SIMD_OPERATION_H
#define SIMD_OPERATION_H

//SIMD AND

void SIMD_AND_512(void *A, void *B, void *C, void *D, void *E);
void SIMD_AND_1024(void *A, void *B, void *C, void *D, void *E);
void SIMD_AND_2048(void *A, void *B, void *C, void *D, void *E);
void SIMD_AND_4096(void *A, void *B, void *C, void *D, void *E);

//SIMD OR 

void SIMD_OR_512(void *A, void *B);
void SIMD_OR_1024(void *A, void *B);
void SIMD_OR_2048(void *A, void *B);
void SIMD_OR_4096(void *A, void *B);

void SIMD_OR_512(void *A, void *B, void *C);
void SIMD_OR_1024(void *A, void *B, void *C);
void SIMD_OR_2048(void *A, void *B, void *C);
void SIMD_OR_4096(void *A, void *B, void *C);

void SIMD_OR_512(void *A, void *B, void *C, void *D);
void SIMD_OR_1024(void *A, void *B, void *C, void *D);
void SIMD_OR_2048(void *A, void *B, void *C, void *D);
void SIMD_OR_4096(void *A, void *B, void *C, void *D);

//SIMD MOV

void SIMD_MOV_512(void *A, void *B);
void SIMD_MOV_1024(void *A, void *B);
void SIMD_MOV_2048(void *A, void *B);
void SIMD_MOV_4096(void *A, void *B);

#endif