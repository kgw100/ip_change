#include <checksum.h>

#pragma pack(push,1)
struct cs_hdr{
    uint32_t sip;
    uint32_t dip;
    uint8_t rsv = 0;
    uint8_t proto;
    uint16_t tcp_len;
};
#pragma pack(pop)

uint16_t calc_checksum(uint16_t * data, uint32_t data_len)
{
    uint32_t temp_checksum = 0;
    uint16_t checksum;

    uint32_t cnt, flow;

    if(data_len % 2 == 0)
    {
        cnt = data_len / 2;
        flow = 0;
    }
    else {
        cnt = (data_len / 2) + 1;
        flow = 1;
    }

    for(int i = 0; i < cnt; i++)
    {
        if((i + 1) == cnt && flow == 1)
            temp_checksum += ntohs((data[i] & 0x00ff));
        else
            temp_checksum += ntohs(data[i]);

    }

    temp_checksum = ((temp_checksum >> 16) & 0xffff) + temp_checksum & 0xffff;
    temp_checksum = ((temp_checksum >> 16) & 0xffff) + temp_checksum & 0xffff;

    checksum = temp_checksum;

    return checksum;
}
uint16_t get_checksum_ip(uint8_t * data)
{
    struct iphdr * iph = reinterpret_cast<struct iphdr*>(data);
    uint16_t checksum;
    in_addr sip,dip;
    memcpy(&(sip).s_addr,&iph->saddr,sizeof(sip));
    memcpy(&(dip).s_addr,&iph->daddr,sizeof(dip));
    string src_ip =string(inet_ntoa(sip));
    string dst_ip =string(inet_ntoa(dip));
    cout<<"SIP:"<< src_ip;
    cout<<" DIP:"<< dst_ip;
    iph->check = 0;
    checksum = calc_checksum(reinterpret_cast<uint16_t *>(iph), (iph->ihl*4));

    iph ->check = htons(checksum ^ 0xffff); //for tcp checksum

    return iph->check;
}

uint16_t get_checksum_tcp(uint8_t * data)
{
    struct iphdr * iphdr = reinterpret_cast<struct iphdr*>(data);
    struct tcphdr * tcphdr = reinterpret_cast<struct tcphdr*>(data + iphdr->ihl*4);

    struct cs_hdr cs_hdr;

    memcpy(&cs_hdr.sip, &iphdr->saddr, sizeof(uint32_t));
    memcpy(&cs_hdr.dip, &iphdr->daddr, sizeof(uint32_t));
    cs_hdr.rsv = 0;
    cs_hdr.proto = iphdr->protocol;
    cs_hdr.tcp_len = htons(ntohs(iphdr->tot_len) - (iphdr->ihl*4));

    uint16_t temp_checksum_pseudo, temp_checksum_tcp, checksum;
    uint32_t temp_checksum;
    tcphdr->check = 0;

    temp_checksum_pseudo = calc_checksum(reinterpret_cast<uint16_t *>(&cs_hdr), sizeof(cs_hdr));
    temp_checksum_tcp = calc_checksum(reinterpret_cast<uint16_t *>(tcphdr), ntohs(cs_hdr.tcp_len));

    temp_checksum = temp_checksum_pseudo + temp_checksum_tcp;

    temp_checksum = ((temp_checksum >> 16) & 0xffff) + temp_checksum & 0xffff;

    temp_checksum = ((temp_checksum >> 16) & 0xffff) + temp_checksum & 0xffff;

    checksum = temp_checksum;

    tcphdr->check = htons(checksum ^ 0xffff);

    return tcphdr->check;
}
