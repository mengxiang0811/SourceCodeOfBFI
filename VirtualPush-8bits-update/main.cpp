#include "Fib.h"
#include <iostream>

#include <stdio.h>
#include <fstream>
#include <math.h>
#include <windows.h>


#define IP_LEN		32

const char * ribFile		= "rrc00.txt";					//original Rib file
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
}



void blessDetectForFullIp(CFib *tFib) {
	
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
#if 1
        if ( k == 89776) {
            printf("Here!\n");
        }
#endif

		memcpy(old_tmp, new_tmp, IP_LEN);
		memcpy(new_tmp, GetStringIP(old_tmp, IP_LEN), IP_LEN);
		
		hop1 = tFib->optTrieLookup(new_tmp);
	
		unsigned int IPInteger = btod(new_tmp);
		hop2 = tFib->blessLookup(IPInteger);

		//if (hop1== -1 && hop2 != hop1)
		//{
		//	nonRouteStatic++;
		//	continue;
		//}

		double ratio=0;
		
		if (hop2 != hop1)
		{
		    //printf("\n*********%lld\n", k);
			printf("\n%d:%d", hop1, hop2);
			printf("\tNot Equal!!!:\t %s\n",new_tmp);
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
}

unsigned int ytReadandUpdate(string sFileName,CFib *tFib)
{
	unsigned int	iEntryCount=0;		//the number of items from file
	char			sPrefix[20];		//prefix from rib file
	unsigned long	lPrefix;			//the value of Prefix
	unsigned int	iPrefixLen;			//the length of PREFIX
	int				iNextHop;			//to store NEXTHOP in RIB file

	char			operate_type_read;
	int 			operate_type;
	int				readlines=0;

	long			yearmonthday=0;		//an integer to record year, month, day
	long			hourminsec=0;		//an integer to record hour, minute, second
	long			yearmonthday_old=0;		//an integer to record year, month, day
	long			hourminsec_old=0;		//an integer to record hour, minute, second
	long			outputCount=0;
	long			insertNum_old=0;
	long			DelNum_old=0;
	long			readlines_old=0;

	unsigned long long	updatetimeused = 0;
	unsigned long long	updatetimeused_old = 0;

	LARGE_INTEGER frequence,p1,p2;
	if(!QueryPerformanceFrequency(&frequence))return 0;
	FILE * fp_u;
	fp_u=fopen("UPDATE_Stat.virtualpush", "w");

	for (int jjj=1;jjj<=UpdateFileCount;jjj++)
	{
		char strName[20];
		memset(strName,0,sizeof(strName));
		sprintf(strName,"updates%d.txt",jjj);

		ifstream fin(strName);
		if (!fin)
		{
			//printf("!!!error!!!!  no file named:%s\n",strName);
			continue;
		}
		printf("\nParsing %s\n",strName);

		memset(strName,0,sizeof(strName));
		sprintf(strName,"stat%d.txt",jjj);
        ofstream fout(strName);

        tFib->statInit();

		while (!fin.eof()) 
		{
		
			lPrefix = 0;
			iPrefixLen = 0;
			iNextHop = -9;

			memset(sPrefix,0,sizeof(sPrefix));
		//read data from rib file, iNextHop attention !!!
			fin >> yearmonthday >> hourminsec >> operate_type_read >> sPrefix;//>> iNextHop;

			if('W'==operate_type_read) {
				operate_type = _DELETE;
			}
			else if ('A'==operate_type_read) {
				fin >> iNextHop;
				operate_type=_NOT_DELETE;
			}
			else
			{
			printf("Format of update file Error, quit....\n");
				getchar();
				return 0;
			}

			int iStart=0;				//the end point of IP
			int iEnd=0;					//the end point of IP
			int iFieldIndex = 3;		
			int iLen=strlen(sPrefix);	//the length of Prefix

		
			if(iLen > 0)
			{
				if (yearmonthday-yearmonthday_old>0 || hourminsec-hourminsec_old>=100)
				{	
					yearmonthday_old=yearmonthday;
					hourminsec_old=hourminsec;
					//printf("%d%d\n",yearmonthday,hourminsec/100);

					int hour_format=hourminsec/100;
					char hour_string[20];
					memset(hour_string,0,sizeof(hour_string));
					if (0==hour_format)			sprintf(hour_string,"0000");
					if (hour_format<10)			sprintf(hour_string,"000%d",hour_format);
					else if (hour_format<100)	sprintf(hour_string,"00%d",hour_format);
					else if (hour_format<1000)	sprintf(hour_string,"0%d",hour_format);
					else						sprintf(hour_string,"%d",hour_format);

					printf("%d%s\t%u\t%u\t%u\t%u\t%.3fMups\n",yearmonthday,hour_string,readlines_old,readlines,updatetimeused_old, updatetimeused,(readlines-readlines_old)/(0.0+updatetimeused-updatetimeused_old));
					fprintf(fp_u,"%d%s\t%u\t%u\t%u\t%u\t%.3fMups\n",yearmonthday,hour_string,readlines_old,readlines,updatetimeused_old, updatetimeused,(readlines-readlines_old)/(0.0+updatetimeused-updatetimeused_old));

					readlines_old=readlines;
					updatetimeused_old=updatetimeused;
				}

				readlines++;
				for ( int i=0; i<iLen; i++ )
				{
				//extract the first 3 sub-part
					if ( sPrefix[i] == '.' )
					{
						iEnd = i;
						string strVal(sPrefix+iStart,iEnd-iStart);
						lPrefix += atol(strVal.c_str()) << (8 * iFieldIndex); 
						iFieldIndex--;
						iStart = i+1;
						i++;
					}
					if ( sPrefix[i] == '/' ){
					//extract the 4th sub-part
						iEnd = i;
						string strVal(sPrefix+iStart,iEnd-iStart);
						lPrefix += atol(strVal.c_str());
						iStart = i+1;

					//extract the length of prefix
						i++;
						strVal= string(sPrefix+iStart,iLen-1);
						iPrefixLen=atoi(strVal.c_str());
					}
				}

				char insert_C[50];
				memset(insert_C,0,sizeof(insert_C));
			//insert the current node into Trie tree
				for (unsigned int yi=0; yi<iPrefixLen; yi++)
				{
				//turn right
					if(((lPrefix<<yi) & HIGHTBIT)==HIGHTBIT)insert_C[yi]='1';
					else insert_C[yi]='0';
				}
				//printf("%s\/%d\t%d\n",insert_C,iPrefixLen,iNextHop);

				if (iPrefixLen<8) {
					printf("%d-%d; ",iPrefixLen,iNextHop);
				}
				else
				{
					QueryPerformanceCounter(&p1);

					tFib->Update(iNextHop,insert_C,operate_type, sPrefix, true);

					QueryPerformanceCounter(&p2);
					updatetimeused +=1000000*(p2.QuadPart-p1.QuadPart)/frequence.QuadPart;
				}
			}
		}

        int ma = 0;

        printf("\nThe total invalid number of routing item is: %u", tFib->invalid);
        printf("\nThe total number of worst case routing item is: %lu", tFib->worst_num);

        for (ma = 0; ma < 33; ma++) {
            fout << ma << '\t' << tFib->maStat[ma] << endl;
        }

		fin.close();
        fout.close();
		fclose(fp_u);
	}

	return readlines;
}

int main_test (char *rFile) 
{
	CFib optFib = CFib();
	unsigned int iEntryCount = 0;
	unsigned int uEntryCount = 0;
	iEntryCount = optFib.BuildFibFromFile(rFile);

	optFib.ytGetNodeCounts();
	printf("\nThe total number of routing items in optFib file is \t%u, \nThe total number of solid Trie node is :\t%u,\noptFib.allNodeCount=%d\n",iEntryCount, optFib.solidNodeCount, optFib.allNodeCount);
#if 0
	optFib.ytOptLevelPushing(optFib.m_pTrie, 0, 0);
	optFib.ytGetNodeCounts();
	printf("\nAfter ytOptLevelPushing...\nThe total number of solid Trie node is :\t%u,\noptFib.allNodeCount=%d\n", optFib.solidNodeCount, optFib.allNodeCount);
#endif

#if 1
    uEntryCount = ytReadandUpdate("updates.txt", &optFib);
	optFib.ytGetNodeCounts();
	printf("\nAfter Update....\nThe total number of update items is \t%u,\nThe total number of solid Trie node is :\t%u,\noptFib.allNodeCount=%d\n", uEntryCount, optFib.solidNodeCount, optFib.allNodeCount);
    optFib.OutputTrie(optFib.m_pTrie, newPortfile, oldPortfile, true);
#endif

	//optFib.OutputTrie(optFib.m_pTrie, newPortfile, oldPortfile, false);
	
	//CFib tFib = CFib();		//build FibTrie
	//tFib.BuildFibFromFile(ribFile);

	//tFib.ytGetNodeCounts();
	//printf("\nThe total number of routing items in tFib file is \t%u, \nThe total number of solid Trie node is :\t%u,\ntFib.allNodeCount=%d\n",iEntryCount, tFib.solidNodeCount, tFib.allNodeCount);
	
	//ytReadandUpdate(updateFile, &optFib);

	//optFib.ytGetNodeCounts();
	//optFib.OutputTrie(optFib.m_pTrie, newPortfile, oldPortfile, false);

	//ytReadandUpdate(string sFileName,CFib *tFib);
	//printf("\nAfter Update....\nThe total number of solid Trie node is :\t%u,\noptFib.allNodeCount=%d\n", optFib.solidNodeCount, optFib.allNodeCount);

#if 0
	//level-pushing
	int default_port = PORT_MAX;

	if (tFib.m_pTrie->newPort > 0) {
		default_port = tFib.m_pTrie->newPort;
	}

	optFib.ytOptLevelPushing(optFib.m_pTrie, 0, 0);
	optFib.OutputTrie(optFib.m_pTrie, newPortfile, oldPortfile, true);
	optFib.ytGetNodeCounts();
	printf("\nAfter ytOptLevelPushing...\nThe total number of solid Trie node is :\t%u,\noptFib.allNodeCount=%d\n", optFib.solidNodeCount, optFib.allNodeCount);

	/*tFib.ytLevelPushing(tFib.m_pTrie, 0, default_port);
	  tFib.ytGetNodeCounts();
	  printf("\nAfter ytLevelPushing...\nThe total number of solid Trie node is :\t%u,\ntFib.allNodeCount=%d\n", tFib.solidNodeCount, tFib.allNodeCount);*/
#endif
	//optFib.ytOptLevelPushing(optFib.m_pTrie, 0, 0);
	//optFib.ytGetNodeCounts();
	//printf("\nAfter ytOptLevelPushing...\nThe total number of solid Trie node is :\t%u,\noptFib.allNodeCount=%d\n", optFib.solidNodeCount, optFib.allNodeCount);

	printf("\nTo test the correctness of the trie structure!!!\n");
	if (!optFib.isCorrectTrie(optFib.m_pTrie)) {
		printf("The optFib trie structure is ***wrong***!!!\n");
	}
	else {
		printf("The optFib trie structure is correct!\n");
	}

	printf("\n\n************************Bless Lookup Correct Test************************\n");
	//optFib.buildLookupTable(optFib.m_pTrie);
	/*printf("To compare the opt trie lookup with binary trie:\n");
	  trieDetectForFullIp(&tFib, &optFib);*/
	printf("\nTo compare the opt trie lookup with its BLESS lookup:\n");
	//blessDetectForFullIp(&optFib);
	printf("**************************************End**************************************\n");

	printf("Mission Complete, Press any key to continue...\n");

	return 0;
}


void main_lookupSpeed(char** argv)
{
    //test(argc, argv);
    CFib aFIB = CFib();
    aFIB.TestLookupSpeed(argv[2], argv[3]);
}

/*
 *  main -r ribfile
 *  or
 *  main -u tracefile ribfile
 *
 *  updates files' names must be updates#.txt
 *
 * */

void error_message() {
    printf("Error parameters!!!\n");
    printf("***parameter help***: \n");
    printf("opt1: main -r ribfile\n");
    printf("opt2: main -u tracefile ribfile\n");
}

int main(int argc, char** argv)
{
    if (argc < 3) {
        error_message();
        return -1;
    }

    if (strcmp(argv[1], "-r") == 0) {
        main_test(argv[2]); // To verify the correctness of the program
    } else if (strcmp(argv[1], "-u") == 0 && argc == 4) {
        main_lookupSpeed(argv); // To test the speed of lookup
    } else {
        error_message();
        return -1;
    }

    return 0;
}
