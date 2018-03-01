#ifndef FIB_H
#define FIB_H
#include "std_bf.h"
#include "SIMD_operation.h"

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <Windows.h>
#include <algorithm>

using namespace std;

struct chunkInfo {
    int chunk_id;
    unsigned int nexthop[CHUNK];
};

struct subTrieUpdateArg {
    unsigned char targetLevel;	//16、24、32
    unsigned char length;
};

struct element_16 {
    unsigned int nexthop;
    bool flag;
};

struct level16_Table {
    int *levelTable16_NH;
    unsigned int *levelTable16_offset;
};

/*
 *  For Packet Classification
 *      2015/01/18
 *
 * */
struct ruleSet {
    unsigned short minID;
    vector<unsigned short> rules;
    //std_bf *rBF;
    //TODO: Add Bloom filter later
};

struct lookupRes {
    unsigned short minID;
    vector<unsigned short> *rules;
    std_bf *rBF;
};

struct FibTrie
{
    FibTrie*			lchild;					//point to the left child(0)
    FibTrie*			rchild;					//point to the right child(1)

    struct ruleSet*      RS;

    char					nodeLevel;			// Add by Qiaobin Fu
    short					chunkID;				//the chunk of the node. Add by Qiaobin Fu
};

struct pRuleSet {
	unsigned int minID;
	vector<unsigned short> *rules;
};

class CFib
{
    public:
        FibTrie* m_pTrie;				//root node of FibTrie
        int allNodeCount;				//to count all the nodes in Trie tree, including empty node
        int solidNodeCount;				//to count all the solid nodes in Trie tree
        unsigned long long ruleCount;

		unsigned int Chunk24_num;
		unsigned Chunk32_num;

        /***********************************New Update***************************************/
        unsigned int invalid;
        unsigned int newBornNode;

        unsigned int currentChunkNum24;	//24层chunk 的总个数
        unsigned int currentChunkNum32;	//32层chunk 的总个数

        unsigned int level16_nonleaf_num;
        unsigned int level24_nonleaf_num;

        unsigned int level16_node_num;
        unsigned int level24_node_num;
        unsigned int level32_node_num;

        int LevelPort[40][200];

        unsigned int currentLenBit24;	
        unsigned int prefix32_num;		
        unsigned int lenofchain;

#if 0
        int8_t *levelTable16_NH;
        unsigned int *levelTable16_offset;

        int8_t *levelTable24_NH;
        unsigned int *levelTable24_offset;

        int8_t *levelTable32_NH;
#endif
        std_bf** levelTable16_BF;
		struct pRuleSet *levelTable16_NH;
        unsigned int *levelTable16_offset;
        
        std_bf** levelTable24_BF;
		struct pRuleSet *levelTable24_NH;
        unsigned int *levelTable24_offset;
        
        std_bf** levelTable32_BF;
		struct pRuleSet *levelTable32_NH;

        struct chunkInfo chunkLevel16;
        struct chunkInfo chunkLevel24;
        struct chunkInfo chunkLevel32;

        /***************************************End*********************************************/

        CFib(void);
        ~CFib(void);
		void CFib::createChunkBF();
		void CFib::ytLevelPushing(FibTrie* pTrie, unsigned int level, struct pRuleSet default_rule);
		//struct ruleSet CFib::basicLookup(unsigned int ip);
		void CFib::basicLookup(unsigned int ip, struct lookupRes &lrs);

        // creat a new FIBTRIE ndoe 
        void CreateNewNode(FibTrie* &pTrie);
        //get the total number of nodes in RibTrie  
        void ytGetNodeCounts();
      //  void Pretraversal(FibTrie* pTrie);
		void Pretraversal(FibTrie* pTrie, int level);
        //output the result
        //void OutputTrie(FibTrie* pTrie,string sFileName,string oldPortfile);
        void OutputTrie_32(FibTrie* pTrie);
        bool IsLeaf(FibTrie * pNode);
    private:
        //get and output all the nexthop in Trie
        void GetTrieHops_32(FibTrie* pTrie,unsigned int iVal,int iBitLen,ofstream* fout,bool ifnewPort);
    public:
        unsigned int BuildFibFromFile(string sFileName, int dir);
        void AddNode(unsigned long lPrefix,unsigned int iPrefixLen,unsigned int iRuleID);

        int num_level[50];

        void LevelStatistic(FibTrie* pTrie, unsigned int level);
        //OH algorithm
        unsigned int btod(char *bstr);
        unsigned int TWMX( unsigned int a);

        /***********************************New Update***************************************/
        bool isCorrectTrie(FibTrie *pTrie);
        unsigned int GetAncestorHop(FibTrie* pTrie);
        void ytOptLevelPushing(FibTrie* pTrie, unsigned int level,struct ruleSet default_rule);
        void OutputTrie(FibTrie* pTrie,string sFileName,string oldPortfile, bool ifOpt);
        void GetTrieHops(FibTrie* pTrie,unsigned int iVal,int iBitLen,ofstream* fout,bool ifnewPort, bool ifOpt);

        void buildBF(struct ruleSet* rs, std_bf *rBF);
        void buildLookupTable(FibTrie* pTrie);

        struct ruleSet optTrieLookup(char * insert_C);
        struct ruleSet blessLookup(unsigned int ip);
       // struct lookupRes BFLookup(unsigned int ip);
		void BFLookup(unsigned int ip, struct lookupRes &lrs);
		void BFLookup_noinsert(unsigned int ip, struct lookupRes &lrs);
		void basicBFLookup(unsigned int ip, struct lookupRes &lrs);
		void basicBFLookup_noinsert(unsigned int ip, struct lookupRes &lrs);

        /***************************************End*********************************************/

        /***************************************Virtual Pushing*******************************************/
        void TrafficRead(char *traffic_file);
        void TestLookupSpeed(char *traffi_fil, char* fib_fil);
        void TestLookupSpeed_old(char *traffi_fil, char* fib_fil);
        /********************************************End**************************************************/
};
#endif
