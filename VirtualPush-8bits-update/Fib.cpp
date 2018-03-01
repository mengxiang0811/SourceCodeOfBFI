

#include "Fib.h"
#include <iostream>
//#include <time.h>
//#include <sys/time.h>

#include <windows.h>

#define FIBLEN				sizeof(struct FibTrie)		//size of each Trie node
#define EMPTYHOP			0							//Trie node doesnt't has a next hop

#define TRACE_READ 100000
unsigned int trafficIn[TRACE_READ];

const char * result_compare="result_compare.txt";
const char * hop_count="hop_count.txt";

//#define  0 

CFib::CFib(void)
{
    //initial the root of the Trie tree
    CreateNewNode(m_pTrie);

    allNodeCount=0;			//to count all the nodes in Trie tree, including empty node
    solidNodeCount=0;		//to count all the solid nodes in Trie tree


    prefix32_num=0;
    lenofchain=0;

    memset(num_level, 0, sizeof(num_level));

    memset(LevelPort,0,sizeof(LevelPort));

    currentLenBit24=0;

    /***********************************New Update***************************************/
    invalid = 0;

    currentChunkNum24 = 0;
    currentChunkNum32 = 0;

    level16_nonleaf_num = 0;
    level24_nonleaf_num = 0;

    level16_node_num = 0;
    level24_node_num = 0;
    level32_node_num = 0;

    levelTable16_NH = (int8_t *)calloc(LEVEL16_ELE_NUM, sizeof(int8_t));
    levelTable16_offset = (unsigned short *)calloc(LEVEL16_ELE_NUM, sizeof(unsigned short));
    memset(levelTable16_NH, PORT_MAX, (sizeof(int8_t) * LEVEL16_ELE_NUM)); // ...

    levelTable24_NH = (int8_t *)calloc((LEVEL24_CHUNK_NUM * CHUNK), sizeof(int8_t));
    levelTable24_offset = (unsigned short *)calloc((LEVEL24_CHUNK_NUM * CHUNK), sizeof(unsigned short));
    memset(levelTable24_NH, PORT_MAX, (sizeof(int8_t) * LEVEL24_CHUNK_NUM * CHUNK)); // ... 

    levelTable32_NH = (int8_t *)calloc((LEVEL32_CHUNK_NUM * CHUNK), sizeof(int8_t));
    memset(levelTable32_NH, PORT_MAX, (sizeof(int8_t) * LEVEL32_CHUNK_NUM * CHUNK)); // 12/25/2014

    worst_num = 0;
    statInit();
    updateInfoInit();
    /***************************************End*********************************************/

    m_pTrie->newPort = PORT_MAX;
    m_pTrie->oldPort = PORT_MAX;

#if 0
    ytOptLevelPushing(this->m_pTrie, 0, 0);
    ytGetNodeCounts();
    printf("The total number of solid Trie node is :\t%u,\noptFib.allNodeCount=%d\n",this->solidNodeCount, this->allNodeCount);
#endif
    struct subTriePushArg arg;
    arg.chunk_id = -1;
    arg.startLevel = 0;
    arg.targetLevel = 16;
    arg.ifCopy = false;
    subTrieVirtualPushing(this->m_pTrie, 0, PORT_MAX, 0, &arg);
    ytGetNodeCounts();
    printf("The total number of solid Trie node is :\t%u,\noptFib.allNodeCount=%d\n",this->solidNodeCount, this->allNodeCount);
}

CFib::~CFib(void)
{
}

void CFib::CreateNewNode(FibTrie* &pTrie)
{

    pTrie= (struct FibTrie*)malloc(FIBLEN);

    //initial
    pTrie->parent = NULL;
    pTrie->lchild = NULL;
    pTrie->rchild = NULL;
    pTrie->newPort = EMPTYHOP;
    pTrie->oldPort = EMPTYHOP;
    pTrie->nodeLevel = 0;
    pTrie->chunkID = -1;
    pTrie->ifpushed= false;
}

unsigned int CFib::btod(char *bstr)
{
    unsigned int d = 0;
    unsigned int len = (unsigned int)strlen(bstr);
    if (len > 32)
    {
        printf("too long\n");
        return -1; 
    }
    len--;

    unsigned int i = 0;
    for (i = 0; i <= len; i++)
    {
        d += (bstr[i] - '0') * (1 << (len - i));
    }

    return d;
}

bool CFib::IsLeaf(FibTrie * pNode)
{
    if (pNode->lchild==NULL && pNode->rchild==NULL)return true;
    else return false;	
}

void CFib::Pretraversal(FibTrie* pTrie)
{
    if (NULL==pTrie)return;

    allNodeCount++;
    if (pTrie->newPort!=0)solidNodeCount++;


    Pretraversal(pTrie->lchild);
    Pretraversal(pTrie->rchild);
}

void CFib::ytGetNodeCounts()
{
    allNodeCount=0;
    solidNodeCount=0;

    Pretraversal(m_pTrie);
}

//void CFib::OutputTrie(FibTrie* pTrie,string sFileName,string oldPortfile)
void CFib::OutputTrie(FibTrie* pTrie,string sFileName,string oldPortfile, bool ifOpt)
{
    ofstream fout(sFileName.c_str());
    GetTrieHops(pTrie,0,0,&fout,true, ifOpt);
    fout<<flush;
    fout.close();

    ofstream fout1(oldPortfile.c_str());
    GetTrieHops(pTrie,0,0,&fout1,false, ifOpt);
    fout1<<flush;
    fout1.close();
}

void CFib::OutputTrie_32(FibTrie* pTrie)
{
    ofstream fout("Prefixes_32.txt");
    GetTrieHops_32(pTrie,0,0,&fout,true);
    fout<<flush;
    fout.close();
}

void CFib::GetTrieHops_32(FibTrie* pTrie,unsigned int iVal,int iBitLen,ofstream* fout,bool ifnewPort)
{
    unsigned short portOut=PORT_MAX;

    if (-1!=pTrie->newPort)
    {
        portOut=pTrie->newPort;
    }

    if(portOut!=EMPTYHOP  && 32==iBitLen )
    {
        *fout<<iVal<<"\t"<<portOut<<endl;
    }

    iBitLen++;

    //try to handle the left sub-tree
    if(pTrie->lchild!=NULL)
    {
        GetTrieHops_32(pTrie->lchild,iVal,iBitLen,fout,ifnewPort);
    }
    //try to handle the right sub-tree
    if(pTrie->rchild!=NULL)
    {
        iVal += 1<<(32-iBitLen);
        GetTrieHops_32(pTrie->rchild,iVal,iBitLen,fout,ifnewPort);
    }
}

//get and output all the nexthop in Trie
//void CFib::GetTrieHops(FibTrie* pTrie,unsigned int iVal,int iBitLen,ofstream* fout,bool ifnewPort)
void CFib::GetTrieHops(FibTrie* pTrie,unsigned int iVal,int iBitLen,ofstream* fout,bool ifnewPort, bool ifOpt)
{
    int portOut=-1;
    if (true==ifnewPort)
        portOut=pTrie->newPort;
    else				
        portOut=pTrie->oldPort;

    //1 00000000  00010000   00000000
    if(portOut!=EMPTYHOP)
    {
        char strVal[50];
        memset(strVal,0,sizeof(strVal));
        //printf("%d.%d.%d.%d/%d\t%d\n",(iVal>>24),(iVal<<8)>>24,(iVal<<16)>>24,(iVal<<24)>>24,iBitLen,portOut);

        sprintf(strVal,"%d.%d.%d.%d/%d\t%d\n",(iVal>>24),(iVal<<8)>>24,(iVal<<16)>>24,(iVal<<24)>>24,iBitLen,portOut);
        *fout<<strVal;
    }
    else if (ifOpt && ifnewPort && (pTrie->nodeLevel == 16 || pTrie->nodeLevel == 24 || pTrie->nodeLevel == 32)) {
        char strVal[50];
        memset(strVal,0,sizeof(strVal));
        //printf("%d.%d.%d.%d/%d\t%d\n",(iVal>>24),(iVal<<8)>>24,(iVal<<16)>>24,(iVal<<24)>>24,iBitLen,portOut);

        printf("wrong!!!\n");
        //sprintf(strVal,"%d.%d.%d.%d/%d\t%d\n",(iVal>>24),(iVal<<8)>>24,(iVal<<16)>>24,(iVal<<24)>>24,iBitLen, GetAncestorHop(pTrie));
        sprintf(strVal,"%d.%d.%d.%d/%d\t%d\n",(iVal>>24),(iVal<<8)>>24,(iVal<<16)>>24,(iVal<<24)>>24,iBitLen, PORT_MAX);
        *fout<<strVal;
    }

    iBitLen++;

    //try to handle the left sub-tree
    if(pTrie->lchild!=NULL)
    {
        GetTrieHops(pTrie->lchild,iVal,iBitLen,fout,ifnewPort, ifOpt);
    }
    //try to handle the right sub-tree
    if(pTrie->rchild!=NULL)
    {
        iVal += 1<<(32-iBitLen);
        GetTrieHops(pTrie->rchild,iVal,iBitLen,fout,ifnewPort, ifOpt);
    }
}

//add a node in Rib tree
void CFib::AddNode(unsigned long lPrefix,unsigned int iPrefixLen,unsigned int iNextHop)
{
    //get the root of rib
    FibTrie* pTrie = m_pTrie;
    //locate every prefix in the rib tree
    for (unsigned int i=0; i<iPrefixLen; i++){
        //turn right
        if(((lPrefix<<i) & HIGHTBIT)==HIGHTBIT){
            //creat new node
            if(pTrie->rchild == NULL){
                FibTrie* pTChild = (struct FibTrie*)malloc(FIBLEN);
                //insert new node
                pTChild->parent = pTrie;
                pTChild->lchild = NULL;
                pTChild->rchild = NULL;
                pTChild->oldPort=0;
                pTChild->newPort=0;

                pTChild->nodeLevel = pTrie->nodeLevel + 1; // the level of the node
                pTChild->ifpushed = false;

                pTrie->rchild = pTChild;
            }
            //change the pointer
            pTrie = pTrie->rchild;
        }
        //turn left
        else{
            //if left node is empty, creat a new node
            if(pTrie->lchild == NULL){
                FibTrie* pTChild = (struct FibTrie*)malloc(FIBLEN);
                //insert new node
                pTChild->parent = pTrie;
                pTChild->lchild = NULL;
                pTChild->rchild = NULL;
                pTChild->oldPort = 0;
                pTChild->newPort = 0;

                pTChild->nodeLevel = pTrie->nodeLevel + 1;
                pTChild->ifpushed = false;

                pTrie->lchild = pTChild;
            }
            //change the pointer
            pTrie = pTrie->lchild;
        }
    }

    pTrie->newPort = iNextHop;
    pTrie->oldPort = iNextHop;
}

/*
 *PURPOSE: construct RIB tree from file
 *RETURN VALUES: number of items in rib file
 */
unsigned int CFib::BuildFibFromFile(string sFileName)
{
    unsigned int	iEntryCount=0;		//the number of items from file

    char			sPrefix[100];		//prefix from rib file
    unsigned long	lPrefix;			//the value of Prefix
    unsigned int	iPrefixLen;			//the length of PREFIX
    unsigned int	iNextHop;			//to store NEXTHOP in RIB file

    ifstream fin(sFileName.c_str());

    if (!fin) {
        fprintf(stderr, "Open file %s error!\n", sFileName.c_str());
        return 0;
    }

    while (!fin.eof()) {

        lPrefix = 0;
        iPrefixLen = 0;
        iNextHop = EMPTYHOP;

        memset(sPrefix,0,sizeof(sPrefix));

        fin >> sPrefix>> iNextHop;

        if (iNextHop > 127)
            continue;

        int iStart=0;				//the start point of PREFIX
        int iEnd=0;					//the start point of PREFIX
        int iFieldIndex = 3;		
        int iLen=(int)strlen(sPrefix);	//The length of PREFIX

        if (iLen>20)
        {
            continue;//maybe IPv6 address
        }

        if(iLen>0){
            iEntryCount++;
            for ( int i=0; i<iLen; i++ ){
                //get the first three sub-items
                if ( sPrefix[i] == '.' ){
                    iEnd = i;
                    string strVal(sPrefix+iStart,iEnd-iStart);
                    lPrefix += atol(strVal.c_str()) << (8 * iFieldIndex);
                    iFieldIndex--;
                    iStart = i+1;
                    i++;
                }
                if ( sPrefix[i] == '/' ){
                    //get the prefix length
                    iEnd = i;
                    string strVal(sPrefix+iStart,iEnd-iStart);
                    lPrefix += atol(strVal.c_str());
                    iStart = i+1;

                    i++;
                    strVal= string(sPrefix+iStart,iLen-1);
                    iPrefixLen=atoi(strVal.c_str());
                }
            }

            char insert_B[50];
            memset(insert_B,0,sizeof(insert_B));
            //insert the current node into Trie tree
            for (unsigned int yi = 0; yi < iPrefixLen; yi++)
            {
                //turn right
                if(((lPrefix << yi) & HIGHTBIT) == HIGHTBIT) insert_B[yi]='1';
                else insert_B[yi]='0';
            }                         

            if (iPrefixLen < 8) {
                printf("%d-%d; \n", iPrefixLen, iNextHop);
            }
            else
            {
                Update(iNextHop, insert_B, _INSERT, sPrefix, false);
            }

            //AddNode(lPrefix,iPrefixLen,iNextHop);
        }
    }
    fin.close();
    return iEntryCount;
}

/*
 *
 * default_port: the NH of the closest ancestor node
 *
 * */

unsigned int  CFib::subTrieUpdate(FibTrie* pTrie, unsigned int level,unsigned int default_port, unsigned int prefix, struct subTrieUpdateArg *arg) {
    unsigned int updateNum = 0;

    unsigned int lPrefix = prefix << 1;
    unsigned int rPrefix = (prefix<< 1) + 1;

    //if (default_port == 0) printf("The default port is 0!!!\n");

    // it's not the oldport shitdown, in the targetLevel (i.e., 16 or 24 or 32), write the array
    //if (level == 16 || level == 24 || level == 32) {
    if (level == arg->targetLevel) {
#if 0
        if (pTrie->chunkID != chunkBuffer.chunk_id) {
            printf("Erorr chunkID in subTrieUpdate!!!\n");
        }
#endif
        if (pTrie->newPort == 0) {
            printf("Error port in subTrieUpdate!!!\n");
        }
        chunkBuffer.chunk_id = pTrie->chunkID;

        chunkBuffer.nexthop[prefix & 255] = IsLeaf(pTrie) ? pTrie->newPort : (0 - pTrie->newPort);
        return 1;
    }

    pTrie->newPort = 0;

    if (pTrie->lchild != NULL) { // Update the left subtree recursively. IF. oldport == 0, UPDATE; ELSE. oldport shitdown
        if (pTrie->lchild->oldPort == 0) {
            pTrie->lchild->newPort = default_port; // Update the NH
            updateNum += subTrieUpdate(pTrie->lchild, level + 1, default_port, lPrefix, arg); // Update recursively
        }
        else {
            updateNum += subTrieUpdate(pTrie->lchild, level + 1, pTrie->lchild->oldPort, lPrefix, arg);	// oldport shitdown
        }
    }

    if (pTrie->rchild != NULL) {
        if (pTrie->rchild->oldPort == 0) {
            pTrie->rchild->newPort = default_port;
            updateNum += subTrieUpdate(pTrie->rchild, level + 1, default_port, rPrefix, arg); 
        }
        else {
            updateNum += subTrieUpdate(pTrie->rchild, level + 1, pTrie->rchild->oldPort, rPrefix, arg);
        }
    }

    return updateNum;
}

/*
 *  The default_port should be 127
 * */
void CFib::subTrieVirtualPushing(FibTrie* pTrie, unsigned int level,unsigned int default_port, unsigned int prefix, struct subTriePushArg *arg) {

    unsigned int lPrefix = prefix << 1;
    unsigned int rPrefix = (prefix << 1) + 1;
    //bool ifBlessLevelNode = false;	// Mark whether the node is in level 16 or 24 or 32

    if(NULL == pTrie) return;

    /*
     * Level 0 if targetLevel 16. For initialization only
     * Level 16 if targetLevel 24
     * Level 24 if targetLevel 32
     * */
    if (level == arg->startLevel) {
        default_port = PORT_MAX; // default_port should be 127
    }
    else if (level == arg->targetLevel) {	// Node in the targeted level. Store it temporarily
        if (level == 16) {
            if (pTrie->chunkID == -1) {
                pTrie->chunkID = prefix >> 8;
            }
        }
        else {
            if (pTrie->chunkID == -1) {
                pTrie->chunkID = arg->chunk_id;
            } else {
                printf("Error chunk id in level 24!!!\n");
            }

            if (arg->ifCopy) {
                if (!IsLeaf(pTrie)) {
                    printf("Error Copy: non-leaf node!!!\n");
                }

                if (pTrie->newPort == 0) printf("Error port in sub Virtual Pushing!!!\n");

                chunkBuffer.nexthop[prefix & 255] = pTrie->newPort;
            }
        }

        return;
    }
    else if (pTrie->newPort > 0) {
        default_port = pTrie->newPort;
    }

    //left child
    if (NULL == pTrie->lchild)
    {
        FibTrie* pTChild  = new FibTrie();//= (struct FibTrie*)malloc(FIBLEN);
        if (NULL==pTChild)
        {
            printf("malloc faild");
        }
        pTChild->parent = pTrie;
        pTChild->lchild = NULL;
        pTChild->rchild = NULL;
        pTChild->oldPort=0;
        pTChild->newPort = default_port;	// NH of the new node should be set as 'default_port'
        pTChild->nodeLevel = pTrie->nodeLevel + 1;
        pTChild->chunkID = -1;
        pTChild->ifpushed = true;
        pTrie->lchild = pTChild;
    }
    else if (0 == pTrie->lchild->newPort) pTrie->lchild->newPort = default_port;

    //right child
    if (NULL==pTrie->rchild)
    {
        FibTrie* pTChild = new FibTrie();//(struct FibTrie*)malloc(FIBLEN);
        if (NULL == pTChild)
        {
            printf("malloc faild");
        }
        pTChild->parent = pTrie;
        pTChild->lchild = NULL;
        pTChild->rchild = NULL;
        pTChild->oldPort = 0;
        pTChild->newPort = default_port;
        pTChild->nodeLevel = pTrie->nodeLevel + 1;
        pTChild->chunkID = -1;
        pTChild->ifpushed = true;
        pTrie->rchild = pTChild;
    }
    else if (0 == pTrie->rchild->newPort) pTrie->rchild->newPort = default_port;

    if (!(level == 16 || level == 24 || level == 32)) {
        pTrie->newPort = 0;
    }

    subTrieVirtualPushing(pTrie->lchild, level+1, default_port, lPrefix, arg);
    subTrieVirtualPushing(pTrie->rchild, level+1, default_port, rPrefix, arg);
}

void CFib::updateInfoInit() {
    memset(&chunkBuffer, 0, sizeof(chunkBuffer));
    chunkBuffer.chunk_id = -1;
#if 0
    memset(&chunkLevel16, 0, sizeof(chunkLevel16));
    memset(&chunkLevel24, 0, sizeof(chunkLevel24));
    memset(&chunkLevel32, 0, sizeof(chunkLevel32));

    chunkLevel16.chunk_id = -1;
    chunkLevel24.chunk_id = -1;
    chunkLevel32.chunk_id = -1;
#endif
}

int CFib::isCriticalLevel(int level) {
    if (level == 16 || level == 24 || level == 32)
        return 1;
    return 0;
}

void CFib::statInit() {
    memory_access = 0;
    memset(maStat, 0, sizeof(maStat));
}

#if 1
void CFib::Update(int insertport, char *insert_C, int operation_type, char* sprefix, bool ifStat)
{

    if (insertport == 0) printf("Error insert port: 0!\n");
    //cout << insertport << '\t' << insert_C << '\t' << operation_type << '\t' << sprefix << endl;
    //indicating to insertion, changing or deletion.
    int operation = -9;

    FibTrie *insertNode= m_pTrie;

    /*
     *  To levelpush the subtrie 
     * */

    FibTrie *levelNode16 = NULL;
    FibTrie *levelNode24 = NULL;

    int default_oldport = PORT_MAX;

    int prefixLen = (int)strlen(insert_C);

    if (prefixLen < 8) {	
        return;
    }

    if (insertport > 127) {
        return;
    }

    int bornStatus = -1;
    bool IfNewBornNode = false;
    bool ifSubTrieUpdate = false;

    //look up the location of the current node
    for (int i = 0; i < prefixLen; i++)	//0.0.0.0/0 IF len is 0, it is root node.
    {
        if ('0' == insert_C[i])
        {
            if (NULL== insertNode->lchild)
            {//turn left, if left child is empty, create new node
                if(_DELETE == operation_type)
                {
                    if (ifStat) invalid++;
                    return;
                }

                IfNewBornNode = true;
                FibTrie* pNewNode;
                CreateNewNode(pNewNode);
                pNewNode->parent = insertNode;
                pNewNode->nodeLevel = insertNode->nodeLevel + 1;		

                if (pNewNode->nodeLevel == 17) {
                    bornStatus = NB_16;
                }
                else if (pNewNode->nodeLevel == 25) {
                    if (bornStatus == NB_16) {
                        bornStatus = NB_BOTH;
                    }
                    else {
                        bornStatus = NB_24;
                    }
                }

                insertNode->lchild = pNewNode;
            }

            if (insertNode->oldPort != 0) default_oldport = insertNode->oldPort;

            // Guarantee each node at most affects 8 levels
            if (isCriticalLevel(insertNode->nodeLevel) && (insertNode->nodeLevel < prefixLen)) default_oldport = PORT_MAX;

            insertNode = insertNode->lchild;
        }
        else
        {
            //turn right, if left child is empty, create new node
            if (NULL== insertNode->rchild)
            {
                if(_DELETE == operation_type)	
                {
                    if (ifStat) invalid++;
                    return;
                }

                IfNewBornNode = true;
                FibTrie* pNewNode;
                CreateNewNode(pNewNode);
                pNewNode->parent = insertNode;
                pNewNode->nodeLevel = insertNode->nodeLevel + 1;		// 2014-4-18

                if (pNewNode->nodeLevel == 17) {
                    bornStatus = NB_16;
                }
                else if (pNewNode->nodeLevel == 25) {
                    if (bornStatus == NB_16) {
                        bornStatus = NB_BOTH;
                    }
                    else {
                        bornStatus = NB_24;
                    }
                }

                insertNode->rchild = pNewNode;
            }

            if (insertNode->oldPort !=0 ) default_oldport = insertNode->oldPort;

            // Guarantee each node at most affects 8 levels
            if (isCriticalLevel(insertNode->nodeLevel) && (insertNode->nodeLevel < prefixLen)) default_oldport = PORT_MAX;

            insertNode = insertNode->rchild;
        }

        if (insertNode->nodeLevel == 16) {
            levelNode16 = insertNode;
        }

        if (insertNode->nodeLevel == 24) {
            levelNode24 = insertNode;
        }
    }

    if(_DELETE != operation_type)		// it is not deletion
    {
        if (0 == insertNode->oldPort) {		//insertion
            operation = _INSERT;
        }
        else if (insertNode->oldPort == insertport)		// insertion port is the same as the original.
        {
            if (ifStat) invalid++;
            return;
        }
        else {	//changing
            operation = _CHANGE;
        }
    }
    else if (0 == insertNode->oldPort)	{	//Withdraw a prefix which is absent
        if (ifStat) invalid++;
        return;
    }
    else	{	//deletion
        operation = _DELETE;
    }

    int updateNumber = 0; 
    unsigned int origPrefix = btod(insert_C);

    unsigned int prefixLevel16 = 0;
    unsigned int prefixLevel24 = 0;
    unsigned int offset24 = 0;

    if (prefixLen >= 16) {
        prefixLevel16 = origPrefix >> (prefixLen - 16);
    }

    if (prefixLen >= 24) {
        prefixLevel24 = origPrefix >> (prefixLen - 24);
        offset24 = prefixLevel24 & 255;
    }

    struct subTrieUpdateArg stuArg;

    if (prefixLen <= 16) {
        stuArg.targetLevel = 16;
    }
    else if (prefixLen <= 24) {
        stuArg.targetLevel = 24;
    }
    else {
        stuArg.targetLevel = 32;
    }

    stuArg.length = stuArg.targetLevel - prefixLen;

    //printf("%s:\t%d\t%d\n", sprefix, insertport, stuArg.length);

    /*
     *  #memory accesses
     * */
    memory_access = 0;

    updateInfoInit();

    if (operation == _INSERT) {

        insertNode->oldPort = insertport;
        insertNode->newPort = insertport;

        if (IfNewBornNode) {    //beyond the outline
            struct subTriePushArg pushArg;

            if (ifStat && (bornStatus == NB_BOTH)) worst_num++;

            if (bornStatus == NB_16 || bornStatus == NB_BOTH) {
                // non-leaf node
                if (levelTable16_NH[prefixLevel16] < 0) {
                    printf("born status error in level 16!!!\n");
                    return;
                }

                // modify the NH and offset
                levelTable16_NH[prefixLevel16] = (0 - levelTable16_NH[prefixLevel16]); // non-leaf
                levelTable16_offset[prefixLevel16] = currentChunkNum24;
                chunkBuffer.chunk_id = currentChunkNum24;

                if (ifStat) {
                    memory_access += 1;
                }

                currentChunkNum24++;
                if (currentChunkNum24 > LEVEL24_CHUNK_NUM) {
                    printf("Too large currentChunkNum24!!!\n");
                    system("pause");
                }


                pushArg.chunk_id = chunkBuffer.chunk_id;
                pushArg.startLevel = 16;
                pushArg.targetLevel = 24;

                if (bornStatus == NB_BOTH) {
                    pushArg.ifCopy = false;
                } else {
                    pushArg.ifCopy = true;
                }

                if (levelNode16) {
                    subTrieVirtualPushing(levelNode16, levelNode16->nodeLevel, PORT_MAX, prefixLevel16, &pushArg);
                } else {
                    printf("Empty node in level 16!!!\n");
                    return;
                }
            }

            if (bornStatus == NB_24 || bornStatus == NB_BOTH) {
                unsigned short chunkID24 = levelTable16_offset[prefixLevel16];
                levelTable24_NH[(chunkID24 << 8) + offset24] = (0 - levelTable24_NH[(chunkID24 << 8) + offset24]);
                levelTable24_offset[(chunkID24 << 8) + offset24] = currentChunkNum32;
                chunkBuffer.chunk_id = currentChunkNum32;
                currentChunkNum32++;

                if (ifStat) {
                    memory_access += 2;
                }

                if (currentChunkNum32 > LEVEL32_CHUNK_NUM) {
                    printf("Too large currentChunkNum32!!!\n");
                    system("pause");
                }

                pushArg.chunk_id = chunkBuffer.chunk_id;
                pushArg.startLevel = 24;
                pushArg.targetLevel = 32;
                pushArg.ifCopy = true;

                if (levelNode24) {
                    subTrieVirtualPushing(levelNode24, levelNode24->nodeLevel, PORT_MAX, prefixLevel24, &pushArg);
                } else {
                    printf("Empty node in level 24!!!\n");
                }
            }

            if (bornStatus == -1){
                printf("unknown born status error!!!\n");
                return;
            }

            if (bornStatus == NB_16) {
                memcpy(&levelTable24_NH[(chunkBuffer.chunk_id << 8) + ((origPrefix << stuArg.length) & 255)], &chunkBuffer.nexthop[(origPrefix << stuArg.length) & 255], (PORT_LEN << stuArg.length));
            } else if (bornStatus == NB_24 || bornStatus == NB_BOTH) {
                memcpy(&levelTable32_NH[(chunkBuffer.chunk_id << 8) + ((origPrefix << stuArg.length) & 255)], &chunkBuffer.nexthop[(origPrefix << stuArg.length) & 255], (PORT_LEN << stuArg.length));
            }

            if (ifStat) {
                memory_access += ((PORT_LEN << stuArg.length) >> 3);
                if ((PORT_LEN << stuArg.length) % 8 != 0) {
                    memory_access += 1;
                }
            }
        }
        else if (insertNode->nodeLevel == 16 || insertNode->nodeLevel == 24 || insertNode->nodeLevel == 32)  { // No matter whether it's leaf node
            updateNumber = 1; // leaf node
            int8_t truePort = IsLeaf(insertNode) ? insertport : (0 - insertport); // leaf node > 0; non-leaf node < 0

            if(insertNode->nodeLevel == 16) {
                levelTable16_NH[prefixLevel16] = truePort;
                
                if (ifStat) {
                    memory_access += 1;
                }
            }
            else if (insertNode->nodeLevel == 24) {
                levelTable24_NH[(levelTable16_offset[prefixLevel16] << 8) + offset24] = truePort;
                
                if (ifStat) {
                    memory_access += 2;
                }
            }
            else {
                levelTable32_NH[(levelTable24_offset[(levelTable16_offset[prefixLevel16] << 8) + offset24] << 8) + (origPrefix & 255)] = insertport;
                if (ifStat) {
                    memory_access += 3;
                }
            }
        }
        else if (!IsLeaf(insertNode)) {		// internal node
            ifSubTrieUpdate = true;
            //updateNumber = subTrieUpdate(insertNode, insertport, origPrefix, true, &stuArg);		//updating the sub-trie
            updateNumber = subTrieUpdate(insertNode, insertNode->nodeLevel, insertport, origPrefix, &stuArg);
        }
        else {
            fprintf(stderr, "Error!!!\n");
        }
    }
    else if (operation == _CHANGE) { //	changing operation
        insertNode->oldPort = insertport;
        insertNode->newPort = insertport;
        ifSubTrieUpdate = true;
        //updateNumber = subTrieUpdate(insertNode, insertport, origPrefix, true, &stuArg);		//updating the sub-trie
        updateNumber = subTrieUpdate(insertNode, insertNode->nodeLevel, insertport, origPrefix, &stuArg);
    }
    else if (operation == _DELETE) {			// deletion
        insertNode->oldPort = 0;
        insertNode->newPort = default_oldport;
        ifSubTrieUpdate = true;
        //if (default_oldport == 0) getchar();//printf("delete!!!!!!!!!!!!!!!!!!!!!!!!!\n");

        //printf("nodeLevel = %d\t default_newport = %d\n", insertNode->nodeLevel, default_newport);
        //updateNumber = subTrieUpdate(insertNode, default_oldport, origPrefix, true, &stuArg);		//updating the sub-trie
        updateNumber = subTrieUpdate(insertNode, insertNode->nodeLevel, default_oldport, origPrefix, &stuArg);
    }

    if (ifSubTrieUpdate) {
        if (stuArg.targetLevel == 16) {
            memcpy(&levelTable16_NH[(chunkBuffer.chunk_id << 8) + ((origPrefix << stuArg.length) & 255)], &chunkBuffer.nexthop[(origPrefix << stuArg.length) & 255], (PORT_LEN << stuArg.length));

        }
        else if (stuArg.targetLevel == 24) {
            memcpy(&levelTable24_NH[(chunkBuffer.chunk_id << 8) + ((origPrefix << stuArg.length) & 255)], &chunkBuffer.nexthop[(origPrefix << stuArg.length) & 255], (PORT_LEN << stuArg.length));
        }
        else {
            memcpy(&levelTable32_NH[(chunkBuffer.chunk_id << 8) + ((origPrefix << stuArg.length) & 255)], &chunkBuffer.nexthop[(origPrefix << stuArg.length) & 255], (PORT_LEN << stuArg.length));
        }


        if (ifStat) {
            memory_access += ((PORT_LEN << stuArg.length) >> 3);
            if ((PORT_LEN << stuArg.length) % 8 != 0) {
                memory_access += 1;
            }
        }
    }

    if (!IfNewBornNode && (updateNumber != ((1 << stuArg.length)))) {
        cout << "Update Error!\tType:" << operation_type << "\t" << sprefix << "\t" << insertport<<"\t" << updateNumber << ":\t" << (1 << stuArg.length) << endl;
    }

    if (ifStat) {
        if (memory_access < 33) {
            maStat[memory_access]++;
        }
        else {
            printf("Error memory access stat!!!\n");
        }
    }
}
#endif

#if 0
void CFib::Update(int insertport, char *insert_C, int operation_type) {

    if (insertport > 127)
        return;

    FibTrie *insertNode = m_pTrie;

    /*
     *  To levelpush the subtrie 
     * */

    FibTrie *levelNode16 = NULL;
    FibTrie *levelNode24 = NULL;

    int bornStatus = -1;
    bool IfNewBornNode = false;
    bool ifSubTrieUpdate = false;

    for (int i = 0; i < (int)strlen(insert_C); i++)
    {
        if ('0'==insert_C[i])
        {
            if (NULL==insertNode->lchild)
            {//turn left, if left child is empty, create new node
                if(_DELETE==operation_type)	
                {
                    invalid++;
                    return;
                }

                newBornNode++;

                IfNewBornNode = true;
                FibTrie* pNewNode;
                CreateNewNode(pNewNode);
                pNewNode->parent = insertNode;
                pNewNode->nodeLevel = insertNode->nodeLevel + 1;		// 
                insertNode->lchild = pNewNode;
            }

            insertNode=insertNode->lchild;
        }
        else
        {
            //turn right, if left child is empty, create new node
            if (NULL==insertNode->rchild)
            {
                if(_DELETE==operation_type)	
                {
                    invalid++;
                    return;
                }

                newBornNode++;
                IfNewBornNode=true;
                FibTrie* pNewNode;
                CreateNewNode(pNewNode);
                pNewNode->parent=insertNode;
                pNewNode->nodeLevel = insertNode->nodeLevel + 1;		// 
                insertNode->rchild=pNewNode;
            }

            insertNode=insertNode->rchild;
        }
    }

    // Update the basic trie
    if(_NOT_DELETE == operation_type) {
        if (insertNode->oldPort == insertport) {
            return;
        }
        else {
            insertNode->oldPort = insertport;
            insertNode->newPort = insertport;
        }
    }
    else {
        insertNode->oldPort = 0;
        insertNode->newPort = 0;
    }
}
#endif

void CFib::LeafPush(FibTrie* pTrie, int depth)
{

    if (NULL==pTrie)return;

    if (NULL==pTrie->lchild && NULL==pTrie->rchild)return;

    if (0==pTrie->newPort)
    {
        LeafPush(pTrie->lchild,depth);
        LeafPush(pTrie->rchild,depth);
        return;
    }

    if (NULL!=pTrie->lchild && NULL==pTrie->rchild)
    {
        FibTrie* pTChild = (struct FibTrie*)malloc(FIBLEN);
        pTChild->parent = pTrie;
        pTChild->lchild = NULL;
        pTChild->rchild = NULL;
        pTChild->oldPort= 0;
        pTChild->newPort= pTrie->newPort;
        pTChild->ifpushed= true;

        pTrie->rchild=pTChild;

        if (0==pTrie->lchild->newPort)pTrie->lchild->newPort=pTrie->newPort;

        LeafPush(pTrie->lchild,depth);

        pTrie->newPort=0;
    }

    else if (NULL!=pTrie->rchild && NULL==pTrie->lchild)
    {
        FibTrie* pTChild = (struct FibTrie*)malloc(FIBLEN);
        pTChild->parent = pTrie;
        pTChild->lchild = NULL;
        pTChild->rchild = NULL;
        pTChild->oldPort=0;
        pTChild->newPort=pTrie->newPort;
        pTChild->ifpushed= true;

        pTrie->lchild=pTChild;

        if (0==pTrie->rchild->newPort)pTrie->rchild->newPort=pTrie->newPort;

        LeafPush(pTrie->rchild,depth);

        pTrie->newPort=0;
    }

    else 
    {
        if (0==pTrie->rchild->newPort)pTrie->rchild->newPort=pTrie->newPort;
        if (0==pTrie->lchild->newPort)pTrie->lchild->newPort=pTrie->newPort;

        LeafPush(pTrie->lchild,depth);
        LeafPush(pTrie->rchild,depth);

        pTrie->newPort=0;
    }
}


void CFib::LevelStatistic(FibTrie* pTrie, unsigned int level)
{
    if(NULL == pTrie)return;
    if(pTrie->newPort != 0)num_level[level]++;

    LevelStatistic(pTrie->lchild, level+1);
    LevelStatistic(pTrie->rchild, level+1);
}

void CFib::ytLevelPushing(FibTrie* pTrie, unsigned int level,unsigned int default_port)
{
    if(NULL == pTrie) return;

    if((level == 16 || level == 24 || level == 32) && IsLeaf(pTrie)) return;

    if (pTrie->newPort > 0) default_port = pTrie->newPort;

    //left child
    if (NULL == pTrie->lchild)
    {
        //FibTrie* pTChild  = new FibTrie();//= (struct FibTrie*)malloc(FIBLEN);
        FibTrie* pTChild  = (struct FibTrie*)malloc(FIBLEN);
        if (NULL==pTChild)
        {
            printf("malloc faild");
        }
        pTChild->parent = pTrie;
        pTChild->lchild = NULL;
        pTChild->rchild = NULL;
        pTChild->oldPort=0;
        pTChild->newPort = pTrie->newPort;
        pTChild->nodeLevel = pTrie->nodeLevel + 1;
        pTChild->ifpushed = true;
        pTrie->lchild = pTChild;
    }
    else if (0 == pTrie->lchild->newPort) pTrie->lchild->newPort = default_port;

    //right child
    if (NULL==pTrie->rchild)
    {
        //FibTrie* pTChild = new FibTrie();//(struct FibTrie*)malloc(FIBLEN);
        FibTrie* pTChild = (struct FibTrie*)malloc(FIBLEN);

        if (NULL == pTChild)
        {
            printf("malloc faild");
        }

        pTChild->parent = pTrie;
        pTChild->lchild = NULL;
        pTChild->rchild = NULL;
        pTChild->oldPort=0;
        pTChild->newPort=pTrie->newPort;
        pTChild->nodeLevel = pTrie->nodeLevel + 1;
        pTChild->ifpushed = true;
        pTrie->rchild = pTChild;
    }
    else if (0 == pTrie->rchild->newPort) pTrie->rchild->newPort = default_port;

    pTrie->newPort = 0;

    ytLevelPushing(pTrie->lchild, level+1,default_port);
    ytLevelPushing(pTrie->rchild, level+1,default_port);
}

/*
 *	default_port: The NH of the closest ancestor
 *	
 **/

void CFib::ytOptLevelPushing(FibTrie* pTrie, unsigned int level, unsigned int default_port) {

    bool ifBlessLevelNode = false;	// Mark whether the node is in level 16 or 24 or 32

    if(NULL == pTrie) return;

    if (level == 0) {
        //default_port = 0;
        default_port = PORT_MAX; //127.  09/13/2014
    }
    else if (level == 16 || level == 24 || level == 32) {	//leaf nodes. Set the NH

        ifBlessLevelNode = true;
        //pTrie->newPort = default_port;

        if (IsLeaf(pTrie)) {
            return;
        }
        else {
            //default_port = 0;
            default_port = PORT_MAX; //127.  09/13/2014
        }
    }
    else if (pTrie->newPort > 0) {
        default_port = pTrie->newPort;
    }

    //left child
    if (NULL == pTrie->lchild)
    {
        FibTrie* pTChild  = new FibTrie();//= (struct FibTrie*)malloc(FIBLEN);
        if (NULL==pTChild)
        {
            printf("malloc faild");
        }
        pTChild->parent = pTrie;
        pTChild->lchild = NULL;
        pTChild->rchild = NULL;
        pTChild->oldPort=0;
        pTChild->newPort = default_port;
        pTChild->nodeLevel = pTrie->nodeLevel + 1;
        pTChild->chunkID = -1;
        pTChild->ifpushed = true;
        pTrie->lchild = pTChild;
    }
    else if (0 == pTrie->lchild->newPort) pTrie->lchild->newPort = default_port;

    //right child
    if (NULL==pTrie->rchild)
    {
        FibTrie* pTChild = new FibTrie();//(struct FibTrie*)malloc(FIBLEN);
        if (NULL == pTChild)
        {
            printf("malloc faild");
        }
        pTChild->parent = pTrie;
        pTChild->lchild = NULL;
        pTChild->rchild = NULL;
        pTChild->oldPort = 0;
        pTChild->newPort = default_port;
        pTChild->nodeLevel = pTrie->nodeLevel + 1;
        pTChild->chunkID = -1;
        pTChild->ifpushed = true;
        pTrie->rchild = pTChild;
    }
    else if (0 == pTrie->rchild->newPort) pTrie->rchild->newPort = default_port;

    /*
     * IF. node is in level 16 or 24 or 32, reserve the new port
     * ELSE. set its NH to 0
     *
     * */

    if (!ifBlessLevelNode) { 
        pTrie->newPort = 0;
    }

    ytOptLevelPushing(pTrie->lchild, level+1,default_port);
    ytOptLevelPushing(pTrie->rchild, level+1,default_port);
}

unsigned int CFib::TWMX( unsigned int a)
{
    a = (a ^ 61) ^ (a >> 16);
    a = a + (a << 3);
    a = a ^ (a >> 4);
    a = a * 0x27d4eb2d;
    a = a ^ (a >> 15);
    return a;
}

bool CFib::isCorrectTrie(FibTrie *pTrie)
{
    if (pTrie == NULL) {
        printf("1\n");
        return false;
    }

    int level = pTrie->nodeLevel;

    if (IsLeaf(pTrie)) {	// leaf nodes must be in level 16 or 24 or 32
        if (level == 16 || level == 24 || level == 32) {
            if (pTrie->newPort == 0) return false;

            if (pTrie->oldPort != 0) { 
                if (pTrie->newPort == pTrie->oldPort)
                    return true;
                else { 
                    printf("Old port doesn't match the new port!!!\n");
                    return false;
                }
            } else {
                return true;
            }
        } else {
            printf("leaf node in level except for 16, 24 or 32!!!\n");
            return false;
        }
    }
    else {	// non-leaf nodes

        if (pTrie->newPort != 0 && (level != 0 && level != 16 && level != 24 && level != 32)) {
            printf("newPort != 0!!!\n");
            return false;
        }

        if (!pTrie->rchild || !pTrie->lchild) {	// empty child nodes
            printf("Empty child nodes!!!\n");
            return false;
        }

        if (isCorrectTrie(pTrie->rchild) && isCorrectTrie(pTrie->lchild)) {	// the correctness of the left- and right- subtries
            return true;
        }
        else {
            printf("2\n");
            return false;
        }
    }
}

unsigned int CFib::GetAncestorHop(FibTrie* pTrie)
{
    unsigned int iHop = EMPTYHOP;
    if(pTrie != NULL){
        pTrie=pTrie->parent;
        if(pTrie!=NULL){
            iHop = pTrie->oldPort;
            if(iHop==EMPTYHOP){
                iHop=GetAncestorHop(pTrie);
            }
        }
    }
    return iHop;
}

unsigned int CFib::getTrueHop(FibTrie* pTrie, int level) {
    unsigned int iHop = PORT_MAX;
    if(pTrie != NULL){
        if (pTrie->nodeLevel == level) {
            return pTrie->newPort;
        }

        pTrie = pTrie->parent;
        return getTrueHop(pTrie, level);
    }

    return iHop;
}

void CFib::buildLookupTable(FibTrie* pTrie)
{
    if (pTrie == NULL)
        return;

    int level = pTrie->nodeLevel;

    if (level == 16) {

        if (IsLeaf(pTrie)) {
            levelTable16_NH[level16_node_num] = pTrie->newPort;
        }
        else {
            levelTable16_NH[level16_node_num] = (0 - pTrie->newPort);
            levelTable16_offset[level16_node_num] = level16_nonleaf_num;
            level16_nonleaf_num++;
        }

        level16_node_num++;
    }
    else if (level == 24) {
        if (IsLeaf(pTrie)) {
            levelTable24_NH[level24_node_num] = pTrie->newPort;
        }
        else {
            levelTable24_NH[level24_node_num] = (0 - pTrie->newPort);
            levelTable24_offset[level24_node_num] = level24_nonleaf_num;
            level24_nonleaf_num++;
        }

        level24_node_num++;
    }
    else if (level == 32) {
        levelTable32_NH[level32_node_num] = pTrie->newPort;
        level32_node_num++;
    }

    if (pTrie->lchild) {
        buildLookupTable(pTrie->lchild);
    }

    if (pTrie->rchild) {
        buildLookupTable(pTrie->rchild);
    }
}

unsigned int CFib::optTrieLookup(char * insert_C) {

    int nextHop = PORT_MAX;//init the return value
    FibTrie *insertNode = m_pTrie;

    int len=(int) strlen(insert_C);

    for (int i=0; i < (len + 1);i++)
    {		
        if ('0' == insert_C[i])
        {//if 0, turn left
            if (NULL != insertNode->lchild)	
            {
                insertNode = insertNode->lchild;
            }
            else {
                break;
            }
        }
        else
        {//if 1, turn right
            if (NULL != insertNode->rchild) {
                insertNode = insertNode->rchild;
            }
            else {
                break;
            }
        }

        if (insertNode->oldPort != 0)	{
            nextHop = insertNode->oldPort;
            //printf("%d --- %d\n",i , nextHop);
        }
    }

    return	nextHop;
}

unsigned int CFib::trieLookup(char * insert_C) {

    int nextHop = PORT_MAX;//init the return value
    FibTrie *insertNode = m_pTrie;

    int len=(int) strlen(insert_C);

    for (int i=0; i < (len + 1);i++)
    {		
        if ('0' == insert_C[i])
        {//if 0, turn left
            if (NULL != insertNode->lchild)	
            {
                insertNode = insertNode->lchild;
            }
            else {
                break;
            }
        }
        else
        {//if 1, turn right
            if (NULL != insertNode->rchild) {
                insertNode = insertNode->rchild;
            }
            else {
                break;
            }
        }

        if (insertNode->newPort != 0)	{
            nextHop = insertNode->newPort;
        }
    }

    return	nextHop;
}

unsigned int CFib::fqbblessLookup(unsigned int ip) {
    register int LMPort = PORT_MAX;
    register int tmp = 0;
    register unsigned short offset16 = ip >> 16;
    register unsigned short level = 0;

    if ((tmp = levelTable16_NH[offset16]) > 0) {
        return tmp;
    }

    LMPort = -tmp;

    if ((tmp = levelTable24_NH[((levelTable16_offset[offset16]) << 8) + (ip<<16>>24)]) > 0) {
        LMPort= (tmp==PORT_MAX? LMPort : tmp);
        return LMPort;
    }

    LMPort= (tmp==-PORT_MAX? LMPort : -tmp);

    if ((tmp = levelTable32_NH[((levelTable24_offset[((levelTable16_offset[offset16]) << 8) + (ip<<16>>24)]) << 8) + (ip & 255)]) > 0) {
        LMPort= (tmp==PORT_MAX? LMPort : tmp);
        return LMPort;
    }

    LMPort= (tmp==-PORT_MAX? LMPort : -tmp);

    return LMPort;
}

unsigned int CFib::blessLookup(unsigned int ip) {
    register int LMPort = PORT_MAX;
    register int tmp = 0;
    register unsigned short offset16 = ip >> 16;
    //register unsigned short level = 0;

    if ((tmp = levelTable16_NH[offset16]) > 0) {
        return tmp;
    }

    LMPort = -tmp;

    if ((tmp = levelTable24_NH[((levelTable16_offset[offset16]) << 8) + (ip<<16>>24)]) > 0) {
        LMPort= (tmp==127? LMPort : tmp);
        return LMPort;
    }

    LMPort= (tmp==-127? LMPort : -tmp);

    tmp = levelTable32_NH[((levelTable24_offset[((levelTable16_offset[offset16]) << 8) + (ip<<16>>24)]) << 8) + (ip & 255)];
    LMPort= (tmp==127? LMPort : tmp);

    return LMPort;
}

void CFib::TrafficRead(char *traffic_file)
{
    //unsigned int *traffic=new unsigned int[TRACE_READ];
    int return_value=-1;
    unsigned int traceNum=0;

    for (int i=0;i<TRACE_READ;i++)
    {
        trafficIn[i]=0;
    }
    //first read the trace...
    ifstream fin(traffic_file);
    if (!fin)
    {
        printf("traffic file open failed!\n");
        return;
    }
    fin>>traceNum;

    int TraceLine=0;
    unsigned int IPtmp=0;
    while (!fin.eof() && TraceLine<TRACE_READ )
    {
        fin>>IPtmp;
        trafficIn[TraceLine]=IPtmp;
        TraceLine++;
    }
    fin.close();
    printf("    trace read complete...\n");

    if (TraceLine<TRACE_READ)
    {
        printf("not enough %d\n",TraceLine);
    }
}

void CFib::TestLookupSpeed(char *traffi_fil, char* fib_fil)
{	
    printf("\n\nBless algorithm starts...\n\n");

    BuildFibFromFile(fib_fil);
    ytOptLevelPushing(m_pTrie, 0, 0);
    buildLookupTable(m_pTrie);
    TrafficRead(traffi_fil);

	register short tmp=0;
	register unsigned char LMPort = PORT_MAX;

    LARGE_INTEGER frequence,p0,p1;
    if(!QueryPerformanceFrequency(&frequence))return;
    QueryPerformanceCounter(&p0);

    for (unsigned int j=0;j<10000;j++)
    {
        for (unsigned int i=0;i<TRACE_READ;i++)
        {
            if ( (tmp = levelTable16_NH[trafficIn[i] >> 16])> 0) 
            {
                LMPort = tmp;
            }
            else
            {
                LMPort = -tmp;
                if ((tmp = levelTable24_NH[((levelTable16_offset[trafficIn[i] >> 16]) << 8) + (trafficIn[i]<<16>>24)]) > 0) 
                {
                    LMPort= tmp== 127? LMPort : tmp;
                }
                else
                {
                    LMPort= tmp==-127? LMPort : -tmp;
                    tmp = levelTable32_NH[((levelTable24_offset[((levelTable16_offset[trafficIn[i] >> 16]) << 8) + (trafficIn[i]<<16>>24)]) << 8) + (trafficIn[i] & 255)];
                    LMPort= tmp== 127? LMPort : tmp;
                }
            }
        }
    }

    QueryPerformanceCounter(&p1);
    long long Lookuptime=1000000*(p1.QuadPart-p0.QuadPart)/frequence.QuadPart;

     printf("\tLMPport=%d\n\tLookup time=%lld us\n\tThroughput is:\t %.3f Mpps\n", LMPort, Lookuptime, 10000.0*TRACE_READ/Lookuptime);

}
