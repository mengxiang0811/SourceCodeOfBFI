#include "Fib.h"
#include <iostream>
//#include <time.h>
#include <time.h>

//#include <windows.h>

#define FIBLEN				sizeof(struct FibTrie)		//size of each Trie node
#define EMPTYHOP			0							//Trie node doesnt't has a next hop

#define TRACE_READ 100000
unsigned int trafficIn[TRACE_READ];

const char * result_compare="result_compare.txt";
const char * hop_count="hop_count.txt";

//#define  0 


void CFib::ytLevelPushing(FibTrie* pTrie, unsigned int level, struct pRuleSet default_rule) {

	unsigned int i;
	unsigned int ins_num;
	if (NULL == pTrie) return;

	if (level == 0) {
		default_rule.minID = pTrie->RS->minID;
		default_rule.rules->clear();

		ins_num = pTrie->RS->rules.size();

		for (i = 0; i < ins_num; i++) {
			default_rule.rules->push_back(pTrie->RS->rules[i]);
		}

		pTrie->RS->rules.clear();

		//left child
		if (NULL == pTrie->lchild)
		{
			FibTrie* pTChild; // = new FibTrie();//= (struct FibTrie*)malloc(FIBLEN);
			CreateNewNode(pTChild);
			pTChild->nodeLevel = pTrie->nodeLevel + 1;
			pTrie->lchild = pTChild;
		}

		//right child
		if (NULL == pTrie->rchild)
		{
			FibTrie* pTChild; //= new FibTrie();//(struct FibTrie*)malloc(FIBLEN);
			CreateNewNode(pTChild);
			pTChild->nodeLevel = pTrie->nodeLevel + 1;
			pTrie->rchild = pTChild;
		}

		ytLevelPushing(pTrie->lchild, level + 1, default_rule);
		ytLevelPushing(pTrie->rchild, level + 1, default_rule);

	}
	else if (level == 16 || level == 24 || level == 32) {

		if (IsLeaf(pTrie)) {
			if (default_rule.minID < pTrie->RS->minID)
				pTrie->RS->minID = default_rule.minID;

			for (i = 0; i < default_rule.rules->size(); i++)
				pTrie->RS->rules.push_back(default_rule.rules->at(i));

			return;
		}
		else {

			if (default_rule.minID >  pTrie->RS->minID)
				default_rule.minID = pTrie->RS->minID;

			ins_num = pTrie->RS->rules.size();

			for (i = 0; i < ins_num; i++)
				default_rule.rules->push_back(pTrie->RS->rules[i]);

			pTrie->RS->rules.clear();

			//left child
			if (NULL == pTrie->lchild)
			{
				FibTrie* pTChild; // = new FibTrie();//= (struct FibTrie*)malloc(FIBLEN);
				CreateNewNode(pTChild);
				pTChild->nodeLevel = pTrie->nodeLevel + 1;
				pTrie->lchild = pTChild;
			}

			//right child
			if (NULL == pTrie->rchild)
			{
				FibTrie* pTChild; //= new FibTrie();//(struct FibTrie*)malloc(FIBLEN);
				CreateNewNode(pTChild);
				pTChild->nodeLevel = pTrie->nodeLevel + 1;
				pTrie->rchild = pTChild;
			}

			ytLevelPushing(pTrie->lchild, level + 1, default_rule);
			ytLevelPushing(pTrie->rchild, level + 1, default_rule);

			for (i = 0; i < ins_num; i++)
				default_rule.rules->pop_back();
		}

	}
	else{
	
		ins_num = pTrie->RS->rules.size();

		if (ins_num > 0) { 

			if (default_rule.minID > pTrie->RS->minID)
				default_rule.minID = pTrie->RS->minID;

			for (i = 0; i < ins_num; i++)
				default_rule.rules->push_back(pTrie->RS->rules[i]);

			pTrie->RS->rules.clear();
		}

		//left child
		if (NULL == pTrie->lchild)
		{
			FibTrie* pTChild;
			CreateNewNode(pTChild);
			pTChild->nodeLevel = pTrie->nodeLevel + 1;
			pTrie->lchild = pTChild;
		}

		//right child
		if (NULL == pTrie->rchild)
		{
			FibTrie* pTChild;
			CreateNewNode(pTChild);
			pTChild->nodeLevel = pTrie->nodeLevel + 1;
			pTrie->rchild = pTChild;
		}

		ytLevelPushing(pTrie->lchild, level + 1, default_rule);
		ytLevelPushing(pTrie->rchild, level + 1, default_rule);

		for (i = 0; i < ins_num; i++)
			default_rule.rules->pop_back();
	}

}

void CFib::basicLookup(unsigned int ip, struct lookupRes &lrs) {

	//unsigned short offset16 = ip >> 16;
	//unsigned short level = 0;

	//if (levelTable16_offset[offset16] == 0) {
	//	lrs.minID = levelTable16_NH[offset16].minID;
	//	lrs.rules = levelTable16_NH[offset16].rules;
	//	lrs.rBF = levelTable16_BF[offset16];
	//	return;
	//}

	//unsigned int offset24 = ((levelTable16_offset[offset16] - 1) << 8) + (ip << 16 >> 24);

	//if ((levelTable24_offset[offset24]) == 0) {
	//	lrs.minID = levelTable24_NH[offset24].minID;
	//	lrs.rules = levelTable24_NH[offset24].rules;
	//	lrs.rBF = levelTable24_BF[offset24];
	//	return;
	//}

	//unsigned int offset32 = ((levelTable24_offset[offset24] - 1) << 8) + (ip & 255);

	//lrs.minID = levelTable32_NH[offset32].minID;
	//lrs.rules = levelTable32_NH[offset32].rules;
	//lrs.rBF = levelTable32_BF[offset32];
}

void CFib::createChunkBF() {
	unsigned int i = 0;

	//cout << "In createChunkBF, chunk24_num = " << Chunk24_num << endl;

	//cout << "Chunk_24num * CHUNK: " << Chunk24_num * CHUNK << endl;

//	cout << "In createChunkBF, chunk32_num = " << Chunk32_num << endl;

//	cout << "Chunk_32num * CHUNK: " << Chunk32_num * CHUNK << endl;

	levelTable16_BF = (std_bf **)calloc(LEVEL16_ELE_NUM, sizeof(std_bf *));

	for (i = 0; i < LEVEL16_ELE_NUM; i++) {
		levelTable16_BF[i] = new std_bf(BF_ELE, BF_HF);
	}

	levelTable16_NH = (struct pRuleSet*)calloc(LEVEL16_ELE_NUM, sizeof(struct pRuleSet));
	levelTable16_offset = (unsigned int *)calloc(LEVEL16_ELE_NUM, sizeof(unsigned int));

	levelTable24_BF = (std_bf **)calloc((Chunk24_num * CHUNK), sizeof(std_bf *));

	for (i = 0; i < (Chunk24_num * CHUNK); i++) {
		levelTable24_BF[i] = new std_bf(BF_ELE, BF_HF);
	}

	levelTable24_NH = (struct pRuleSet*)calloc((Chunk24_num * CHUNK), sizeof(struct pRuleSet));
	levelTable24_offset = (unsigned int *)calloc((Chunk24_num * CHUNK), sizeof(unsigned int));

	levelTable32_BF = (std_bf **)calloc((Chunk32_num * CHUNK), sizeof(std_bf *));

	for (i = 0; i < (Chunk32_num * CHUNK); i++) {
		levelTable32_BF[i] = new std_bf(BF_ELE, BF_HF);
	}

	levelTable32_NH = (struct pRuleSet*)calloc((Chunk32_num * CHUNK), sizeof(struct pRuleSet));
}

CFib::CFib(void)
{
	//initial the root of the Trie tree
	CreateNewNode(m_pTrie);

	allNodeCount=0;			//to count all the nodes in Trie tree, including empty node
	solidNodeCount=0;		//to count all the solid nodes in Trie tree
    ruleCount = 0;


	prefix32_num=0;
	lenofchain=0;

	memset(num_level, 0, sizeof(num_level));

	memset(LevelPort,0,sizeof(LevelPort));

	currentLenBit24=0;

	/***********************************New Update***************************************/
	invalid = 0;

	currentChunkNum24 = 0;
	currentChunkNum32 = 0;

	level16_nonleaf_num = 1;
	level24_nonleaf_num = 1;

	level16_node_num = 0;
	level24_node_num = 0;
	level32_node_num = 0;

	/***************************************End*********************************************/
}

CFib::~CFib(void)
{
}

void CFib::CreateNewNode(FibTrie* &pTrie)
{
	
	pTrie= (struct FibTrie*)malloc(FIBLEN);

	//initial
	pTrie->lchild = NULL;
	pTrie->rchild = NULL;

	pTrie->RS = (struct ruleSet*)calloc(1, sizeof(struct ruleSet));

    pTrie->RS->minID = (unsigned short)-1;
    pTrie->RS->rules.clear();

	pTrie->nodeLevel = 0;
	pTrie->chunkID = -1;
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

/*
void CFib::Pretraversal(FibTrie* pTrie)
{
	if (NULL==pTrie)return;

	allNodeCount++;

	if (pTrie->newRS->rules.size() != 0)solidNodeCount++; // ruleID.size() == 0 means that there is no such rules


	Pretraversal(pTrie->lchild);
	Pretraversal(pTrie->rchild);
}*/

void CFib::Pretraversal(FibTrie* pTrie, int level)
{
	if (NULL == pTrie)return;

	allNodeCount++;

	if (pTrie->RS->rules.size() != 0)
		solidNodeCount++; // ruleID.size() == 0 means that there is no such rules

	if (16 == level && !IsLeaf(pTrie))
		Chunk24_num++;
	else if (24 == level && !IsLeaf(pTrie))
		Chunk32_num++;

	Pretraversal(pTrie->lchild, level + 1);
	Pretraversal(pTrie->rchild, level + 1);
}

/*
void CFib::ytGetNodeCounts()
{
	allNodeCount=0;
	solidNodeCount=0;

	Pretraversal(m_pTrie);
}*/

void CFib::ytGetNodeCounts()
{
	allNodeCount = 0;
	solidNodeCount = 0;
	Chunk24_num = 3;
	Chunk32_num = 3;
	Pretraversal(m_pTrie, 0);
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
#if 0
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
#endif
}

//get and output all the nexthop in Trie
//void CFib::GetTrieHops(FibTrie* pTrie,unsigned int iVal,int iBitLen,ofstream* fout,bool ifnewPort)
void CFib::GetTrieHops(FibTrie* pTrie,unsigned int iVal,int iBitLen,ofstream* fout,bool ifnewPort, bool ifOpt)
{
#if 0
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

		sprintf(strVal,"%d.%d.%d.%d/%d\t%d\n",(iVal>>24),(iVal<<8)>>24,(iVal<<16)>>24,(iVal<<24)>>24,iBitLen, GetAncestorHop(pTrie));
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
#endif

}

//add a node in Rib tree
void CFib::AddNode(unsigned long lPrefix,unsigned int iPrefixLen,unsigned int iRuleID)
{
	//get the root of rib
	FibTrie* pTrie = m_pTrie;

    if (iPrefixLen > 32) {
        cout << "Error prefix length!" << endl;
        return;
    }

	//locate every prefix in the rib tree
	for (unsigned int i = 0; i < iPrefixLen; i++) {
		//turn right
		if(((lPrefix << i) & HIGHTBIT) == HIGHTBIT) {
			//creat new node
			if(pTrie->rchild == NULL){
				FibTrie* pTChild; //= (struct FibTrie*)malloc(FIBLEN);
                CreateNewNode(pTChild);
				//insert new node
				pTChild->lchild = NULL;
				pTChild->rchild = NULL;

				pTChild->RS->rules.clear();
				pTChild->nodeLevel = pTrie->nodeLevel + 1; // the level of the node
				pTrie->rchild = pTChild;
			}

			//change the pointer
			pTrie = pTrie->rchild;
		}
		//turn left
		else{
			//if left node is empty, creat a new node
			if(pTrie->lchild == NULL) {
				FibTrie* pTChild; //= (struct FibTrie*)malloc(FIBLEN);
                CreateNewNode(pTChild);
				//insert new node
				pTChild->lchild = NULL;
				pTChild->rchild = NULL;

				pTChild->RS->rules.clear();
				pTChild->nodeLevel = pTrie->nodeLevel + 1;
				pTrie->lchild = pTChild;
			}
			//change the pointer
			pTrie = pTrie->lchild;
		}
	}

	//pTrie->newRS->rules.push_back(iRuleID);
    if (pTrie->RS->minID > iRuleID) {
        pTrie->RS->minID = iRuleID;
    }
	pTrie->RS->rules.push_back(iRuleID);
}

/*
	*PURPOSE: construct RIB tree from file
	*RETURN VALUES: number of items in rib file
	*/
unsigned int CFib::BuildFibFromFile(string sFileName, int dir) {
    int             ret = -1;
    unsigned int    iRuleID = 0;
	unsigned int	iEntryCount = 0;		//the number of items from file

    char            line[1024];         //rule
	unsigned long	lPrefix;			//the value of Prefix
	
	ifstream fin(sFileName.c_str());
    
    if (!fin) {
        cout << "Open file error!" << endl;
        return -1;
    }

    memset(line, 0, sizeof(line));

	while (fin.getline(line, sizeof(line))) {

        int src_ip1;
        int src_ip2;
        int src_ip3;
        int src_ip4;
        int src_ipm;

        int dst_ip1;
        int dst_ip2;
        int dst_ip3;
        int dst_ip4;
        int dst_ipm;

        int src_port1;
        int src_port2;

        int dst_port1;
        int dst_port2;

        int proto;
        int proto_m;
#if 0
        int prio;
        int prio_m;
#endif
        lPrefix = 0;

        //ret = sscanf(line, "@%d.%d.%d.%d/%d %d.%d.%d.%d/%d %d : %d %d : %d 0x%x/0x%x 0x%x/0x%x", &src_ip1, &src_ip2, &src_ip3, &src_ip4, &src_ipm, &dst_ip1, &dst_ip2, &dst_ip3, &dst_ip4, &dst_ipm, &src_port1, &src_port2, &dst_port1, &dst_port2, &proto, &proto_m, &prio, &prio_m);
        ret = sscanf_s(line, "@%d.%d.%d.%d/%d %d.%d.%d.%d/%d %d : %d %d : %d 0x%x/0x%x", &src_ip1, &src_ip2, &src_ip3, &src_ip4, &src_ipm, &dst_ip1, &dst_ip2, &dst_ip3, &dst_ip4, &dst_ipm, &src_port1, &src_port2, &dst_port1, &dst_port2, &proto, &proto_m);

        if (ret == 16) {

            if (dir == DIR_SRC) {
                lPrefix += (src_ip1 << 24);
                lPrefix += (src_ip2 << 16);
                lPrefix += (src_ip3 << 8);
                lPrefix += src_ip4;

                AddNode(lPrefix, src_ipm, iRuleID);
            } else {
                lPrefix += (dst_ip1 << 24);
                lPrefix += (dst_ip2 << 16);
                lPrefix += (dst_ip3 << 8);
                lPrefix += dst_ip4;

                AddNode(lPrefix, dst_ipm, iRuleID);
            }

            iRuleID++;
        }
        else {
            cout << "Error flow item!!!" << endl;
        }

        iEntryCount++;

        memset(line, 0, sizeof(line));
    }

    fin.close();

    return iEntryCount;
}


void CFib::LevelStatistic(FibTrie* pTrie, unsigned int level)
{
    if(NULL == pTrie)return;
    if(pTrie->RS->rules.size() != 0)num_level[level]++;

    LevelStatistic(pTrie->lchild, level+1);
    LevelStatistic(pTrie->rchild, level+1);
}

/*
 *	default_port代表最近祖先节点的NH
 *	
 *
void CFib::ytOptLevelPushing(FibTrie* pTrie, unsigned int level, struct ruleSet default_rule) {

    bool ifBlessLevelNode = false;	//标识节点是否处于16、24或32层

    if(NULL == pTrie) return;

    if (level == 0) {
        default_rule.minID = pTrie->oldRS->minID;
        default_rule.rules = pTrie->oldRS->rules; // To be confirmed
    }
    else if (level == 16 || level == 24 || level == 32) {

        ifBlessLevelNode = true;
        //pTrie->newRS->rules.insert(pTrie->newRS->rules.end(), default_rule.rules.begin(), default_rule.rules.end()); //default_rule.rules;
        pTrie->newRS->minID = default_rule.minID;
        pTrie->newRS->rules = default_rule.rules;

        pTrie->newRS->rules.insert(pTrie->newRS->rules.end(), pTrie->oldRS->rules.begin(), pTrie->oldRS->rules.end()); //default_rule.rules;

        if (default_rule.minID >  pTrie->oldRS->minID) {
            pTrie->newRS->minID = pTrie->oldRS->minID;
        }

        if (IsLeaf(pTrie)) {
            return;
        }
        else {
            default_rule.minID = (unsigned short)-1;
            default_rule.rules.clear(); //To be confirmed. By Qiaobin 2015/01/18
        }
    }
    else if (pTrie->oldRS->rules.size() > 0) { //modify by Qiaobin

        if (default_rule.minID > pTrie->oldRS->minID) {
            default_rule.minID = pTrie->oldRS->minID;
        }

        default_rule.rules.insert(default_rule.rules.end(), pTrie->oldRS->rules.begin(), pTrie->oldRS->rules.end());
    }

    //left child
    if (NULL == pTrie->lchild)
    {
        FibTrie* pTChild; // = new FibTrie();//= (struct FibTrie*)malloc(FIBLEN);
        CreateNewNode(pTChild);

        if (NULL==pTChild)
        {
            printf("malloc faild");
        }
        pTChild->lchild = NULL;
        pTChild->rchild = NULL;

        pTChild->oldRS->rules.clear();
        pTChild->newRS->rules.clear();

        pTChild->nodeLevel = pTrie->nodeLevel + 1;
        pTrie->lchild = pTChild;
    }
#if 0
    else if (0 == pTrie->lchild->newRS->rules.size()) {
        pTrie->lchild->newRS->rules.insert(pTrie->lchild->newRS->rules.end(), default_rule.rules.begin(), default_rule.rules.end());
    }
#endif

    //right child
    if (NULL==pTrie->rchild)
    {
        FibTrie* pTChild; //= new FibTrie();//(struct FibTrie*)malloc(FIBLEN);
        CreateNewNode(pTChild);

        if (NULL == pTChild)
        {
            printf("malloc faild");
        }
        pTChild->lchild = NULL;
        pTChild->rchild = NULL;

        pTChild->oldRS->rules.clear();
        pTChild->newRS->rules.clear();

        pTChild->nodeLevel = pTrie->nodeLevel + 1;
        pTrie->rchild = pTChild;
    }
#if 0
    else if (0 == pTrie->rchild->newRS->rules.size()) {
        pTrie->rchild->newRS->rules.insert(pTrie->rchild->newRS->rules.end(), default_rule.rules.begin(), default_rule.rules.end());
    }

    if (!ifBlessLevelNode) {	//如果是16、24或32层，则将新端口号保留
        pTrie->newRS->rules.clear();
    }
#endif

    ytOptLevelPushing(pTrie->lchild, level+1,default_rule);
    ytOptLevelPushing(pTrie->rchild, level+1,default_rule);
}*/

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
    if (pTrie == NULL)
        return false;

    int level = pTrie->nodeLevel;

    ruleCount += pTrie->RS->rules.size();

    if (IsLeaf(pTrie)) {	//如果是叶子节点，必须为16, 24, 32层
        if (level == 16 || level == 24 || level == 32) {
            return true;
        }
        else {
            return false;
            printf("leaf in other levels!\n");
        }
    }
    else {	//非叶子节点

        if (pTrie->RS->rules.size() != 0 && (level != 0 && level != 16 && level != 24 && level != 32)) {
            printf("Error newRS!\n");
            return false;
        }
        if (!pTrie->rchild || !pTrie->lchild) {	// 某一个子节点为空
            printf("<2 children!\n");
            return false;
        }
        else if (isCorrectTrie(pTrie->rchild) && isCorrectTrie(pTrie->lchild)) {	// 子节点均不为空，判断左右子树的正确性
            return true;
        }
        else
            return false;
    }
}

unsigned int CFib::GetAncestorHop(FibTrie* pTrie)
{
#if 0
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
#endif

    return -1;
}

void CFib::buildBF(struct ruleSet* rs, std_bf *rBF) {
    if (!rs) {
        cout << "Invalid rule set" << endl;
        return;
    }

    if (!rBF) {
        cout << "NULL BFs" << endl;
        return;
    }

    uint i = 0;
 //   char key[32];

    for (i = 0; i < rs->rules.size(); i++) {
		rBF->insert(rs->rules[i]);
    }
}

void CFib::buildLookupTable(FibTrie* pTrie)
{
	if (pTrie == NULL)
		return;

	int level = pTrie->nodeLevel;

	sort(pTrie->RS->rules.begin(), pTrie->RS->rules.end());

	if (level == 16) 
	{
		//buildBF(pTrie->newRS, levelTable16_BF[level16_node_num]);
		if (IsLeaf(pTrie)) {
			levelTable16_NH[level16_node_num].minID = pTrie->RS->minID;
			levelTable16_NH[level16_node_num].rules = &pTrie->RS->rules;

			buildBF(pTrie->RS, levelTable16_BF[level16_node_num]);
		}
		else {
			//levelTable16_NH[level16_node_num].minID = pTrie->newRS->minID;
			//levelTable16_NH[level16_node_num].rules = pTrie->newRS->rules;

			levelTable16_offset[level16_node_num] = level16_nonleaf_num;
			level16_nonleaf_num++;
		}

		if (level16_nonleaf_num > Chunk24_num) {
			cout << "Too many offsets in level 24!!!!" << endl;
			return;
		}

		level16_node_num++;
	}
	else if (level == 24) 
	{
		if (IsLeaf(pTrie)) {

			levelTable24_NH[level24_node_num].minID = pTrie->RS->minID;
			levelTable24_NH[level24_node_num].rules = &pTrie->RS->rules;
			buildBF(pTrie->RS, levelTable24_BF[level24_node_num]);
		}
		else {
			//levelTable24_NH[level24_node_num].minID = pTrie->newRS->minID;
			//levelTable24_NH[level24_node_num].rules = pTrie->newRS->rules;
			levelTable24_offset[level24_node_num] = level24_nonleaf_num;
			level24_nonleaf_num++;
		}

		if (level24_nonleaf_num > Chunk32_num) {
			cout << "Too many offsets in level 32!!!!" << endl;
			return;
		}

		level24_node_num++;
	}
	else if (level == 32) 
	{
		levelTable32_NH[level32_node_num].minID = pTrie->RS->minID;
		levelTable32_NH[level32_node_num].rules = &pTrie->RS->rules;
		buildBF(pTrie->RS, levelTable32_BF[level32_node_num]);
		level32_node_num++;
	}

	if (pTrie->lchild) {
		buildLookupTable(pTrie->lchild);
	}

	if (pTrie->rchild) {
		buildLookupTable(pTrie->rchild);
	}
}


/*
void CFib::buildLookupTable(FibTrie* pTrie)
{
    if (pTrie == NULL)
        return;

    int level = pTrie->nodeLevel;

    if (level == 16) {

        buildBF(pTrie->newRS, levelTable16_BF[level16_node_num]);

        if (IsLeaf(pTrie)) {
            levelTable16_NH[level16_node_num].minID = pTrie->newRS->minID;
            levelTable16_NH[level16_node_num].rules = pTrie->newRS->rules;
        }
        else {
            levelTable16_NH[level16_node_num].minID = pTrie->newRS->minID;
            levelTable16_NH[level16_node_num].rules = pTrie->newRS->rules;
            levelTable16_offset[level16_node_num] = level16_nonleaf_num;
            level16_nonleaf_num++;
        }

        level16_node_num++;
    }
    else if (level == 24) {

        buildBF(pTrie->newRS, levelTable24_BF[level24_node_num]);

        if (IsLeaf(pTrie)) {
            levelTable24_NH[level24_node_num].minID = pTrie->newRS->minID;
            levelTable24_NH[level24_node_num].rules = pTrie->newRS->rules;
        }
        else {
            levelTable24_NH[level24_node_num].minID = pTrie->newRS->minID;
            levelTable24_NH[level24_node_num].rules = pTrie->newRS->rules;
            levelTable24_offset[level24_node_num] = level24_nonleaf_num;
            level24_nonleaf_num++;
        }

        level24_node_num++;
    }
    else if (level == 32) {

        buildBF(pTrie->newRS, levelTable32_BF[level32_node_num]);

        levelTable32_NH[level32_node_num].minID = pTrie->newRS->minID;
        levelTable32_NH[level32_node_num].rules = pTrie->newRS->rules;
        level32_node_num++;
    }

    if (pTrie->lchild) {
        buildLookupTable(pTrie->lchild);
    }

    if (pTrie->rchild) {
        buildLookupTable(pTrie->rchild);
    }
}
*/
struct ruleSet CFib::optTrieLookup(char * insert_C) {

    struct ruleSet rs; //= (struct ruleSet*)calloc(1, sizeof(*rs));
    rs.minID = (unsigned short)-1;
    rs.rules.clear();

    FibTrie *insertNode = m_pTrie;

    if (insertNode->RS->rules.size() != 0) {
        rs.minID = insertNode->RS->minID;
        rs.rules.insert(rs.rules.end(), insertNode->RS->rules.begin(), insertNode->RS->rules.end());
    }

    int len=(int) strlen(insert_C);

    for (int i=0; i < (len + 1);i++)
    {		
        if ('0' == insert_C[i])
        {//if 0, turn left
            if (NULL != insertNode->lchild)	
            {
                insertNode = insertNode->lchild;

                if (insertNode->RS->rules.size() != 0) {

                    if (rs.minID > insertNode->RS->minID) {
                        rs.minID = insertNode->RS->minID;
                    }

                    rs.rules.insert(rs.rules.end(), insertNode->RS->rules.begin(), insertNode->RS->rules.end());
                }

            }
            else {
                break;
            }
        }
        else
        {//if 1, turn right
            if (NULL != insertNode->rchild) {
                insertNode = insertNode->rchild;

                if (insertNode->RS->rules.size() != 0) {

                    if (rs.minID > insertNode->RS->minID) {
                        rs.minID = insertNode->RS->minID;
                    }

                    rs.rules.insert(rs.rules.end(), insertNode->RS->rules.begin(), insertNode->RS->rules.end());
                }

            }
            else {
                break;
            }
        }

    }

    return	rs;
}

struct ruleSet CFib::blessLookup(unsigned int ip) {
	/*
	struct pRuleSet* tmp = NULL;
    struct ruleSet rs; //= (struct ruleSet*)calloc(1, sizeof(*rs));
    rs.minID = (unsigned short)-1;
    rs.rules.clear();

	unsigned int offset16 = ip >> 16;
    unsigned int level = 0;

    rs.minID = levelTable16_NH[offset16].minID;
    rs.rules = levelTable16_NH[offset16].rules;

    if (levelTable16_offset[offset16] == 0) {
        return rs;
    }

    unsigned int offset24 = ((levelTable16_offset[offset16] - 1) << 8) + (ip<<16>>24);
    tmp = &levelTable24_NH[offset24];

    if (rs.minID > tmp->minID) {
        rs.minID = tmp->minID;
    }

    rs.rules.insert(rs.rules.end(), tmp->rules.begin(), tmp->rules.end());

    if ((levelTable24_offset[offset24]) == 0) {
        return rs;
    }

    register unsigned int offset32 = ((levelTable24_offset[((levelTable16_offset[offset16] -1) << 8) + (ip<<16>>24)] - 1) << 8) + (ip & 255);

    tmp = &levelTable32_NH[offset32];

    if (rs.minID > tmp->minID) {
        rs.minID = tmp->minID;
    }

    rs.rules.insert(rs.rules.end(), tmp->rules.begin(), tmp->rules.end());

    return rs;
	*/
}

void CFib::BFLookup(unsigned int ip, struct lookupRes &lrs) {
 //   struct ruleSet* tmp = NULL;
 //
 //   std_bf *bf16 = NULL;
 //   std_bf *bf24 = NULL;
 //   std_bf *bf32 = NULL;

 //   lrs.minID = (unsigned short)-1;
 //   lrs.rules.clear();

 //   register unsigned short offset16 = ip >> 16;
 //   register unsigned short level = 0;

 //   lrs.minID = levelTable16_NH[offset16].minID;
 //   lrs.rules = levelTable16_NH[offset16].rules;

	//bf16 = levelTable16_BF[offset16];
 //   if (levelTable16_offset[offset16] == 0) {
	//	SIMD_MOV(BF_ELE)(bf16->bf_base, lrs.rBF->bf_base);
	//	return;
 //   }

 //   register unsigned int offset24 = ((levelTable16_offset[offset16] - 1) << 8) + (ip<<16>>24);
 //   tmp = &levelTable24_NH[offset24];

 //   if (lrs.minID > tmp->minID) {
 //       lrs.minID = tmp->minID;
 //   }

 //   lrs.rules.insert(lrs.rules.end(), tmp->rules.begin(), tmp->rules.end());

	//bf24 = levelTable24_BF[offset24];
 //   if ((levelTable24_offset[offset24]) == 0) {
	//	SIMD_OR(BF_ELE)(bf16->bf_base, bf24->bf_base, lrs.rBF->bf_base);
 //       return ;
 //   }

 //   register unsigned int offset32 = ((levelTable24_offset[((levelTable16_offset[offset16] -1) << 8) + (ip<<16>>24)] - 1) << 8) + (ip & 255);

 //   tmp = &levelTable32_NH[offset32];

 //   if (lrs.minID > tmp->minID) {
 //       lrs.minID = tmp->minID;
 //   }

 //   lrs.rules.insert(lrs.rules.end(), tmp->rules.begin(), tmp->rules.end());
 //   
	//SIMD_OR(BF_ELE)(bf16->bf_base, bf24->bf_base, levelTable32_BF[offset32]->bf_base, lrs.rBF->bf_base);
}

void CFib::BFLookup_noinsert(unsigned int ip, struct lookupRes &lrs) {
	struct pRuleSet* tmp = NULL;

	std_bf *bf16 = NULL;
	std_bf *bf24 = NULL;
	std_bf *bf32 = NULL;

	lrs.minID = (unsigned short)-1;
	//lrs.rules.clear();

	register unsigned short offset16 = ip >> 16;
	register unsigned short level = 0;

	lrs.minID = levelTable16_NH[offset16].minID;
	//lrs.rules = levelTable16_NH[offset16].rules;

	bf16 = levelTable16_BF[offset16];
	if (levelTable16_offset[offset16] == 0) {
		SIMD_MOV(BF_ELE)(bf16->bf_base, lrs.rBF->bf_base);
		return;
	}

	register unsigned int offset24 = ((levelTable16_offset[offset16] - 1) << 8) + (ip << 16 >> 24);
	tmp = &levelTable24_NH[offset24];

	if (lrs.minID > tmp->minID) {
		lrs.minID = tmp->minID;
	}

	//lrs.rules.insert(lrs.rules.end(), tmp->rules.begin(), tmp->rules.end());

	bf24 = levelTable24_BF[offset24];
	if ((levelTable24_offset[offset24]) == 0) {
		SIMD_OR(BF_ELE)(bf16->bf_base, bf24->bf_base, lrs.rBF->bf_base);
		return;
	}

	register unsigned int offset32 = ((levelTable24_offset[((levelTable16_offset[offset16] - 1) << 8) + (ip << 16 >> 24)] - 1) << 8) + (ip & 255);

	tmp = &levelTable32_NH[offset32];

	if (lrs.minID > tmp->minID) {
		lrs.minID = tmp->minID;
	}

	//lrs.rules.insert(lrs.rules.end(), tmp->rules.begin(), tmp->rules.end());

	SIMD_OR(BF_ELE)(bf16->bf_base, bf24->bf_base, levelTable32_BF[offset32]->bf_base, lrs.rBF->bf_base);
}

void CFib::basicBFLookup(unsigned int ip, struct lookupRes &lrs) {
    register unsigned int offset16 = ip >> 16;
    register unsigned int level = 0;
	//lrs.rules.clear();

    if (levelTable16_offset[offset16] == 0) {
        lrs.minID = levelTable16_NH[offset16].minID;
		lrs.rules = levelTable16_NH[offset16].rules;
		memcpy(lrs.rBF->bf_base, levelTable16_BF[offset16]->bf_base, 512/8);
        //SIMD_MOV(BF_ELE)(levelTable16_BF[offset16]->bf_base, lrs.rBF->bf_base);
        return;
    }

    register unsigned int offset24 = ((levelTable16_offset[offset16] - 1) << 8) + (ip<<16>>24);
    
    if ((levelTable24_offset[offset24]) == 0) {
        lrs.minID = levelTable24_NH[offset24].minID;
		lrs.rules = levelTable24_NH[offset24].rules;
		memcpy(lrs.rBF->bf_base, levelTable24_BF[offset24]->bf_base, 512/8);
        //SIMD_MOV(BF_ELE)(levelTable24_BF[offset24]->bf_base, lrs.rBF->bf_base);
        return ;
    }

    register unsigned int offset32 = ((levelTable24_offset[offset24] - 1) << 8) + (ip & 255);
    lrs.minID = levelTable32_NH[offset32].minID;
	lrs.rules = levelTable32_NH[offset32].rules;
	memcpy(lrs.rBF->bf_base, levelTable32_BF[offset32]->bf_base, 512/8);
    //SIMD_MOV(BF_ELE)(levelTable32_BF[offset32]->bf_base, lrs.rBF->bf_base);
}


void CFib::basicBFLookup_noinsert(unsigned int ip, struct lookupRes &lrs) {
	register unsigned int offset16 = ip >> 16;
	register unsigned int level = 0;

	if (levelTable16_offset[offset16] == 0) {
		lrs.minID = levelTable16_NH[offset16].minID;
		lrs.rBF->bf_base=levelTable16_BF[offset16]->bf_base;
		//memcpy(lrs.rBF->bf_base, levelTable16_BF[offset16]->bf_base, 512/8);
		//lrs.rules = &levelTable16_NH[offset16].rules;
		//SIMD_MOV(BF_ELE)(levelTable16_BF[offset16]->bf_base, lrs.rBF->bf_base);
		return;
	}

	register unsigned int offset24 = ((levelTable16_offset[offset16] - 1) << 8) + (ip<<16>>24);

	if ((levelTable24_offset[offset24]) == 0) {
		lrs.minID = levelTable24_NH[offset24].minID;
		lrs.rBF->bf_base=levelTable24_BF[offset24]->bf_base;
		//memcpy(lrs.rBF->bf_base, levelTable24_BF[offset24]->bf_base, 512/8);
		//lrs.rules = &levelTable24_NH[offset24].rules;
		//SIMD_MOV(BF_ELE)(levelTable24_BF[offset24]->bf_base, lrs.rBF->bf_base);
		return ;
	}

	register unsigned int offset32 = ((levelTable24_offset[offset24] - 1) << 8) + (ip & 255);
	lrs.minID = levelTable32_NH[offset32].minID;
	lrs.rBF->bf_base=levelTable32_BF[offset32]->bf_base;

	//memcpy(lrs.rBF->bf_base, levelTable32_BF[offset32]->bf_base, 512/8);
	//lrs.rules = &levelTable32_NH[offset32].rules;
	//SIMD_MOV(BF_ELE)(levelTable32_BF[offset32]->bf_base, lrs.rBF->bf_base);
}

//void CFib::basicBFLookup_noinsert(unsigned int ip, struct lookupRes &lrs) {
//    struct ruleSet* tmp = NULL;
//
//    lrs.minID = (unsigned short)-1;
//
//    register unsigned short offset16 = ip >> 16;
//    register unsigned short level = 0;
//
//    lrs.minID = levelTable16_NH[offset16].minID;
//
//    if (levelTable16_offset[offset16] == 0) {
//        SIMD_MOV(BF_ELE)(levelTable16_BF[offset16]->bf_base, lrs.rBF->bf_base);
//        return;
//    }
//
//    register unsigned int offset24 = ((levelTable16_offset[offset16] - 1) << 8) + (ip << 16 >> 24);
//    tmp = &levelTable24_NH[offset24];
//
//    if (lrs.minID > tmp->minID) {
//        lrs.minID = tmp->minID;
//    }
//
//    if ((levelTable24_offset[offset24]) == 0) {
//        SIMD_MOV(BF_ELE)(levelTable24_BF[offset24]->bf_base, lrs.rBF->bf_base);
//        return;
//    }
//
//    register unsigned int offset32 = ((levelTable24_offset[offset24] - 1) << 8) + (ip & 255);
//    tmp = &levelTable32_NH[offset32];
//
//    if (lrs.minID > tmp->minID) {
//        lrs.minID = tmp->minID;
//    }
//
//    SIMD_MOV(BF_ELE)(levelTable32_BF[offset32]->bf_base, lrs.rBF->bf_base);
//}

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
#if 0
    printf("\n\nBless algorithm starts...\n\n");

    BuildFibFromFile(fib_fil);
    ytOptLevelPushing(m_pTrie, 0, 0);
    buildLookupTable(m_pTrie);
    TrafficRead(traffi_fil);

#if 0
    LARGE_INTEGER frequence,p0,p1;
    if(!QueryPerformanceFrequency(&frequence))return;
    QueryPerformanceCounter(&p0);
#endif

    struct timeval start, end;

    gettimeofday(&start, NULL);

    register short tmp=0;
    register unsigned char LMPort = PORT_MAX;
    for (register int j=0;j<10000;j++)
    {
        for (register int i=0;i<TRACE_READ;i++)
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

#if 0
    QueryPerformanceCounter(&p1);
    long long Lookuptime=1000000*(p1.QuadPart-p0.QuadPart)/frequence.QuadPart;
#endif

    gettimeofday(&end, NULL);
    long long Lookuptime= (end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec);
    printf("\tLMPport=%d\n\tLookup time=%lld us\n\tThroughput is:\t %.3f Mpps\n", LMPort, Lookuptime, 10000.0*TRACE_READ/Lookuptime);
#endif
}

void CFib::TestLookupSpeed_old(char *traffi_fil, char* fib_fil)
{
#if 0
    printf("\n\nBless algorithm starts...\n\n");

    unsigned int iEntryCount = BuildFibFromFile(fib_fil);
    ytOptLevelPushing(m_pTrie, 0, 0);
    buildLookupTable(m_pTrie);


    //unsigned int *traffic=TrafficRead(traffic_file);
    TrafficRead(traffi_fil);

    register unsigned short res;
    LARGE_INTEGER frequence,privious,privious1;
    //printf("\tfrequency=%u\n",frequence.QuadPart);//2825683

    register unsigned short level = 0;
    register unsigned short tmp;
    register unsigned short LMPort[4] = {PORT_MAX<<1, 0, 0, 0};

    if(!QueryPerformanceFrequency(&frequence))return;
    QueryPerformanceCounter(&privious);

    for (register int j=0;j<10000;j++)
    {
        for (register int i=0;i<TRACE_READ;i++)
        {
            if ((tmp = levelTable16_NH[trafficIn[i]>>16]) >> 1) 
            {
                res=tmp>>1;
            }

            if (!(tmp & 1)) 
            {
                if ((tmp = levelTable24_NH[((levelTable16_offset[trafficIn[i]>>16]) << 8) + (trafficIn[i]<<16>>24)]) >> 1) 
                {
                    res=tmp>>1;
                }

                if (!(tmp & 1)) 
                {
                    if ((tmp = levelTable32_NH[((levelTable24_offset[((levelTable16_offset[trafficIn[i]>>16]) << 8) + (trafficIn[i]<<16>>24)]) << 8) + (trafficIn[i] & 255)])) 
                    {
                        res=tmp>>1;
                    }
                }
            }
            //port=LMPort[level]>>1;
        }
    }

    QueryPerformanceCounter(&privious1);
    long long Lookuptime=1000000*(privious1.QuadPart-privious.QuadPart)/frequence.QuadPart;
    printf("\tLMPport=%d\n\tLookup time=%u\n\tThroughput is:\t %.3f Mpps\n", res, Lookuptime, 10000.0*TRACE_READ/Lookuptime);
#endif
}
