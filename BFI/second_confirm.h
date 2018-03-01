#ifndef SECOND_CONFIRM_H
#define SECOND_CONFIRM_H

#include <string.h>
#include <stdint.h>
#include <string>
#include <fstream>
#include <iostream>
#include "define.h"
#include "DataProcessing.h"

using namespace std;

struct orig_rule {
    uint32_t src_ip;
    uint32_t dst_ip;

    uint32_t src_mask;
    uint32_t dst_mask;

    uint16_t  src_port_start;
    uint16_t  src_port_end;

    uint16_t dst_port_start;
    uint16_t dst_port_end;

    uint8_t protocol;
    uint8_t proto_mask;
};


struct trans_rule {
    uint64_t ips;
    uint8_t proto;
};

struct trans_rule_mask {
    uint64_t ip_mask;
    uint8_t proto_mask;
};

struct pc_rules {
    struct orig_rule ors[MAX_RULES];
    struct trans_rule trs[MAX_RULES];
    struct trans_rule_mask trms[MAX_RULES];
};

int rules_init(struct pc_rules *pcr, string ruleFile);
int second_confirmation(struct pc_rules *pcr, unsigned int ruleID, struct flow *f);

#endif
