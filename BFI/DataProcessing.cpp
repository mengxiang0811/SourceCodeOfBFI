#include "DataProcessing.h"

int trace_rule_num;

struct flow* read_trace_file(char* traceFile) {
	FILE *fin;

	if (fopen_s(&fin, traceFile, "r") != 0) {
		cout << "Open trace file error!" << endl;
		exit(1);
	}

	struct flow *flows = new struct flow[MAX_TRACES];
	trace_rule_num = 0;

	int ret = 0;
	while (true) {
		struct flow f;
		int ret = fscanf_s(fin, "%u %u %u %u %u %u", &f.src_ip, &f.dst_ip, &f.src_port, &f.dst_port, &f.proto, &f.trueRID);
		if (ret != 6)
			break;
		flows[trace_rule_num++] = f;
	}

	fclose(fin);
	return flows;
}