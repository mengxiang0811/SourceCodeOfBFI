#include "std_bf.h"

uint pos_div[MAX_RULES + 1][BF_HF];
uint pos_mod[MAX_RULES + 1][BF_HF];

std_bf::std_bf()
{
}


std_bf::~std_bf()
{
	delete[] bf_base;
}

std_bf::std_bf(uint m, uint k) {
	bf_m = m;
	bf_k = k;
	//bf_n = 0;
	//querymemAccNum = 0;

	uint size = bf_m / 8;
#if 0
	if (bf_m % 8 != 0) {
		size++;
		printf("Warning: BF size should be 512 bit, make sure this's ok!");
	}
#endif

    bf_base = (uchar *)_aligned_malloc(size, 16);

	memset(bf_base, 0, size);

//	for (int i = 0; i < bf_k; i++)
//		bf_hfp[i].initialize(i);

	/*
	unsigned int(*tmp_ptr[18])(const unsigned char * str, unsigned int len) =
	{ JSHash, OCaml, OAAT, PJWHash, RSHash, SBOX, SDBM, Simple, SML, STL,
	APHash, BKDR, BOB, CRC32, DEKHash, DJBHash, FNV32, Hsieh };
	for (int i = 0; i < 18; i++){
		bf_hfp[i] = tmp_ptr[i];
	}
	*/
}


uint std_bf::insert(uint x){
	register uint i;
	for (i = 0; i < BF_HF; i++)
		bf_base[pos_div[x][i]] |= pos_mod[x][i];
	return 0;
}

uint std_bf::query(uint x){
	register uint i;
	for (i = 0; i < BF_HF; i++)
		if (0 == (bf_base[pos_div[x][i]] & pos_mod[x][i])) return 0;
	return 1;
}

/*

uint std_bf::insert(const unsigned char * str, uint len){
	uint pos;
	for (uint i = 0; i < bf_k; i++){
		//pos = bf_hfp[i].run(str, len) % bf_m;
		pos = bf_hfp[i](str, len) % bf_m;
		bf_base[pos / 8] |= (128 >> (pos % 8));
	}
	bf_n++;
	return 1;
}

uint std_bf::query(const unsigned char * str, uint len){
	uint pos;
	for (uint i = 0; i < bf_k; i++){
		//pos = bf_hfp[i].run(str, len) % bf_m;
		pos = bf_hfp[i](str, len) % bf_m;
		querymemAccNum++;
		if (0 == (bf_base[pos / 8] & (128 >> (pos % 8))))return 0;
	}
	return 1;
}
*/