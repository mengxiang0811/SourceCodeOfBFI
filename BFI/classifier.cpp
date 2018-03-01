#include "classifier.h"
#include "DataProcessing.h"
#include <algorithm>
#include <iostream>
using namespace std;

void trie_init(struct packet_classifier *pc, string flowFile) {
	struct pRuleSet rs;
	vector<unsigned short> vv;
	rs.rules = &vv;

	unsigned int iEntryCount = 0;
	
    pc->srcFib = new CFib();
    iEntryCount = pc->srcFib->BuildFibFromFile(flowFile, DIR_SRC);

    pc->srcFib->ytGetNodeCounts();
   // printf("\nThe total number of routing items in srcFib file is: %u\nThe total number of solid Trie node is :%u\nsrcFib.allNodeCount=%d\n",iEntryCount, pc->srcFib->solidNodeCount, pc->srcFib->allNodeCount);

//	cout << "src chunk24_num: " << pc->srcFib->Chunk24_num << endl;
	//cout << "src chunk32_num: " << pc->srcFib->Chunk32_num << endl;

    //level-pushing

   // pc->srcFib->ytOptLevelPushing(pc->srcFib->m_pTrie, 0, rs);
	pc->srcFib->createChunkBF();

	pc->srcFib->ytLevelPushing(pc->srcFib->m_pTrie, 0, rs);

	
    //optFib.OutputTrie(optFib.m_pTrie, newPortfile, oldPortfile, true);
    pc->srcFib->ytGetNodeCounts();
  //  printf("\nAfter ytLevelPushing...\nThe total number of solid Trie node is :%u\nsrcFib.allNodeCount=%d\n", pc->srcFib->solidNodeCount, pc->srcFib->allNodeCount);



	//printf("\nTo test the correctness of the trie structure!!!\n");
#if 0
    if (!pc->srcFib->isCorrectTrie(pc->srcFib->m_pTrie)) {
        printf("The srcFib trie structure is incorrect!!!\n");
    }
    else {
        printf("The srcFib trie structure is correct!\n");
    }

    printf("There are %d rules\n", pc->srcFib->ruleCount);

    printf("\n\n********************Building Lookup Table************************\n");
#endif

    pc->srcFib->buildLookupTable(pc->srcFib->m_pTrie);
#if 0
    printf("*****************************End*********************************\n");
#endif

    
	

    /************************************************************/
    pc->dstFib = new CFib();
    iEntryCount = pc->dstFib->BuildFibFromFile(flowFile, DIR_DST);

    pc->dstFib->ytGetNodeCounts();

 //   printf("\nThe total number of routing items in dstFib file is %u\nThe total number of solid Trie node is %u,\ndstFib.allNodeCount=%d\n",iEntryCount, pc->dstFib->solidNodeCount, pc->dstFib->allNodeCount);

	pc->dstFib->createChunkBF();

    //level-pushing

  //  pc->dstFib->ytOptLevelPushing(pc->dstFib->m_pTrie, 0, rs);
	
	pc->dstFib->ytLevelPushing(pc->dstFib->m_pTrie, 0, rs);

    //optFib.OutputTrie(optFib.m_pTrie, newPortfile, oldPortfile, true);
    pc->dstFib->ytGetNodeCounts();
 //   printf("\nAfter ytLevelPushing...\nThe total number of solid Trie node is %u\ndstFib.allNodeCount=%d\n", pc->dstFib->solidNodeCount, pc->dstFib->allNodeCount);

  //  printf("\nTo test the correctness of the trie structure!!!\n");
#if 0
    if (!pc->dstFib->isCorrectTrie(pc->dstFib->m_pTrie)) {
        printf("The dstFib trie structure is incorrect!!!\n");
    }
    else {
        printf("The dstFib trie structure is correct!\n");
    }
	
	
    printf("There are %d rules\n", pc->dstFib->ruleCount);

    printf("\n\n************************Building Lookup Table************************\n");
#endif
    pc->dstFib->buildLookupTable(pc->dstFib->m_pTrie);
#if 0
	printf("*****************************End*********************************\n");
#endif
}

void port_init(struct packet_classifier *pc, string flowFile) {
    int i = 0;

    for (i = 0; i < 65536; i++) {
		pc->srcPort[i] = new struct ppi;
        pc->srcPort[i]->rbf = new std_bf(BF_ELE, BF_HF);
		pc->srcPort[i]->minID = (unsigned short)-1;
		pc->dstPort[i] = new struct ppi;
        pc->dstPort[i]->rbf = new std_bf(BF_ELE, BF_HF);
		pc->dstPort[i]->minID = (unsigned short)-1;
    }

    for (i = 0; i < 256; i++) {
		pc->protocol[i] = new struct ppi;
        pc->protocol[i]->rbf = new std_bf(BF_ELE, BF_HF);
		pc->protocol[i]->minID = (unsigned short)-1;
    }
    
    int             ret = -1;
    unsigned int    iRuleID = 0;
	unsigned int	iEntryCount = 0;		//the number of items from file

    char            line[1024];         //rule
	
	ifstream fin(flowFile.c_str());
    
    if (!fin) {
        cout << "Open flow file error!" << endl;
        return;
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

        //ret = sscanf(line, "@%d.%d.%d.%d/%d %d.%d.%d.%d/%d %d : %d %d : %d 0x%x/0x%x 0x%x/0x%x", &src_ip1, &src_ip2, &src_ip3, &src_ip4, &src_ipm, &dst_ip1, &dst_ip2, &dst_ip3, &dst_ip4, &dst_ipm, &src_port1, &src_port2, &dst_port1, &dst_port2, &proto, &proto_m, &prio, &prio_m);
        ret = sscanf_s(line, "@%d.%d.%d.%d/%d %d.%d.%d.%d/%d %d : %d %d : %d 0x%x/0x%x", &src_ip1, &src_ip2, &src_ip3, &src_ip4, &src_ipm, &dst_ip1, &dst_ip2, &dst_ip3, &dst_ip4, &dst_ipm, &src_port1, &src_port2, &dst_port1, &dst_port2, &proto, &proto_m);

        if (ret == 16) {

         //   char key[32];
          //  memset(key, 0, sizeof(key));
       //     sprintf_s(key, "%u", iRuleID);
#if 0
            if (iRuleID == 81) {
                cout << "The 81th rule" << endl;
            }
#endif       
            for (i = src_port1; i <= src_port2; i++) {
       //         pc->srcPort[i]->insert((const uchar*)key, strlen(key));
				pc->srcPort[i]->rbf->insert(iRuleID);
				if (iRuleID < pc->srcPort[i]->minID)
					pc->srcPort[i]->minID = iRuleID;
            }

            for (i = dst_port1; i <= dst_port2; i++) {
               // pc->dstPort[i]->insert((const uchar*)key, strlen(key));
				pc->dstPort[i]->rbf->insert(iRuleID);
				if (iRuleID < pc->dstPort[i]->minID)
					pc->dstPort[i]->minID = iRuleID;
            }

            if (proto_m == 0xFF) {
                //pc->protocol[proto]->insert((const uchar *)key, strlen(key));
				pc->protocol[proto]->rbf->insert(iRuleID);
				if (iRuleID < pc->protocol[proto]->minID)
					pc->protocol[proto]->minID = iRuleID;

            } else {
                for (i = 0; i <= 255; i++) {
                 //   pc->protocol[i]->insert((const uchar*)key, strlen(key));
					pc->protocol[i]->rbf->insert(iRuleID);
					if (iRuleID < pc->protocol[i]->minID)
					pc->protocol[i]->minID = iRuleID;
                }
            }

            iRuleID++;
        }

        iEntryCount++;

        memset(line, 0, sizeof(line));
    }

    fin.close();
}


//Global variable for BFlookup
struct lookupRes srcRS;
struct lookupRes dstRS;

void classifier_init(struct packet_classifier *pc, string flowFile) {
    trie_init(pc, flowFile);
    port_init(pc, flowFile);
	srcRS.rBF = new std_bf(BF_ELE, BF_HF);
	dstRS.rBF = new std_bf(BF_ELE, BF_HF);
}

void performance(struct packet_classifier *pc, struct pc_rules *pcr, char* traceFile) {
    unsigned int    lRID;


	LARGE_INTEGER frequence, p0, p1;
	if (!QueryPerformanceFrequency(&frequence))return;

	struct flow *flows = read_trace_file(traceFile);

#if 0
#define CYCLEs 10
#endif
	QueryPerformanceCounter(&p0);
#if 0
for (int jjj=0;jjj<CYCLEs;jjj++)
{
#endif

    for (int i = 0; i < trace_rule_num; i++) {
		lRID = flowLookup_dst(pc, pcr, &flows[i]);
#if 0
        if (lRID != (flows[i].trueRID-1))
           cout << "Error" << endl;
#endif
    }
#if 0
}
#endif
	QueryPerformanceCounter(&p1);

	double time = (double)(p1.QuadPart - p0.QuadPart) / (double)frequence.QuadPart;

   // cout << "There are " << trace_rule_num << " trace records!" << endl;
	//cout << "Total time: " << time << endl;
#if 0
	cout << CYCLEs*(double)trace_rule_num / time / 1000000.0 << endl;
#endif
	cout <<"BFI"<<"\t"<<(double)trace_rule_num / time / 1000000.0 << endl;
	//cout << "Second err: " << sec_err << endl;
}


//unsigned int flowLookup(struct packet_classifier *pc, struct pc_rules *pcr, struct flow *f) {
//
//	//pc->srcFib->BFLookup(f->src_ip, srcRS);
//	//pc->dstFib->BFLookup_noinsert(f->dst_ip, dstRS);
//
//	pc->srcFib->basicBFLookup(f->src_ip, srcRS);
//	//pc->dstFib->basicBFLookup(f->dst_ip, dstRS);
//#if 0
//	pc->dstFib->basicBFLookup_noinsert(f->dst_ip, dstRS);
//	
//	//if (dstRS.minID == (unsigned short)-1) {
//	//	return (unsigned short)-1;
//	//}
//	
//	/*
//	if (srcRS.rules.size() == 0) {
//		return (unsigned short)-1;
//	}
//*/
//	//if (srcRS.rules.size() == 0) {
//	//	return (unsigned short)-1;
//	//}
//	/*
//	if (dstRS.rules.size() == 0) {
//		return (unsigned short)-1;
//	}
//	*/
//#if 1
//	unsigned int maxid = dstRS.minID;
//	if (srcRS.minID > maxid) maxid = srcRS.minID;
//	if (pc->srcPort[f->src_port]->minID > maxid) maxid = pc->srcPort[f->src_port]->minID;
//	if (pc->dstPort[f->dst_port]->minID > maxid) maxid = pc->dstPort[f->dst_port]->minID;
//	if (pc->protocol[f->proto]->minID > maxid) maxid = pc->protocol[f->proto]->minID;
//#endif
//
//	SIMD_AND(BF_ELE)(srcRS.rBF->bf_base, dstRS.rBF->bf_base, pc->srcPort[f->src_port]->rbf->bf_base, pc->dstPort[f->dst_port]->rbf->bf_base, pc->protocol[f->proto]->rbf->bf_base);
//
//	//sort(srcRS.rules.begin(), srcRS.rules.end());
//
//	for (unsigned int i = 0; i < srcRS.rules.size(); i++) {
//#if 1
//		if (srcRS.rules[i] < maxid) continue;
//#endif
//		if (srcRS.rBF->query(srcRS.rules[i])) {
//			if (second_confirmation(pcr, srcRS.rules[i], f)) {
//				return srcRS.rules[i];
//			}
//			else {
//				sec_err++;
//			}
//		}
//	}
//#endif
//	return ((unsigned short)-1);
//}


//choose version--good

/*
unsigned int flowLookup_choose(struct packet_classifier *pc, struct pc_rules *pcr, struct flow *f)
{
	pc->dstFib->BFLookup(f->dst_ip, dstRS);
	pc->srcFib->BFLookup(f->src_ip, srcRS);
	if (srcRS.rules->size()<dstRS.rules->size())
	{
#if 1
		//if (srcRS.minID == (unsigned short)-1) {
		//	return (unsigned short)-1;
		//}

		//if (srcRS.rules.size() == 0) {
		//	return (unsigned short)-1;
		//}

		unsigned int maxid = srcRS.minID;
		if (pc->srcPort[f->src_port]->minID > maxid) maxid = pc->srcPort[f->src_port]->minID;
		if (pc->dstPort[f->dst_port]->minID > maxid) maxid = pc->dstPort[f->dst_port]->minID;
		if (pc->protocol[f->proto]->minID > maxid) maxid = pc->protocol[f->proto]->minID;

		//为什么屏蔽这一行，结果还是对的？
		SIMD_AND(BF_ELE)(srcRS.rBF->bf_base, dstRS.rBF->bf_base, pc->srcPort[f->src_port]->rbf->bf_base, pc->dstPort[f->dst_port]->rbf->bf_base, pc->protocol[f->proto]->rbf->bf_base);
		sort(srcRS.rules->begin(), srcRS.rules->end());

		for (unsigned int i = 0; i < srcRS.rules->size(); i++) {
#if 1
			if (srcRS.rules[i] < maxid)
			{
				//printf(".");
				continue;
			}
#endif
			if (srcRS.rBF->query(srcRS.rules[i]))
			{
				if (second_confirmation(pcr, srcRS.rules[i], f))
				{
					return srcRS.rules[i];
				}
				else
				{
					sec_err++;
				}
			}
		}
	}
	else
	{
		unsigned int maxid = dstRS.minID;
		if (pc->srcPort[f->src_port]->minID > maxid) maxid = pc->srcPort[f->src_port]->minID;
		if (pc->dstPort[f->dst_port]->minID > maxid) maxid = pc->dstPort[f->dst_port]->minID;
		if (pc->protocol[f->proto]->minID > maxid) maxid = pc->protocol[f->proto]->minID;


		//if (dstRS.minID == (unsigned short)-1) {
		//	return (unsigned short)-1;
		//}

		if (dstRS.rules.size() == 0) {
			return (unsigned short)-1;
		}

		SIMD_AND(BF_ELE)(dstRS.rBF->bf_base, srcRS.rBF->bf_base, pc->srcPort[f->src_port]->rbf->bf_base, pc->dstPort[f->dst_port]->rbf->bf_base, pc->protocol[f->proto]->rbf->bf_base);
		sort(dstRS.rules.begin(), dstRS.rules.end());

		for (unsigned int i = 0; i < dstRS.rules.size(); i++) {
#if 1
			if (dstRS.rules[i] < maxid)
			{
				//printf(".");
				continue;
			}
#endif
			if (dstRS.rBF->query(dstRS.rules[i]))
			{
				if (second_confirmation(pcr, dstRS.rules[i], f))
				{
					return dstRS.rules[i];
				}
				else
				{
					sec_err++;
				}
			}
		}
	}

#endif
	return ((unsigned short)-1);
}
*/
/*
//use dst address
u*nsigned int flowLookup_dst(struct packet_classifier *pc, struct pc_rules *pcr, struct flow *f)
{
	pc->srcFib->BFLookup_noinsert(f->src_ip, srcRS);
	pc->dstFib->BFLookup(f->dst_ip, dstRS);

	//if (dstRS.minID == (unsigned short)-1) {
	//	return (unsigned short)-1;
	//}
	//if (dstRS.rules.size() == 0) {
	//	return (unsigned short)-1;
	//}

	unsigned int maxid = dstRS.minID;
	//if (srcRS.minID > maxid) maxid = srcRS.minID;
	if (pc->srcPort[f->src_port]->minID > maxid) maxid = pc->srcPort[f->src_port]->minID;
	if (pc->dstPort[f->dst_port]->minID > maxid) maxid = pc->dstPort[f->dst_port]->minID;
	if (pc->protocol[f->proto]->minID > maxid) maxid = pc->protocol[f->proto]->minID;

	SIMD_AND(BF_ELE)(dstRS.rBF->bf_base, srcRS.rBF->bf_base, pc->srcPort[f->src_port]->rbf->bf_base, pc->dstPort[f->dst_port]->rbf->bf_base, pc->protocol[f->proto]->rbf->bf_base);
	sort(dstRS.rules.begin(), dstRS.rules.end());

	for (unsigned int i = 0; i < dstRS.rules.size(); i++) {
#if 1
		if (dstRS.rules[i] < maxid)
		{
			//printf(".");
			continue;
		}
#endif
		if (dstRS.rBF->query(dstRS.rules[i]))
		{
			if (second_confirmation(pcr, dstRS.rules[i], f))
			{
				return dstRS.rules[i];
			}
			else
			{
				sec_err++;
			}
		}
	}
	return (unsigned short)-1;
}*/

//use dst address
unsigned int flowLookup_dst(struct packet_classifier *pc, struct pc_rules *pcr, struct flow *f)
{
	pc->srcFib->basicBFLookup_noinsert(f->src_ip, srcRS);

#if 1
	pc->dstFib->basicBFLookup(f->dst_ip, dstRS);

	unsigned int maxid = dstRS.minID;
	if (srcRS.minID > maxid) maxid = srcRS.minID;
	cout << f->src_port << endl;
	if (pc->srcPort[f->src_port]->minID > maxid) maxid = pc->srcPort[f->src_port]->minID;
	if (pc->dstPort[f->dst_port]->minID > maxid) maxid = pc->dstPort[f->dst_port]->minID;
	if (pc->protocol[f->proto]->minID > maxid) maxid = pc->protocol[f->proto]->minID;

	SIMD_AND(BF_ELE)(dstRS.rBF->bf_base, srcRS.rBF->bf_base, pc->srcPort[f->src_port]->rbf->bf_base, pc->dstPort[f->dst_port]->rbf->bf_base, pc->protocol[f->proto]->rbf->bf_base);

	for (unsigned int i = 0; i < dstRS.rules->size(); i++) {
#if 1
		if (dstRS.rules->at(i) < maxid)
			continue;
#endif
		if (dstRS.rBF->query(dstRS.rules->at(i)))
		{
			if (second_confirmation(pcr, dstRS.rules->at(i), f))
				return dstRS.rules->at(i);
#if 0
			else
			{
				sec_err++;
			}
#endif
		}
	}
#endif
	return (unsigned short)-1;
}
