#ifndef DATAPROCESSING_H
#define DATAPROCESSING_H

#include <iostream>
#include <stdio.h>
#include "define.h"
using namespace std;

#define MAX_TRACES 100000

extern int trace_rule_num;

struct flow {
	unsigned int src_ip;
	unsigned int dst_ip;

	unsigned int src_port;
	unsigned int dst_port;

	unsigned int proto;
	unsigned int trueRID;
};

struct flow* read_trace_file(char* traceFile);

#endif