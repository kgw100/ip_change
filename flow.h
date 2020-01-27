#pragma once
#include <sfdafx.h>

class flow{

public:
    u_int64_t sip;
    u_int64_t dip;
    u_int16_t sport;
    u_int16_t dport;

    bool operator<(const flow& fw) const{
        if(this->sip<fw.sip||this->dip<fw.dip||this->sport<fw.sport||this->dport<fw.dport){
            return true;
        }else{
            return false;
        }
    }
};
int cb(struct nfq_q_handle *q_handle, struct nfgenmsg *nfmsg,
          struct nfq_data *nfa, void *data);
char * getMYIP();
int ip_change(unsigned char *data, int pay_len);
