#ifndef STD_BF_H
#define STD_BF_H

#include "define.h"
#include "BOBHash.h"
#include "hash_function.h"
#include <string.h>
#include <iostream>
#include <set>
#include <malloc.h>
using namespace std;

extern uint pos_div[MAX_RULES + 1][BF_HF];
extern uint pos_mod[MAX_RULES + 1][BF_HF];

class std_bf 
{
public:
	std_bf();
	~std_bf();
	std_bf(uint m, uint k);
//	uint insert(const uchar * str, uint len);
//	uint query(const uchar * str, uint len);
	uint insert(uint x);
	uint query(uint x);

	uchar * bf_base; //bloom filter base

private:

	uint bf_m; //bloom filter length
	uint bf_k; //hash function numbers;
	uint bf_n; //# of elements inserted
	
	//pointers to hash function
	//BOBHash bf_hfp[MAX_HF]; ////////////////////////////////////////////////////////////////////

	//unsigned int(*bf_hfp[18])(const unsigned char * str, unsigned int len);

};
#endif
