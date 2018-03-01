#include "second_confirm.h"

int rules_init(struct pc_rules *pcr, string ruleFile) {

    int ret = -1;
    int iRuleID = 0;
    char line[1024];

    ifstream fin(ruleFile.c_str());

    if (!fin) {
        cout << "Open file error!" << endl;
        return 0;
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
        
        ret = sscanf_s(line, "@%d.%d.%d.%d/%d %d.%d.%d.%d/%d %d : %d %d : %d 0x%x/0x%x", &src_ip1, &src_ip2, &src_ip3, &src_ip4, &src_ipm, &dst_ip1, &dst_ip2, &dst_ip3, &dst_ip4, &dst_ipm, &src_port1, &src_port2, &dst_port1, &dst_port2, &proto, &proto_m);

        if (ret == 16) {

            memset(&pcr->ors[iRuleID], 0, sizeof(struct orig_rule));
            memset(&pcr->trs[iRuleID], 0, sizeof(struct trans_rule));
            memset(&pcr->trms[iRuleID], 0, sizeof(struct trans_rule_mask));

            pcr->ors[iRuleID].src_ip += (src_ip1 << 24);
            pcr->ors[iRuleID].src_ip += (src_ip2 << 16);
            pcr->ors[iRuleID].src_ip += (src_ip3 << 8);
            pcr->ors[iRuleID].src_ip += src_ip4;

            if (src_ipm == 0) {
                pcr->ors[iRuleID].src_mask = (uint32_t)-1;
            } else {
                pcr->ors[iRuleID].src_mask = (1 << (32 - src_ipm)) - 1;
            }

            pcr->ors[iRuleID].dst_ip += (dst_ip1 << 24);
            pcr->ors[iRuleID].dst_ip += (dst_ip2 << 16);
            pcr->ors[iRuleID].dst_ip += (dst_ip3 << 8);
            pcr->ors[iRuleID].dst_ip += dst_ip4;

            if (dst_ipm == 0) {
                pcr->ors[iRuleID].dst_mask = (uint32_t)-1;
            } else {
                pcr->ors[iRuleID].dst_mask = (1 << (32 - dst_ipm)) - 1;
            }

            pcr->ors[iRuleID].src_port_start = src_port1;
            pcr->ors[iRuleID].src_port_end = src_port2;

            pcr->ors[iRuleID].dst_port_start = dst_port1;
            pcr->ors[iRuleID].dst_port_end = dst_port2;
            
            pcr->ors[iRuleID].protocol = proto;
            pcr->ors[iRuleID].proto_mask = (0xFF - proto_m);
            
            pcr->trs[iRuleID].ips = pcr->ors[iRuleID].src_ip;
            pcr->trs[iRuleID].ips <<= 32;
            pcr->trs[iRuleID].ips += pcr->ors[iRuleID].dst_ip;
            pcr->trs[iRuleID].proto = proto;

            pcr->trms[iRuleID].ip_mask = pcr->ors[iRuleID].src_mask;
            pcr->trms[iRuleID].ip_mask <<= 32;
            pcr->trms[iRuleID].ip_mask += pcr->ors[iRuleID].dst_mask;
            pcr->trms[iRuleID].proto_mask = (0xFF - proto_m);

            pcr->trs[iRuleID].ips |= pcr->trms[iRuleID].ip_mask;

            iRuleID++;
        }
        else {
            cout << "Error rule item!!!" << endl;
        }

        memset(line, 0, sizeof(line));
    }

    fin.close();

    return iRuleID;
}

int second_confirmation(struct pc_rules *pcr, unsigned int ruleID, struct flow *f) {
    struct trans_rule tRule;

    tRule.ips = f->src_ip;
    tRule.ips <<= 32;
    tRule.ips += f->dst_ip;

    //tRule.proto = f->proto;

#if 0
    if ((tRule.ips | pcr->trms[ruleID].ip_mask) == pcr->trs[ruleID].ips) {
        cout << "IP is right!" << endl;
    }

    if (pcr->trms[ruleID].proto_mask == 0xFF || ((f->proto | pcr->trms[ruleID].proto_mask) == pcr->trs[ruleID].proto)) {
        cout << "Protocol is right!" << endl;
    }
#endif

    if (((tRule.ips | pcr->trms[ruleID].ip_mask) == pcr->trs[ruleID].ips) && \
            (pcr->trms[ruleID].proto_mask == 0xFF || (f->proto | pcr->trms[ruleID].proto_mask) == pcr->trs[ruleID].proto)) {
        if ((pcr->ors[ruleID].src_port_start <= f->src_port) && (pcr->ors[ruleID].src_port_end >= f->src_port) && \
                (pcr->ors[ruleID].dst_port_start <= f->dst_port) && (pcr->ors[ruleID].dst_port_end >= f->dst_port)) {
            return 1;
        } else {
            return 0;
        }
    }

    return 0;
}
