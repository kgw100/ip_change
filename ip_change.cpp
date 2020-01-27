#include <checksum.h>
#include <sfdafx.h>
#include <flow.h>

int cb(struct nfq_q_handle *q_handle, struct nfgenmsg *nfmsg,
          struct nfq_data *nfa, void *data)
{
//    static unordered_map<tuple_key, vector<uint32_t>,tuple_hash,key_equal> CgData_HashMap;
//    static unordered_map<tuple_key, vector<uint32_t>,tuple_hash,key_equal>::iterator iter;
    int id = 0;
    struct nfqnl_msg_packet_hdr *ph;
    uint32_t payload_len;
    u_char *cg_data;
    string cg_data_str;
    uint16_t cg_data_len ;
    int i = 0;
    ph = nfq_get_msg_packet_hdr(nfa);
    if (ph ) {
        id = ntohl(ph->packet_id);
    }
    payload_len =static_cast<uint32_t>(nfq_get_payload(nfa, &cg_data));

    if(payload_len != 0){
      struct iphdr * iph =reinterpret_cast<struct iphdr*>(cg_data);
      struct tcphdr * tcph = reinterpret_cast<struct tcphdr*>(cg_data + iph->ihl*4);
      if(iph->protocol ==6){
      uint8_t * payload = cg_data + (iph->ihl*4) + (tcph->th_off*4);
      uint8_t temp_data[4096];



      get_checksum_ip(temp_data);
      get_checksum_tcp(temp_data);

      printf("------------------------------------------------------------\n");
      return nfq_set_verdict(q_handle, id, NF_ACCEPT,payload_len, cg_data);
        }
    }
     return nfq_set_verdict(q_handle, id, NF_ACCEPT,0, nullptr);
}

char * getMYIP()
{
    char * myip;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    const char* kGoogleDnsIp = "8.8.8.8";
    int kDnsPort = 53;

    struct sockaddr_in serv;
    struct sockaddr_in host_name;

    memset(&serv, 0, sizeof(serv));

    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr(kGoogleDnsIp);
    serv.sin_port = htons(kDnsPort);

    if( connect(sockfd, (struct sockaddr *)&serv, sizeof(serv)) < 0 ) printf("[-] sock connect for get ipaddr faild!\n");

    socklen_t host_len = sizeof(host_name);
    if( getsockname(sockfd, (struct sockaddr *)&host_name, &host_len) < 0 ) printf("[-] getsockname faild!\n");

    inet_ntop(AF_INET, &host_name.sin_addr, myip, sizeof(myip));
    close(sockfd);

    return myip;
}

int ip_change(unsigned char *data, int pay_len){
}
