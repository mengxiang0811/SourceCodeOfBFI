#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <fstream>

using namespace std;

#define _NOT_DELETE 0
#define _INSERT 1
#define _DELETE	2
#define _CHANGE	3
#define LENGTH_24   35535*256
//#define SIZEOF2_24	16777216
#define H1_LEN  9949801//8677633 49999*199 
#define H2_LEN 524288*4 //2^19

#define PORT_LEN    sizeof(int8_t)
#define PORT_MAX    127

#define HIGHTBIT    2147483648  //Binary: 10000000000000000000000000000000

/**********************New Update***************************/
#define	CHUNK   256 // The number of elements in each chunk

#define	LEVEL16_ELE_NUM			65536       // size of lookup table for level 16
#define	LEVEL24_CHUNK_NUM		25000       // number of chunks for level 24
#define	LEVEL32_CHUNK_NUM		1000000     // number of chunks for level 32

#define NB_16   0 //only create new nodes in level 17~24
#define NB_24   1 //only create new nodes in level 25~32
#define NB_BOTH 2 //both above                                                    
/**************************End********************************/

// 8-bit Next Hop: 1 ... 127
struct chunkInfo {
    int chunk_id;
    int8_t nexthop[CHUNK];
};

struct subTrieUpdateArg {
    uint8_t targetLevel;	//16 or 24 or 32
    uint8_t length;
};

struct subTriePushArg {
    int chunk_id;
    uint8_t startLevel; // Level 0 or 16 or 24
    uint8_t targetLevel; // Level 16 or 24 or 32
    bool ifCopy; // decide whether to copy the NH array
};

struct element_16 {
    int8_t nexthop;
    bool flag;
};

struct level16_Table {
    int8_t *levelTable16_NH;
    unsigned short *levelTable16_offset;
};

struct FibTrie {
    FibTrie*			parent;     //point to father node
    FibTrie*			lchild;     //point to the left child(0)
    FibTrie*			rchild;     //point to the right child(1)
    int					newPort;					
    int					oldPort;
    int					nodeLevel;  
    int					chunkID;    
    bool				ifpushed;   //if the missing node.
};

class CFib {
    public:
        FibTrie* m_pTrie;				//root node of FibTrie
        int allNodeCount;				//to count all the nodes in Trie tree, including empty node
        int solidNodeCount;				//to count all the solid nodes in Trie tree


        /***********************************New Update***************************************/
        unsigned int invalid;
        unsigned int newBornNode;

        unsigned int currentChunkNum24;	// current number of chunks for level 24
        unsigned int currentChunkNum32;	// current number of chunks for level 32

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
        short *levelTable16_NH;
        unsigned short *levelTable16_offset;
#endif

#if 0
        short *levelTable16_NH;
        short *levelTable24_NH;
        unsigned short *levelTable32_NH;
#endif

        int8_t *levelTable16_NH;
        unsigned short *levelTable16_offset;

        int8_t *levelTable24_NH;
        unsigned short *levelTable24_offset;

        int8_t *levelTable32_NH;

        /*
         *  
         *  Each updating at most affects 128 nodes (except for level 8)
         *  For convenience, we set the chunk size as 256 (i.e., 1 << 8)
         *   left-subtrie uses 0 ... 127
         *   right-subtrie uses 128 ... 255
         *
         * */

        struct chunkInfo chunkBuffer;

        /*
         * Update statistics
         *
         * */
        uint64_t memory_access;
        uint64_t worst_num;
        uint64_t maStat[33];
#if 0
        struct chunkInfo chunkLevel16;
        struct chunkInfo chunkLevel24;
        struct chunkInfo chunkLevel32;
#endif

        /***************************************End*********************************************/

        CFib(void);
        ~CFib(void);

        // creat a new FIBTRIE ndoe 
        void CreateNewNode(FibTrie* &pTrie);
        //get the total number of nodes in RibTrie  
        void ytGetNodeCounts();
        void Pretraversal(FibTrie* pTrie);
        //output the result
        //void OutputTrie(FibTrie* pTrie,string sFileName,string oldPortfile);
        void OutputTrie_32(FibTrie* pTrie);
        bool IsLeaf(FibTrie * pNode);
    private:
        //get and output all the nexthop in Trie
        //void CFib::GetTrieHops(FibTrie* pTrie,unsigned int iVal,int iBitLen,ofstream* fout,bool ifnewPort);
        void GetTrieHops_32(FibTrie* pTrie,unsigned int iVal,int iBitLen,ofstream* fout,bool ifnewPort);
    public:
        unsigned int BuildFibFromFile(string sFileName);
        void AddNode(unsigned long lPrefix,unsigned int iPrefixLen,unsigned int iNextHop);
        void LeafPush(FibTrie* pTrie, int depth);

        void ytLevelPushing(FibTrie* pTrie, unsigned int level,unsigned int default_port);

        int num_level[50];

        void LevelStatistic(FibTrie* pTrie, unsigned int level);
        //OH algorithm
        unsigned int btod(char *bstr);
        unsigned int TWMX( unsigned int a);

        /***********************************New Update***************************************/
        bool isCorrectTrie(FibTrie *pTrie);
        unsigned int GetAncestorHop(FibTrie* pTrie);
        unsigned int getTrueHop(FibTrie* pTrie, int level);
        void ytOptLevelPushing(FibTrie* pTrie, unsigned int level,unsigned int default_port);
        void OutputTrie(FibTrie* pTrie,string sFileName,string oldPortfile, bool ifOpt);
        void GetTrieHops(FibTrie* pTrie,unsigned int iVal,int iBitLen,ofstream* fout,bool ifnewPort, bool ifOpt);

        void buildLookupTable(FibTrie* pTrie);

        unsigned int trieLookup(char * insert_C);
        unsigned int optTrieLookup(char * insert_C);
        unsigned int blessLookup(unsigned int ip);
        unsigned int fqbblessLookup(unsigned int ip);
        //unsigned int fqbblessLookup(unsigned int ip);

        int isCriticalLevel(int level);
        void statInit();
        void Update(int insertport, char *insert_C, int operation_type, char* sprefix, bool ifStat);
        /***************************************End*********************************************/

        /***************************************Virtual Pushing*******************************************/
        void updateInfoInit();
        unsigned int  subTrieUpdate(FibTrie* pTrie, unsigned int level,unsigned int default_port, unsigned int prefix, struct subTrieUpdateArg *arg);
        void subTrieVirtualPushing(FibTrie* pTrie, unsigned int level,unsigned int default_port, unsigned int prefix, struct subTriePushArg *arg);
        void TrafficRead(char *traffic_file);
        void TestLookupSpeed(char *traffi_fil, char* fib_fil);
        void TestLookupSpeed_old(char *traffi_fil, char* fib_fil);
        /********************************************End**************************************************/
};
