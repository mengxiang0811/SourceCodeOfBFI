#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include "Fib.h"
#include "second_confirm.h"

struct ppi {
	std_bf *rbf;
	unsigned short minID;
};

struct packet_classifier {
    CFib *srcFib;   // For the Source IP field
    CFib *dstFib;   // For the Destination IP field
    ppi *srcPort[65536]; // For the Source Port field
    ppi *dstPort[65536]; // For the Destination Port field
    ppi *protocol[256];  // For the Protocol field
};

void classifier_init(struct packet_classifier *pc, string flowFile);
void performance(struct packet_classifier *pc, struct pc_rules *pcr, char* traceFile);
unsigned int flowLookup(struct packet_classifier *pc, struct pc_rules *pcr, struct flow *f);
unsigned int flowLookup_choose(struct packet_classifier *pc, struct pc_rules *pcr, struct flow *f);
unsigned int flowLookup_dst(struct packet_classifier *pc, struct pc_rules *pcr, struct flow *f);

#endif
