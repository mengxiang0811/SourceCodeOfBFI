#include "classifier.h"

#include <math.h>
#include <stdio.h>
#include <fstream>
#include <iostream>

#define IP_LEN		32

//const char * ribFile		= "acl5";					//original Rib file
//acl1_100_trace.txt
const char * ribFile		= "acl1_100.txt";					//original Rib file
const char * updateFile		= "updates.txt"; 			//update file in IP format

const char * oldPortfile    = "oldport-Opt.txt";
const char * oldPortfile_bin= "oldport_bin.txt";
const char * newPortfile    = "newport-Opt.txt";
const char * newPortfile_bin= "newport_bin.txt";

const char * trace_path		= "trace(100000).integer";	//"rand_trace(100000).integer";//
const char * ribfileName	= "rib.txt.port";

#define UpdateFileCount		6
#define UPDATE_TIME		"update.stat"
#define UPDATE_ALG	_MINI_REDUANDANCY_TWOTRAS

char ret[IP_LEN+1];


//given a ip in binary---str and its length---len, return the next ip in binary
char * GetStringIP(char *str, int len)
{
	memset(ret,0,sizeof(ret));
	memcpy(ret,str,IP_LEN);
	int i;
	for (i=0;i<len;i++)
	{
		if ('0'==ret[i])
		{
			ret[i]='1';
			break;
		}
		else if ('1'==ret[i])
		{
			ret[i]='0';
		}
	}
	//printf("%s*\n",ret);
	return ret;
}

unsigned int btod(char *bstr)
{
	unsigned int d = 0;
	unsigned int len = (unsigned int)strlen(bstr);
	if (len > 32)
	{
		printf("too long\n");
		return -1; 
	}
	len--;
	for (unsigned int i = 0; i <= len; i++)
	{
		d += (bstr[i] - '0') * (1 << (len - i));
	}

	return d;
}

void trieDetectForFullIp(CFib *tFib, CFib *optFib) {
#if 0
	int nonRouteStatic=0;

	int hop1=0;
	int hop2=0;

	char strIP00[IP_LEN + 1];
	memset(strIP00, 0, sizeof(strIP00));
	
	for (int tmp=0; tmp < IP_LEN; tmp++)
	{
		strIP00[tmp]='0';
	}

	int len88 = strlen(strIP00);

	char new_tmp[IP_LEN + 1];
	char old_tmp[IP_LEN + 1];

	memset(new_tmp, 0, sizeof(new_tmp));
	memset(new_tmp, 0, sizeof(new_tmp));
	memcpy(new_tmp, strIP00, IP_LEN);

	double zhishuI = pow((double)2,(double)IP_LEN);

	bool ifhalved = false;
	printf("\t\ttotal\t%.0f\t\n", zhishuI);
	printf("\t\tlength\tcycles\t\tpercent\tnexthop\n");

	for (long long k=0; k < zhishuI; k++)
	{
		memcpy(old_tmp, new_tmp, IP_LEN);
		memcpy(new_tmp, GetStringIP(old_tmp, IP_LEN), IP_LEN);
		
		hop1 = tFib->trieLookup(new_tmp);
		hop2 = optFib->optTrieLookup(new_tmp);

		//if (hop1== -1 && hop2 != hop1)
		//{
		//	nonRouteStatic++;
		//	continue;
		//}

		double ratio=0;
		
		if (hop2 != hop1)
		{
			printf("%d:%d", hop1, hop2);
			printf("\n\n\t\tNot Equal!!!: %s\n",new_tmp);
			getchar();
		}
		else 
		{
			//if (-1==hop1)nonRouteNum++;

			if (k%100000 == 0)
			{
				ratio=k/(double)(zhishuI/100);
				printf("\r\t\t%d\t%lld\t%.2f%%\t%d             ", IP_LEN, k, ratio, hop1);
			}
		}
	}

	printf("\n\t\tTotal number of garbage roaming route%d",nonRouteStatic);
	//printf("\n\t\tTotal number of Non-Route: %d\n",nonRouteNum);
	printf("\n\t\tEqual!!!!\n");
#endif
}


void blessDetectForFullIp(CFib *tFib) {
#if 1
	int nonRouteStatic=0;

#if 0
	int hop1=0;
	int hop2=0;
#endif

	char strIP00[IP_LEN + 1];
	memset(strIP00, 0, sizeof(strIP00));
	
	for (int tmp=0; tmp < IP_LEN; tmp++)
	{
		strIP00[tmp]='0';
	}

	int len88 = strlen(strIP00);

	char new_tmp[IP_LEN + 1];
	char old_tmp[IP_LEN + 1];

	memset(new_tmp, 0, sizeof(new_tmp));
	memset(new_tmp, 0, sizeof(new_tmp));
	memcpy(new_tmp, strIP00, IP_LEN);

	double zhishuI = pow((double)2,(double)IP_LEN);

	bool ifhalved = false;
	printf("\t\ttotal\t%.0f\t\n", zhishuI);
	printf("\t\tlength\tcycles\t\tpercent\tnexthop\n");

	for (long long k=0; k < zhishuI; k++)
	{
		memcpy(old_tmp, new_tmp, IP_LEN);
		memcpy(new_tmp, GetStringIP(old_tmp, IP_LEN), IP_LEN);
#if 0
        if (k == 43) {
            cout << "Here" << endl;
        }
#endif	

        struct ruleSet rs1;
        struct ruleSet rs2;

        rs1 = tFib->optTrieLookup(new_tmp);

        unsigned int IPInteger = btod(new_tmp);
        rs2 = tFib->blessLookup(IPInteger);

        //if (hop1== -1 && hop2 != hop1)
        //{
        //	nonRouteStatic++;
        //	continue;
        //}

#if 0
        if (rs1->rules.size() != 0) {
            cout << rs1->rules.size() << "\t" << rs1->minID << "\t" << rs2->rules.size() << "\t" << rs2->minID <<endl;
        }
#endif
        double ratio=0;
#if 0
        if (rs1.minID != rs2.minID) {
            printf("minID Not Equal! %llu***%u***%u\n", k, rs1.minID, rs2.minID);
            getchar();
        }
#endif
        if (rs1.minID != rs2.minID && rs1.rules.size() != rs2.rules.size() && memcmp(&rs1, &rs2, sizeof(unsigned int) * (rs1.rules.size() + 1)) != 0)
        {
            printf("\n*********%lld,***%lu***%lu\n", k, rs1.rules.size(), rs2.rules.size());
            printf("\tNot Equal!!!:\t %s\n",new_tmp);
            getchar();
        }
        else 
        {
            //if (-1==hop1)nonRouteNum++;

            if (k%100000 == 0)
            {
                ratio=k/(double)(zhishuI/100);
                printf("\r\t\t%d\t%lld\t%.2f%%\t%lu             ", IP_LEN, k, ratio, rs1.rules.size());
            }
        }

#if 0
        if (rs1) {
            vector<unsigned int>().swap(rs1->rules);
            free(rs1);
        }
        else {
            cout << "Error poiner!" << endl;
        }

        if (rs2) {
            vector<unsigned int>().swap(rs2->rules);
            free(rs2);
        }
        else {
            cout << "Error poiner!" << endl;
        }

        rs1 = NULL;
        rs2 = NULL;
#endif
    }

    printf("\n\t\tTotal number of garbage roaming route%d",nonRouteStatic);
    //printf("\n\t\tTotal number of Non-Route: %d\n",nonRouteNum);
    printf("\n\t\tEqual!!!!\n");
#endif
}

struct pc_rules pcr;
struct packet_classifier pkt_classifier;

void init_hash_function() {

	uint all_ones = (1 << BF_ELE_BIT) - 1;

	for (uint i = 0; i <= MAX_RULES; i++) {
		uint v = TWMX(i);

		uint p1 = v & all_ones;
		pos_div[i][0] = (p1 >> 3);
		pos_mod[i][0] = (128 >> (p1 & 7));

		if (BF_HF <= 1) continue;

		v = (v >> BF_ELE_BIT);
		uint p2 = v & all_ones;
		pos_div[i][1] = (p2 >> 3);
		pos_mod[i][1] = (128 >> (p2 & 7));

		if (BF_HF <= 2) continue;

		v = (v >> BF_ELE_BIT);
		uint p3 = v & all_ones;
		pos_div[i][2] = (p3 >> 3);
		pos_mod[i][2] = (128 >> (p3 & 7));
		if (BF_HF <= 3) continue;

		v = (v >> BF_ELE_BIT);
		uint p4 = v ^ p1;
		pos_div[i][3] = (p4 >> 3);
		pos_mod[i][3] = (128 >> (p4 & 7));

	}
}

int main_test(int argc, char** argv) {

	init_hash_function();

	rules_init(&pcr, argv[1]);

	classifier_init(&pkt_classifier, argv[1]);

    performance(&pkt_classifier, &pcr, argv[2]);

    return 0;
}

void main_yt(int argc, char** argv)
{
    //test(argc, argv);
    if (argc>1)
    {
        CFib aFIB = CFib();
        aFIB.TestLookupSpeed(argv[1], argv[2]);
    }
}

int main(int argc, char** argv)
{
	main_test(argc,argv);
    
	//main_yt(argc, argv);//≤‚ ‘ÀŸ∂»

 //   printf("Mission Complete, Press any key to continue...\n");

	//system("pause");
    return 0;
}
