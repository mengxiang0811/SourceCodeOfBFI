#include "SIMD_operation.h"

void SIMD_AND_512(void *A, void *B, void *C, void *D, void *E) {
	/*
	* Address must be 16-aligned!
	* Input: five 512-bit BFs
	* Output: A = A & B & C & D & E
	*/
	_asm
	{
		mov edi, A;	// Address
		mov esi, B;
		mov eax, C;
		mov ebx, D;
		mov edx, E;

		movapd xmm0, [edi];			//Load from A
		movapd xmm1, [edi + 16];	//Load from A
		movapd xmm2, [edi + 32];	//Load from A
		movapd xmm3, [edi + 48];	//Load from A

		pand xmm0, [esi];			//Load from B & AND 
		pand xmm1, [esi + 16];		//Load from B & AND
		pand xmm2, [esi + 32];		//Load from B & AND
		pand xmm3, [esi + 48];		//Load from B & AND

		pand xmm0, [eax];			//Load from C & AND 
		pand xmm1, [eax + 16];		//Load from C & AND
		pand xmm2, [eax + 32];		//Load from C & AND
		pand xmm3, [eax + 48];		//Load from C & AND

		pand xmm0, [ebx];			//Load from D & AND 
		pand xmm1, [ebx + 16];		//Load from D & AND
		pand xmm2, [ebx + 32];		//Load from D & AND
		pand xmm3, [ebx + 48];		//Load from D & AND

		pand xmm0, [edx];			//Load from E & ANE 
		pand xmm1, [edx + 16];		//Load from E & AND
		pand xmm2, [edx + 32];		//Load from E & AND
		pand xmm3, [edx + 48];		//Load from E & AND

		movapd[edi], xmm0;			//Move into A
		movapd[edi + 16], xmm1;		//Move into A
		movapd[edi + 32], xmm2;		//Move into A
		movapd[edi + 48], xmm3;		//Move into A

		EMMS;
	}//end __asm
}

void SIMD_AND_1024(void *A, void *B, void *C, void *D, void *E) {
	/*
	* Address must be 16-aligned!
	* Input: five 1024-bit BFs
	* Output: A = A & B & C & D & E
	*/
	_asm
	{
		mov edi, A;	// Address
		mov esi, B;
		mov eax, C;
		mov ebx, D;
		mov edx, E;

		movapd xmm0, [edi];			//Load from A
		movapd xmm1, [edi + 16];	//Load from A
		movapd xmm2, [edi + 32];	//Load from A
		movapd xmm3, [edi + 48];	//Load from A
		movapd xmm4, [edi + 64];	//Load from A
		movapd xmm5, [edi + 80];	//Load from A
		movapd xmm6, [edi + 96];	//Load from A
		movapd xmm7, [edi + 112];	//Load from A


		pand xmm0, [esi];			//Load from B & AND 
		pand xmm1, [esi + 16];		//Load from B & AND
		pand xmm2, [esi + 32];		//Load from B & AND
		pand xmm3, [esi + 48];		//Load from B & AND
		pand xmm4, [esi + 64];		//Load from B & AND
		pand xmm5, [esi + 80];		//Load from B & AND
		pand xmm6, [esi + 96];		//Load from B & AND
		pand xmm7, [esi + 112];		//Load from B & AND

		pand xmm0, [eax];			//Load from C & AND 
		pand xmm1, [eax + 16];		//Load from C & AND
		pand xmm2, [eax + 32];		//Load from C & AND
		pand xmm3, [eax + 48];		//Load from C & AND
		pand xmm4, [eax + 64];		//Load from C & AND
		pand xmm5, [eax + 80];		//Load from C & AND
		pand xmm6, [eax + 96];		//Load from C & AND
		pand xmm7, [eax + 112];		//Load from C & AND

		pand xmm0, [ebx];			//Load from D & AND 
		pand xmm1, [ebx + 16];		//Load from D & AND
		pand xmm2, [ebx + 32];		//Load from D & AND
		pand xmm3, [ebx + 48];		//Load from D & AND
		pand xmm4, [ebx + 64];		//Load from D & AND
		pand xmm5, [ebx + 80];		//Load from D & AND
		pand xmm6, [ebx + 96];		//Load from D & AND
		pand xmm7, [ebx + 112];		//Load from D & AND

		pand xmm0, [edx];			//Load from E & ANE 
		pand xmm1, [edx + 16];		//Load from E & AND
		pand xmm2, [edx + 32];		//Load from E & AND
		pand xmm3, [edx + 48];		//Load from E & AND
		pand xmm4, [edx + 64];		//Load from E & AND
		pand xmm5, [edx + 80];		//Load from E & AND
		pand xmm6, [edx + 96];		//Load from E & AND
		pand xmm7, [edx + 112];		//Load from E & AND

		movapd[edi], xmm0;			//Move into A
		movapd[edi + 16], xmm1;		//Move into A
		movapd[edi + 32], xmm2;		//Move into A
		movapd[edi + 48], xmm3;		//Move into A
		movapd[edi + 64], xmm4;		//Move into A
		movapd[edi + 80], xmm5;		//Move into A
		movapd[edi + 96], xmm6;		//Move into A
		movapd[edi + 112], xmm7;	//Move into A

		EMMS;
	}//end __asm
}

void SIMD_AND_2048(void *A, void *B, void *C, void *D, void *E) {
	/*
	* Address must be 16-aligned!
	* Input: five 2048-bit BFs
	* Output: A = A & B & C & D & E
	*/
	_asm
	{
		mov edi, A;	// Address
		mov esi, B;
		mov eax, C;
		mov ebx, D;
		mov edx, E;

		mov ecx, 2;	// Counter
		jmp ZERO1;
	AL1:
		movapd xmm0, [edi];		//Load from A
		movapd xmm1, [edi + 16];		//Load from A
		movapd xmm2, [edi + 32];		//Load from A
		movapd xmm3, [edi + 48];		//Load from A
		movapd xmm4, [edi + 64];		//Load from A
		movapd xmm5, [edi + 80];		//Load from A
		movapd xmm6, [edi + 96];		//Load from A
		movapd xmm7, [edi + 112];		//Load from A

		pand xmm0, [esi];			//Load from B & AND 
		pand xmm1, [esi + 16];		//Load from B & AND
		pand xmm2, [esi + 32];		//Load from B & AND
		pand xmm3, [esi + 48];		//Load from B & AND
		pand xmm4, [esi + 64];		//Load from B & AND
		pand xmm5, [esi + 80];		//Load from B & AND
		pand xmm6, [esi + 96];		//Load from B & AND
		pand xmm7, [esi + 112];		//Load from B & AND

		pand xmm0, [eax];			//Load from C & AND 
		pand xmm1, [eax + 16];		//Load from C & AND
		pand xmm2, [eax + 32];		//Load from C & AND
		pand xmm3, [eax + 48];		//Load from C & AND
		pand xmm4, [eax + 64];		//Load from C & AND
		pand xmm5, [eax + 80];		//Load from C & AND
		pand xmm6, [eax + 96];		//Load from C & AND
		pand xmm7, [eax + 112];		//Load from C & AND

		pand xmm0, [ebx];			//Load from D & AND 
		pand xmm1, [ebx + 16];		//Load from D & AND
		pand xmm2, [ebx + 32];		//Load from D & AND
		pand xmm3, [ebx + 48];		//Load from D & AND
		pand xmm4, [ebx + 64];		//Load from D & AND
		pand xmm5, [ebx + 80];		//Load from D & AND
		pand xmm6, [ebx + 96];		//Load from D & AND
		pand xmm7, [ebx + 112];		//Load from D & AND

		pand xmm0, [edx];			//Load from E & ANE 
		pand xmm1, [edx + 16];		//Load from E & AND
		pand xmm2, [edx + 32];		//Load from E & AND
		pand xmm3, [edx + 48];		//Load from E & AND
		pand xmm4, [edx + 64];		//Load from E & AND
		pand xmm5, [edx + 80];		//Load from E & AND
		pand xmm6, [edx + 96];		//Load from E & AND
		pand xmm7, [edx + 112];		//Load from E & AND

		movapd[edi], xmm0;			//Move into A
		movapd[edi + 16], xmm1;		//Move into A
		movapd[edi + 32], xmm2;		//Move into A
		movapd[edi + 48], xmm3;		//Move into A
		movapd[edi + 64], xmm4;		//Move into A
		movapd[edi + 80], xmm5;		//Move into A
		movapd[edi + 96], xmm6;		//Move into A
		movapd[edi + 112], xmm7;		//Move into A

		add edi, 128;
		add esi, 128;
		add eax, 128;
		add ebx, 128;
		add edx, 128;

	ZERO1:
		dec ecx;
		jns AL1;
		EMMS;
	}//end __asm
}

void SIMD_AND_4096(void *A, void *B, void *C, void *D, void *E) {
	/*
	* Address must be 16-aligned!
	* Input: five 4096-bit BFs
	* Output: A = A & B & C & D & E
	*/
	_asm
	{
		mov edi, A;	// Address
		mov esi, B;
		mov eax, C;
		mov ebx, D;
		mov edx, E;

		mov ecx, 4;	// Counter
		jmp ZERO1;
	AL1:
		movapd xmm0, [edi];		//Load from A
		movapd xmm1, [edi + 16];		//Load from A
		movapd xmm2, [edi + 32];		//Load from A
		movapd xmm3, [edi + 48];		//Load from A
		movapd xmm4, [edi + 64];		//Load from A
		movapd xmm5, [edi + 80];		//Load from A
		movapd xmm6, [edi + 96];		//Load from A
		movapd xmm7, [edi + 112];		//Load from A

		pand xmm0, [esi];			//Load from B & AND 
		pand xmm1, [esi + 16];		//Load from B & AND
		pand xmm2, [esi + 32];		//Load from B & AND
		pand xmm3, [esi + 48];		//Load from B & AND
		pand xmm4, [esi + 64];		//Load from B & AND
		pand xmm5, [esi + 80];		//Load from B & AND
		pand xmm6, [esi + 96];		//Load from B & AND
		pand xmm7, [esi + 112];		//Load from B & AND

		pand xmm0, [eax];			//Load from C & AND 
		pand xmm1, [eax + 16];		//Load from C & AND
		pand xmm2, [eax + 32];		//Load from C & AND
		pand xmm3, [eax + 48];		//Load from C & AND
		pand xmm4, [eax + 64];		//Load from C & AND
		pand xmm5, [eax + 80];		//Load from C & AND
		pand xmm6, [eax + 96];		//Load from C & AND
		pand xmm7, [eax + 112];		//Load from C & AND

		pand xmm0, [ebx];			//Load from D & AND 
		pand xmm1, [ebx + 16];		//Load from D & AND
		pand xmm2, [ebx + 32];		//Load from D & AND
		pand xmm3, [ebx + 48];		//Load from D & AND
		pand xmm4, [ebx + 64];		//Load from D & AND
		pand xmm5, [ebx + 80];		//Load from D & AND
		pand xmm6, [ebx + 96];		//Load from D & AND
		pand xmm7, [ebx + 112];		//Load from D & AND

		pand xmm0, [edx];			//Load from E & ANE 
		pand xmm1, [edx + 16];		//Load from E & AND
		pand xmm2, [edx + 32];		//Load from E & AND
		pand xmm3, [edx + 48];		//Load from E & AND
		pand xmm4, [edx + 64];		//Load from E & AND
		pand xmm5, [edx + 80];		//Load from E & AND
		pand xmm6, [edx + 96];		//Load from E & AND
		pand xmm7, [edx + 112];		//Load from E & AND

		movapd[edi], xmm0;			//Move into A
		movapd[edi + 16], xmm1;		//Move into A
		movapd[edi + 32], xmm2;		//Move into A
		movapd[edi + 48], xmm3;		//Move into A
		movapd[edi + 64], xmm4;		//Move into A
		movapd[edi + 80], xmm5;		//Move into A
		movapd[edi + 96], xmm6;		//Move into A
		movapd[edi + 112], xmm7;		//Move into A

		add edi, 128;
		add esi, 128;
		add eax, 128;
		add ebx, 128;
		add edx, 128;

	ZERO1:
		dec ecx;
		jns AL1;
		EMMS;
	}//end __asm
}



void SIMD_OR_512(void *A, void *B, void *C) {
	/*
	* Address must be 16-aligned!
	* Input: Three 512-bit BF address
	* Output: C = A | B
	*/
	_asm
	{
		mov edi, A;	// Address
		mov esi, B;
		mov eax, C

		movapd xmm0, [edi];			//Load from A
		movapd xmm1, [edi + 16];	//Load from A
		movapd xmm2, [edi + 32];	//Load from A
		movapd xmm3, [edi + 48];	//Load from A

		por xmm0, [esi];			//Load from B & AND
		por xmm1, [esi + 16];		//Load from B & AND
		por xmm2, [esi + 32];		//Load from B & AND
		por xmm3, [esi + 48];		//Load from B & AND

		movapd[eax], xmm0;			//Move into C
		movapd[eax + 16], xmm1;		//Move into C
		movapd[eax + 32], xmm2;		//Move into C
		movapd[eax + 48], xmm3;		//Move into C

		EMMS;
	}//end __asm

}

void SIMD_OR_1024(void *A, void *B, void *C) {
	/*
	* Address must be 16-aligned!
	* Input: Three 1024-bit BF address
	* Output: C = A | B
	*/
	_asm
	{
		mov edi, A;	// Address
		mov esi, B;
		mov eax, C

		movapd xmm0, [edi];			//Load from A
		movapd xmm1, [edi + 16];	//Load from A
		movapd xmm2, [edi + 32];	//Load from A
		movapd xmm3, [edi + 48];	//Load from A
		movapd xmm4, [edi + 64];	//Load from A
		movapd xmm5, [edi + 80];	//Load from A
		movapd xmm6, [edi + 96];	//Load from A
		movapd xmm7, [edi + 112];	//Load from A

		por xmm0, [esi];			//Load from B & AND
		por xmm1, [esi + 16];		//Load from B & AND
		por xmm2, [esi + 32];		//Load from B & AND
		por xmm3, [esi + 48];		//Load from B & AND
		por xmm4, [esi + 64];		//Load from B & AND
		por xmm5, [esi + 80];		//Load from B & AND
		por xmm6, [esi + 96];		//Load from B & AND
		por xmm7, [esi + 112];		//Load from B & AND

		movapd[eax], xmm0;			//Move into C
		movapd[eax + 16], xmm1;		//Move into C
		movapd[eax + 32], xmm2;		//Move into C
		movapd[eax + 48], xmm3;		//Move into C
		movapd[eax + 64], xmm4;		//Move into C
		movapd[eax + 80], xmm5;		//Move into C
		movapd[eax + 96], xmm6;		//Move into C
		movapd[eax + 112], xmm7;		//Move into C

		EMMS;
	}//end __asm

}

void SIMD_OR_2048(void *A, void *B, void *C) {
	/*
	* Address must be 16-aligned!
	* Input: Three 2048-bit BF address
	* Output: C = A | B
	*/
	_asm
	{
		mov edi, A;	// Address
		mov esi, B;
		mov eax, C

		mov ecx, 2;	// Counter
		jmp ZERO1;
	AL1:
		movapd xmm0, [edi];			//Load from A
		movapd xmm1, [edi + 16];	//Load from A
		movapd xmm2, [edi + 32];	//Load from A
		movapd xmm3, [edi + 48];	//Load from A
		movapd xmm4, [edi + 64];	//Load from A
		movapd xmm5, [edi + 80];	//Load from A
		movapd xmm6, [edi + 96];	//Load from A
		movapd xmm7, [edi + 112];	//Load from A

		por xmm0, [esi];			//Load from B & AND
		por xmm1, [esi + 16];		//Load from B & AND
		por xmm2, [esi + 32];		//Load from B & AND
		por xmm3, [esi + 48];		//Load from B & AND
		por xmm4, [esi + 64];		//Load from B & AND
		por xmm5, [esi + 80];		//Load from B & AND
		por xmm6, [esi + 96];		//Load from B & AND
		por xmm7, [esi + 112];		//Load from B & AND

		movapd[eax], xmm0;			//Move into C
		movapd[eax + 16], xmm1;		//Move into C
		movapd[eax + 32], xmm2;		//Move into C
		movapd[eax + 48], xmm3;		//Move into C
		movapd[eax + 64], xmm4;		//Move into C
		movapd[eax + 80], xmm5;		//Move into C
		movapd[eax + 96], xmm6;		//Move into C
		movapd[eax + 112], xmm7;		//Move into C

		add edi, 128;
		add esi, 128;
		add eax, 128;

	ZERO1:
		dec ecx;
		jns AL1;

		EMMS;
	}//end __asm

}

void SIMD_OR_4096(void *A, void *B, void *C) {
	/*
	* Address must be 16-aligned!
	* Input: Three 4096-bit BF address
	* Output: C = A | B
	*/
	_asm
	{
		mov edi, A;	// Address
		mov esi, B;
		mov eax, C

		mov ecx, 4;	// Counter
		jmp ZERO1;
	AL1:
		movapd xmm0, [edi];			//Load from A
		movapd xmm1, [edi + 16];	//Load from A
		movapd xmm2, [edi + 32];	//Load from A
		movapd xmm3, [edi + 48];	//Load from A
		movapd xmm4, [edi + 64];	//Load from A
		movapd xmm5, [edi + 80];	//Load from A
		movapd xmm6, [edi + 96];	//Load from A
		movapd xmm7, [edi + 112];	//Load from A

		por xmm0, [esi];			//Load from B & AND
		por xmm1, [esi + 16];		//Load from B & AND
		por xmm2, [esi + 32];		//Load from B & AND
		por xmm3, [esi + 48];		//Load from B & AND
		por xmm4, [esi + 64];		//Load from B & AND
		por xmm5, [esi + 80];		//Load from B & AND
		por xmm6, [esi + 96];		//Load from B & AND
		por xmm7, [esi + 112];		//Load from B & AND

		movapd[eax], xmm0;			//Move into C
		movapd[eax + 16], xmm1;		//Move into C
		movapd[eax + 32], xmm2;		//Move into C
		movapd[eax + 48], xmm3;		//Move into C
		movapd[eax + 64], xmm4;		//Move into C
		movapd[eax + 80], xmm5;		//Move into C
		movapd[eax + 96], xmm6;		//Move into C
		movapd[eax + 112], xmm7;		//Move into C

		add edi, 128;
		add esi, 128;
		add eax, 128;

	ZERO1:
		dec ecx;
		jns AL1;

		EMMS;
	}//end __asm

}



void SIMD_OR_512(void *A, void *B, void *C, void *D) {
	/*
	* Address must be 16-aligned!
	* Input: For 512-bit BF address
	* Output: D = A | B | C
	*/
	_asm
	{
		mov edi, A;	// Address
		mov esi, B;
		mov eax, C;
		mov ebx, D;

		movapd xmm0, [edi];			//Load from A
		movapd xmm1, [edi + 16];	//Load from A
		movapd xmm2, [edi + 32];	//Load from A
		movapd xmm3, [edi + 48];	//Load from A

		por xmm0, [esi];			//Load from B & AND
		por xmm1, [esi + 16];		//Load from B & AND
		por xmm2, [esi + 32];		//Load from B & AND
		por xmm3, [esi + 48];		//Load from B & AND

		por xmm0, [eax];			//Load from C & AND
		por xmm1, [eax + 16];		//Load from C & AND
		por xmm2, [eax + 32];		//Load from C & AND
		por xmm3, [eax + 48];		//Load from C & AND

		movapd[ebx], xmm0;			//Move into D
		movapd[ebx + 16], xmm1;		//Move into D
		movapd[ebx + 32], xmm2;		//Move into D
		movapd[ebx + 48], xmm3;		//Move into D

		EMMS;
	}//end __asm

}

void SIMD_OR_1024(void *A, void *B, void *C, void *D) {
	/*
	* Address must be 16-aligned!
	* Input: For 1024-bit BF address
	* Output: D = A | B | C
	*/
	_asm
	{
		mov edi, A;	// Address
		mov esi, B;
		mov eax, C;
		mov ebx, D;

		movapd xmm0, [edi];			//Load from A
		movapd xmm1, [edi + 16];	//Load from A
		movapd xmm2, [edi + 32];	//Load from A
		movapd xmm3, [edi + 48];	//Load from A
		movapd xmm4, [edi + 64];	//Load from A
		movapd xmm5, [edi + 80];	//Load from A
		movapd xmm6, [edi + 96];	//Load from A
		movapd xmm7, [edi + 112];	//Load from A

		por xmm0, [esi];			//Load from B & AND
		por xmm1, [esi + 16];		//Load from B & AND
		por xmm2, [esi + 32];		//Load from B & AND
		por xmm3, [esi + 48];		//Load from B & AND
		por xmm4, [esi + 64];		//Load from B & AND
		por xmm5, [esi + 80];		//Load from B & AND
		por xmm6, [esi + 96];		//Load from B & AND
		por xmm7, [esi + 112];		//Load from B & AND

		por xmm0, [eax];			//Load from C & AND
		por xmm1, [eax + 16];		//Load from C & AND
		por xmm2, [eax + 32];		//Load from C & AND
		por xmm3, [eax + 48];		//Load from C & AND
		por xmm4, [eax + 64];		//Load from C & AND
		por xmm5, [eax + 80];		//Load from C & AND
		por xmm6, [eax + 96];		//Load from C & AND
		por xmm7, [eax + 112];		//Load from C & AND

		movapd[ebx], xmm0;			//Move into D
		movapd[ebx + 16], xmm1;		//Move into D
		movapd[ebx + 32], xmm2;		//Move into D
		movapd[ebx + 48], xmm3;		//Move into D
		movapd[ebx + 64], xmm4;		//Move into D
		movapd[ebx + 80], xmm5;		//Move into D
		movapd[ebx + 96], xmm6;		//Move into D
		movapd[ebx + 112], xmm7;	//Move into D

		EMMS;
	}//end __asm

}

void SIMD_OR_2048(void *A, void *B, void *C, void *D) {
	/*
	* Address must be 16-aligned!
	* Input: For 2048-bit BF address
	* Output: D = A | B | C
	*/
	_asm
	{
		mov edi, A;	// Address
		mov esi, B;
		mov eax, C;
		mov ebx, D;

		mov ecx, 2;	// Counter
		jmp ZERO1;
	AL1:

		movapd xmm0, [edi];			//Load from A
		movapd xmm1, [edi + 16];	//Load from A
		movapd xmm2, [edi + 32];	//Load from A
		movapd xmm3, [edi + 48];	//Load from A
		movapd xmm4, [edi + 64];	//Load from A
		movapd xmm5, [edi + 80];	//Load from A
		movapd xmm6, [edi + 96];	//Load from A
		movapd xmm7, [edi + 112];	//Load from A

		por xmm0, [esi];			//Load from B & AND
		por xmm1, [esi + 16];		//Load from B & AND
		por xmm2, [esi + 32];		//Load from B & AND
		por xmm3, [esi + 48];		//Load from B & AND
		por xmm4, [esi + 64];		//Load from B & AND
		por xmm5, [esi + 80];		//Load from B & AND
		por xmm6, [esi + 96];		//Load from B & AND
		por xmm7, [esi + 112];		//Load from B & AND

		por xmm0, [eax];			//Load from C & AND
		por xmm1, [eax + 16];		//Load from C & AND
		por xmm2, [eax + 32];		//Load from C & AND
		por xmm3, [eax + 48];		//Load from C & AND
		por xmm4, [eax + 64];		//Load from C & AND
		por xmm5, [eax + 80];		//Load from C & AND
		por xmm6, [eax + 96];		//Load from C & AND
		por xmm7, [eax + 112];		//Load from C & AND

		movapd[ebx], xmm0;			//Move into D
		movapd[ebx + 16], xmm1;		//Move into D
		movapd[ebx + 32], xmm2;		//Move into D
		movapd[ebx + 48], xmm3;		//Move into D
		movapd[ebx + 64], xmm4;		//Move into D
		movapd[ebx + 80], xmm5;		//Move into D
		movapd[ebx + 96], xmm6;		//Move into D
		movapd[ebx + 112], xmm7;	//Move into D

		add edi, 128;
		add esi, 128;
		add eax, 128;
		add ebx, 128;

	ZERO1:
		dec ecx;
		jns AL1;
		EMMS;
	}//end __asm

}

void SIMD_OR_4096(void *A, void *B, void *C, void *D) {
	/*
	* Address must be 16-aligned!
	* Input: For 4096-bit BF address
	* Output: D = A | B | C
	*/
	_asm
	{
		mov edi, A;	// Address
		mov esi, B;
		mov eax, C;
		mov ebx, D;

		mov ecx, 4;	// Counter
		jmp ZERO1;
	AL1:

		movapd xmm0, [edi];			//Load from A
		movapd xmm1, [edi + 16];	//Load from A
		movapd xmm2, [edi + 32];	//Load from A
		movapd xmm3, [edi + 48];	//Load from A
		movapd xmm4, [edi + 64];	//Load from A
		movapd xmm5, [edi + 80];	//Load from A
		movapd xmm6, [edi + 96];	//Load from A
		movapd xmm7, [edi + 112];	//Load from A

		por xmm0, [esi];			//Load from B & AND
		por xmm1, [esi + 16];		//Load from B & AND
		por xmm2, [esi + 32];		//Load from B & AND
		por xmm3, [esi + 48];		//Load from B & AND
		por xmm4, [esi + 64];		//Load from B & AND
		por xmm5, [esi + 80];		//Load from B & AND
		por xmm6, [esi + 96];		//Load from B & AND
		por xmm7, [esi + 112];		//Load from B & AND

		por xmm0, [eax];			//Load from C & AND
		por xmm1, [eax + 16];		//Load from C & AND
		por xmm2, [eax + 32];		//Load from C & AND
		por xmm3, [eax + 48];		//Load from C & AND
		por xmm4, [eax + 64];		//Load from C & AND
		por xmm5, [eax + 80];		//Load from C & AND
		por xmm6, [eax + 96];		//Load from C & AND
		por xmm7, [eax + 112];		//Load from C & AND

		movapd[ebx], xmm0;			//Move into D
		movapd[ebx + 16], xmm1;		//Move into D
		movapd[ebx + 32], xmm2;		//Move into D
		movapd[ebx + 48], xmm3;		//Move into D
		movapd[ebx + 64], xmm4;		//Move into D
		movapd[ebx + 80], xmm5;		//Move into D
		movapd[ebx + 96], xmm6;		//Move into D
		movapd[ebx + 112], xmm7;	//Move into D

		add edi, 128;
		add esi, 128;
		add eax, 128;
		add ebx, 128;

	ZERO1:
		dec ecx;
		jns AL1;
		EMMS;
	}//end __asm

}



void SIMD_OR_512(void *A, void *B) {
	/*
	* Address must be 16-aligned!
	* Input:	Two 512-bit BFs
	* Output:	A = A | B
	*/
	_asm
	{
		mov edi, A;	// Address
		mov esi, B;

		movapd xmm0, [edi];			//Load from A
		movapd xmm1, [edi + 16];	//Load from A
		movapd xmm2, [edi + 32];	//Load from A
		movapd xmm3, [edi + 48];	//Load from A

		por xmm0, [esi];			//Load from B & AND 
		por xmm1, [esi + 16];		//Load from B & AND
		por xmm2, [esi + 32];		//Load from B & AND
		por xmm3, [esi + 48];		//Load from B & AND

		movapd[edi], xmm0;			//Move into A
		movapd[edi + 16], xmm1;		//Move into A
		movapd[edi + 32], xmm2;		//Move into A
		movapd[edi + 48], xmm3;		//Move into A

		EMMS;
	}//end __asm
}

void SIMD_OR_1024(void *A, void *B) {
	/*
	* Address must be 16-aligned!
	* Input:	Two 1024-bit BFs
	* Output:	A = A | B
	*/
	_asm
	{
		mov edi, A;	// Address
		mov esi, B;

		movapd xmm0, [edi];			//Load from A
		movapd xmm1, [edi + 16];	//Load from A
		movapd xmm2, [edi + 32];	//Load from A
		movapd xmm3, [edi + 48];	//Load from A
		movapd xmm4, [edi + 64];	//Load from A
		movapd xmm5, [edi + 80];	//Load from A
		movapd xmm6, [edi + 96];	//Load from A
		movapd xmm7, [edi + 112];	//Load from A

		por xmm0, [esi];			//Load from B & AND 
		por xmm1, [esi + 16];		//Load from B & AND
		por xmm2, [esi + 32];		//Load from B & AND
		por xmm3, [esi + 48];		//Load from B & AND
		por xmm4, [esi + 64];		//Load from B & AND
		por xmm5, [esi + 80];		//Load from B & AND
		por xmm6, [esi + 96];		//Load from B & AND
		por xmm7, [esi + 112];		//Load from B & AND

		movapd[edi], xmm0;			//Move into A
		movapd[edi + 16], xmm1;		//Move into A
		movapd[edi + 32], xmm2;		//Move into A
		movapd[edi + 48], xmm3;		//Move into A
		movapd[edi + 64], xmm4;		//Move into A
		movapd[edi + 80], xmm5;		//Move into A
		movapd[edi + 96], xmm6;		//Move into A
		movapd[edi + 112], xmm7;		//Move into A

		EMMS;
	}//end __asm
}

void SIMD_OR_2048(void *A, void *B) {
	/*
	* Address must be 16-aligned!
	* Input:	Two 2048-bit BFs
	* Output:	A = A | B
	*/
	_asm
	{
		mov edi, A;	// Address
		mov esi, B;

		mov ecx, 2;	// Counter
		jmp ZERO1;
	AL1:

		movapd xmm0, [edi];			//Load from A
		movapd xmm1, [edi + 16];	//Load from A
		movapd xmm2, [edi + 32];	//Load from A
		movapd xmm3, [edi + 48];	//Load from A
		movapd xmm4, [edi + 64];	//Load from A
		movapd xmm5, [edi + 80];	//Load from A
		movapd xmm6, [edi + 96];	//Load from A
		movapd xmm7, [edi + 112];	//Load from A

		por xmm0, [esi];			//Load from B & AND 
		por xmm1, [esi + 16];		//Load from B & AND
		por xmm2, [esi + 32];		//Load from B & AND
		por xmm3, [esi + 48];		//Load from B & AND
		por xmm4, [esi + 64];		//Load from B & AND
		por xmm5, [esi + 80];		//Load from B & AND
		por xmm6, [esi + 96];		//Load from B & AND
		por xmm7, [esi + 112];		//Load from B & AND

		movapd[edi], xmm0;			//Move into A
		movapd[edi + 16], xmm1;		//Move into A
		movapd[edi + 32], xmm2;		//Move into A
		movapd[edi + 48], xmm3;		//Move into A
		movapd[edi + 64], xmm4;		//Move into A
		movapd[edi + 80], xmm5;		//Move into A
		movapd[edi + 96], xmm6;		//Move into A
		movapd[edi + 112], xmm7;	//Move into A

		add edi, 128;
		add esi, 128;

	ZERO1:
		dec ecx;
		jns AL1;

		EMMS;
	}//end __asm
}

void SIMD_OR_4096(void *A, void *B) {
	/*
	* Address must be 16-aligned!
	* Input:	Two 4096-bit BFs
	* Output:	A = A | B
	*/
	_asm
	{
		mov edi, A;	// Address
		mov esi, B;

		mov ecx, 4;	// Counter
		jmp ZERO1;
	AL1:

		movapd xmm0, [edi];			//Load from A
		movapd xmm1, [edi + 16];	//Load from A
		movapd xmm2, [edi + 32];	//Load from A
		movapd xmm3, [edi + 48];	//Load from A
		movapd xmm4, [edi + 64];	//Load from A
		movapd xmm5, [edi + 80];	//Load from A
		movapd xmm6, [edi + 96];	//Load from A
		movapd xmm7, [edi + 112];	//Load from A

		por xmm0, [esi];			//Load from B & AND 
		por xmm1, [esi + 16];		//Load from B & AND
		por xmm2, [esi + 32];		//Load from B & AND
		por xmm3, [esi + 48];		//Load from B & AND
		por xmm4, [esi + 64];		//Load from B & AND
		por xmm5, [esi + 80];		//Load from B & AND
		por xmm6, [esi + 96];		//Load from B & AND
		por xmm7, [esi + 112];		//Load from B & AND

		movapd[edi], xmm0;			//Move into A
		movapd[edi + 16], xmm1;		//Move into A
		movapd[edi + 32], xmm2;		//Move into A
		movapd[edi + 48], xmm3;		//Move into A
		movapd[edi + 64], xmm4;		//Move into A
		movapd[edi + 80], xmm5;		//Move into A
		movapd[edi + 96], xmm6;		//Move into A
		movapd[edi + 112], xmm7;	//Move into A

		add edi, 128;
		add esi, 128;

	ZERO1:
		dec ecx;
		jns AL1;

		EMMS;
	}//end __asm
}




void SIMD_MOV_512(void *A, void *B) {
	/*
	* Address must be 16-aligned!
	* Input:	Two 512-bit BF address
	* Output:	B = A
	*/
	_asm
	{
		mov edi, A;	// Address
		mov esi, B;

		movapd xmm0, [edi];			//Load from A
		movapd xmm1, [edi + 16];	//Load from A
		movapd xmm2, [edi + 32];	//Load from A
		movapd xmm3, [edi + 48];	//Load from A

		movapd[esi], xmm0;			//Move into B
		movapd[esi + 16], xmm1;		//Move into B
		movapd[esi + 32], xmm2;		//Move into B
		movapd[esi + 48], xmm3;		//Move into B

		EMMS;
	}//end __asm
}

void SIMD_MOV_1024(void *A, void *B) {
	/*
	* Address must be 16-aligned!
	* Input:	Two 1024-bit BF address
	* Output:	B = A
	*/
	_asm
	{
		mov edi, A;	// Address
		mov esi, B;

		movapd xmm0, [edi];			//Load from A
		movapd xmm1, [edi + 16];	//Load from A
		movapd xmm2, [edi + 32];	//Load from A
		movapd xmm3, [edi + 48];	//Load from A
		movapd xmm4, [edi + 64];	//Load from A
		movapd xmm5, [edi + 80];	//Load from A
		movapd xmm6, [edi + 96];	//Load from A
		movapd xmm7, [edi + 112];	//Load from A

		movapd[esi], xmm0;			//Move into B
		movapd[esi + 16], xmm1;		//Move into B
		movapd[esi + 32], xmm2;		//Move into B
		movapd[esi + 48], xmm3;		//Move into B
		movapd[esi + 64], xmm4;		//Move into B
		movapd[esi + 80], xmm5;		//Move into B
		movapd[esi + 96], xmm6;		//Move into B
		movapd[esi + 112], xmm7;	//Move into B

		EMMS;
	}//end __asm
}

void SIMD_MOV_2048(void *A, void *B) {
	/*
	* Address must be 16-aligned!
	* Input:	Two 2048-bit BF address
	* Output:	B = A
	*/
	_asm
	{
		mov edi, A;	// Address
		mov esi, B;

		mov ecx, 2;	// Counter
		jmp ZERO1;
	AL1:

		movapd xmm0, [edi];			//Load from A
		movapd xmm1, [edi + 16];	//Load from A
		movapd xmm2, [edi + 32];	//Load from A
		movapd xmm3, [edi + 48];	//Load from A
		movapd xmm4, [edi + 64];	//Load from A
		movapd xmm5, [edi + 80];	//Load from A
		movapd xmm6, [edi + 96];	//Load from A
		movapd xmm7, [edi + 112];	//Load from A

		movapd[esi], xmm0;			//Move into B
		movapd[esi + 16], xmm1;		//Move into B
		movapd[esi + 32], xmm2;		//Move into B
		movapd[esi + 48], xmm3;		//Move into B
		movapd[esi + 64], xmm4;		//Move into B
		movapd[esi + 80], xmm5;		//Move into B
		movapd[esi + 96], xmm6;		//Move into B
		movapd[esi + 112], xmm7;	//Move into B

		add edi, 128;
		add esi, 128;

	ZERO1:
		dec ecx;
		jns AL1;
		EMMS;
	}//end __asm
}

void SIMD_MOV_4096(void *A, void *B) {
	/*
	* Address must be 16-aligned!
	* Input:	Two 4096-bit BF address
	* Output:	B = A
	*/
	_asm
	{
		mov edi, A;	// Address
		mov esi, B;

		mov ecx, 4;	// Counter
		jmp ZERO1;
	AL1:

		movapd xmm0, [edi];			//Load from A
		movapd xmm1, [edi + 16];	//Load from A
		movapd xmm2, [edi + 32];	//Load from A
		movapd xmm3, [edi + 48];	//Load from A
		movapd xmm4, [edi + 64];	//Load from A
		movapd xmm5, [edi + 80];	//Load from A
		movapd xmm6, [edi + 96];	//Load from A
		movapd xmm7, [edi + 112];	//Load from A

		movapd[esi], xmm0;			//Move into B
		movapd[esi + 16], xmm1;		//Move into B
		movapd[esi + 32], xmm2;		//Move into B
		movapd[esi + 48], xmm3;		//Move into B
		movapd[esi + 64], xmm4;		//Move into B
		movapd[esi + 80], xmm5;		//Move into B
		movapd[esi + 96], xmm6;		//Move into B
		movapd[esi + 112], xmm7;	//Move into B

		add edi, 128;
		add esi, 128;

	ZERO1:
		dec ecx;
		jns AL1;
		EMMS;
	}//end __asm
}


/*
void SIMD_AND_2(void *A, void *B, void *C, void *D, void *E) {
_asm
{
mov edi, A;	// Address
mov esi, B;
mov eax, C;
mov ebx, D;
mov edx, E;

movapd xmm0, [edi];			//Load from A
movapd xmm1, [edi + 16];	//Load from A
movapd xmm2, [edi + 32];	//Load from A
movapd xmm3, [edi + 48];	//Load from A

pand xmm0, [esi];			//Load from B & AND
pand xmm1, [esi + 16];		//Load from B & AND
pand xmm2, [esi + 32];		//Load from B & AND
pand xmm3, [esi + 48];		//Load from B & AND

movapd xmm4, [eax];			//Load from C & AND
movapd xmm5, [eax + 16];		//Load from C & AND
movapd xmm6, [eax + 32];		//Load from C & AND
movapd xmm7, [eax + 48];		//Load from C & AND

pand xmm4, [ebx];			//Load from D & AND
pand xmm5, [ebx + 16];		//Load from D & AND
pand xmm6, [ebx + 32];		//Load from D & AND
pand xmm7, [ebx + 48];		//Load from D & AND

pand xmm0, [edx];			//Load from E & ANE
pand xmm1, [edx + 16];		//Load from E & AND
pand xmm2, [edx + 32];		//Load from E & AND
pand xmm3, [edx + 48];		//Load from E & AND

pand xmm0, xmm4;
pand xmm1, xmm5;
pand xmm2, xmm6;
pand xmm3, xmm7;

movapd[edi], xmm0;			//Move into A
movapd[edi + 16], xmm1;		//Move into A
movapd[edi + 32], xmm2;		//Move into A
movapd[edi + 48], xmm3;		//Move into A

EMMS;
}//end __asm
}
*/