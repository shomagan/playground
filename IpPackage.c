#include <stdio.h>
#include <string.h>
#include <math.h>
  unsigned char point;
  char a_char[50];
  char common_sequence_char[50];
  char max_common_sequence_char[50];
  char b_char[50];
  char b_start_symbol;
  unsigned char point_a,point_b;
  unsigned char i,j,k,number_b_char,number_a_char,number_b_char_reduce,number_a_char_reduce,common_char_number,max_common_char_number;

struct __attribute__((packed)) ipv4_hdr{
  uint8_t vhl,
    tos,
    len[2],
    ipid[2],
    ipoffset[2],
    ttl,
    proto;
  uint16_t ipchksum;
  uip_ip4addr_t srcipaddr, destipaddr;
};

#define EPHEMERAL_PORTRANGE 1024

#define IPV4_HDRLEN 20

#define IP_PROTO_ICMPV4  1
#define IP_PROTO_TCP     6
#define IP_PROTO_UDP     17
#define IP_PROTO_ICMPV6  58

#define ICMP_ECHO_REPLY  0
#define ICMP_ECHO        8
#define ICMP6_ECHO_REPLY 129
#define ICMP6_ECHO       128

struct tcp_hdr {
  uint16_t srcport;
  uint16_t destport;
  uint8_t seqno[4];
  uint8_t ackno[4];
  uint8_t tcpoffset;
  uint8_t flags;
  uint8_t  wnd[2];
  uint16_t tcpchksum;
  uint8_t urgp[2];
  uint8_t optdata[4];
};

struct udp_hdr {
  uint16_t srcport;
  uint16_t destport;
  uint16_t udplen;
  uint16_t udpchksum;
};

struct icmpv4_hdr {
  uint8_t type, icode;
  uint16_t icmpchksum;
};

#define BUFSIZE 512

static uip_ip4addr_t ip64_hostaddr;
static uip_ip4addr_t ip64_netmask;
static uip_ip4addr_t ip64_draddr;

static uint16_t ipid;
static uint8_t ip64_hostaddr_configured = 0;

static uip_ip6addr_t ipv6_local_address;
static uint8_t ipv6_local_address_configured = 0;

static uip_ip4addr_t ipv4_broadcast_addr;

uip_ip6addr_t gate_address;

/* Lifetimes for address mappings. */
#define SYN_LIFETIME     (CLOCK_SECOND * 20)
#define RST_LIFETIME     (CLOCK_SECOND * 30)
#define DEFAULT_LIFETIME (CLOCK_SECOND * 60 * 5)

/* TCP flag defines */
#define TCP_FIN 0x01
#define TCP_SYN 0x02
#define TCP_RST 0x04
/*---------------------------------------------------------------------------*/
static uint16_t
chksum(uint16_t sum, const uint8_t *data, uint16_t len)
{
  uint16_t t;
  const uint8_t *dataptr;
  const uint8_t *last_byte;

  dataptr = data;
  last_byte = data + len - 1;

  while(dataptr < last_byte) {	/* At least two more bytes */
    t = (dataptr[0] << 8) + dataptr[1];
    sum += t;
    if(sum < t) {
      sum++;		/* carry */
    }
    dataptr += 2;
  }

  if(dataptr == last_byte) {
    t = (dataptr[0] << 8) + 0;
    sum += t;
    if(sum < t) {
      sum++;		/* carry */
    }
  }

  /* Return sum in host byte order. */
  return sum;
}
/*---------------------------------------------------------------------------*/
static uint16_t
ipv4_checksum(struct ipv4_hdr *hdr)
{
  uint16_t sum;

  sum = chksum(0, (uint8_t *)hdr, IPV4_HDRLEN);
  return (sum == 0) ? 0xffff : htons(sum);
}

static int
ip64_4to6(const uint8_t *ipv4packet, const uint16_t ipv4packet_len,
	  uint8_t *resultpacket)
{
  struct ipv4_hdr *v4hdr;
  struct ipv6_hdr *v6hdr;
  struct udp_hdr *udphdr;
  struct tcp_hdr *tcphdr;
  struct icmpv4_hdr *icmpv4hdr;
  struct icmpv6_hdr *icmpv6hdr;
  uint16_t ipv4len, ipv6len, ipv6_packet_len;
  struct ip64_addrmap_entry *m;

  v6hdr = (struct ipv6_hdr *)resultpacket;
  v4hdr = (struct ipv4_hdr *)ipv4packet;

  if((v4hdr->len[0] << 8) + v4hdr->len[1] <= ipv4packet_len) {
    ipv4len = (v4hdr->len[0] << 8) + v4hdr->len[1];
  } else {
    PRINTF("ip64_4to6: packet smaller than reported in IPv4 header, dropping\n");
    return 0;
  }

  if(ipv4len <= IPV4_HDRLEN) {
    return 0;
  }

  /* Make sure that the resulting packet fits in the ip64 packet
     buffer. If not, we drop it. */
  if(ipv4len - IPV4_HDRLEN + IPV6_HDRLEN > BUFSIZE) {
    PRINTF("ip64_4to6: packet too big to fit in buffer, dropping\n");
    return 0;
  }
  /* We copy the data from the IPv4 packet into the IPv6 packet. */
  memcpy(&resultpacket[IPV6_HDRLEN],
	 &ipv4packet[IPV4_HDRLEN],
	 ipv4len - IPV4_HDRLEN);
  
  udphdr = (struct udp_hdr *)&resultpacket[IPV6_HDRLEN];
  tcphdr = (struct tcp_hdr *)&resultpacket[IPV6_HDRLEN];
  icmpv4hdr = (struct icmpv4_hdr *)&ipv4packet[IPV4_HDRLEN];
  icmpv6hdr = (struct icmpv6_hdr *)&resultpacket[IPV6_HDRLEN];

  ipv6len = ipv4len - IPV4_HDRLEN + IPV6_HDRLEN;
  ipv6_packet_len = ipv6len - IPV6_HDRLEN;

  /* Translate the IPv4 header into an IPv6 header. */

  /* We first fill in the simple fields: IP header version, traffic
     class and flow label, and length fields. */
  v6hdr->vtc = 0x60;
  v6hdr->tcflow = 0;
  v6hdr->flow = 0;
  v6hdr->len[0] = ipv6_packet_len >> 8;
  v6hdr->len[1] = ipv6_packet_len & 0xff;

  /* We use the IPv4 TTL field as the IPv6 hop limit field. */
  v6hdr->hoplim = v4hdr->ttl;

  
  /* We now translate the IPv4 source and destination addresses to
     IPv6 source and destination addresses. We translate the IPv4
     source address into an IPv6-encoded IPv4 address. The IPv4
     destination address will be the address with which we have
     previously been configured, through the ip64_set_ipv4_address()
     function. We use the mapping table to look up the new IPv6
     destination address. As we assume that the IPv4 packet is a
     response to a previously sent IPv6 packet, we should have a
     mapping between the (protocol, destport, srcport, srcaddress)
     tuple. If not, we'll return 0 to indicate that we failed to
     translate the packet. */
  if(ip64_addr_4to6(&v4hdr->srcipaddr, &v6hdr->srcipaddr) == 0) {
    PRINTF("ip64_packet_4to6: failed to convert source IP address\n");
    return 0;
  }
	if(ip64_addr_4to6(&v4hdr->destipaddr, &v6hdr->destipaddr) == 0) {
    PRINTF("ip64_packet_4to6: failed to convert source IP address\n");
    return 0;
  }
    /* For the next header field, we simply use the IPv4 protocol
     field. We only support UDP and TCP packets. */
  switch(v4hdr->proto) {
  case IP_PROTO_UDP:
    v6hdr->nxthdr = IP_PROTO_UDP;
    break;

  case IP_PROTO_TCP:
    v6hdr->nxthdr = IP_PROTO_TCP;
    break;

  case IP_PROTO_ICMPV4:
    /* Allow only ICMPv4 ECHO_REQUESTS (ping packets) through to the
       local IPv6 host. */
    if(icmpv4hdr->type == ICMP_ECHO) {
      PRINTF("ip64_4to6: translating ICMPv4 ECHO packet\n");
      v6hdr->nxthdr = IP_PROTO_ICMPV6;
      icmpv6hdr->type = ICMP6_ECHO;
      //ip64_addr_copy6(&v6hdr->destipaddr, &ipv6_local_address);
    }else if(icmpv4hdr->type == ICMP_ECHO_REPLY){
      PRINTF("ip64_4to6: translating ICMPv4 ECHO packet\n");
      v6hdr->nxthdr = IP_PROTO_ICMPV6;
      icmpv6hdr->type = ICMP6_ECHO_REPLY;
    } else {
      PRINTF("ip64_packet_4to6: ICMPv4 packet type %d not supported\n",
	     icmpv4hdr->type);
      return 0;
    }
    break;

  default:
    /* For protocol types that we do not support, we return 0 to
       indicate that we failed to translate the packet to an IPv6
       packet. */
    PRINTF("ip64_packet_4to6: protocol type %d not supported\n",
	   v4hdr->proto);
    return 0;
  }
  /* The checksum is in different places in the different protocol
     headers, so we need to be sure that we update the correct
     field. */
  switch(v6hdr->nxthdr) {
  case IP_PROTO_TCP:
    tcphdr->tcpchksum = 0;
    tcphdr->tcpchksum = ~(ipv6_transport_checksum(resultpacket,
						  ipv6len,
						  IP_PROTO_TCP));
    break;
  case IP_PROTO_UDP:
    udphdr->udpchksum = 0;
    udphdr->udpchksum = ~(ipv6_transport_checksum(resultpacket,
						  ipv6len,
						  IP_PROTO_UDP));
    if(udphdr->udpchksum == 0) {
      udphdr->udpchksum = 0xffff;
    }
    break;

  case IP_PROTO_ICMPV6:
    icmpv6hdr->icmpchksum = 0;
    icmpv6hdr->icmpchksum = ~(ipv6_transport_checksum(resultpacket,
                                                ipv6len,
                                                IP_PROTO_ICMPV6));
    break;
  default:
    PRINTF("ip64_4to6: transport protocol %d not implemented\n", v4hdr->proto);
    return 0;
  }

  /* Finally, we return the length of the resulting IPv6 packet. */
  PRINTF("ip64_4to6: ipv6len %d\n", ipv6len);
  return ipv6len;
}

int main(int argc, const char * argv[]) {
    FILE *file = fopen(argv[1], "r");
    char line[1024];
    while (fgets(line, 1024, file)) {
        // Do something with the line
        char c;
//        c = 0x2c;// ","
        c = 0x3b;// ";"
        b_start_symbol = 0;
        number_b_char=number_a_char=number_b_char_reduce=number_a_char_reduce=0;
        max_common_char_number = 0;
        int line_len = strlen(line);
        if (line_len>1){
            int len;
//                printf(" %i %i \n",line[n-1],line[n]);          
//--------------------first-----------------------
            for (i=0;i<(line_len);i++){ //find right string(i>0)
              if (line[i]==c){
                b_start_symbol = i+1;
              }else{
                if ((line[i]==0x20||line[i]==0x0a||line[i]==0x00)){
                }else{
                  if (b_start_symbol){
                    b_char[number_b_char] = line[i];
                    number_b_char++;
                  }else{
                    a_char[number_a_char] = line[i];
                    number_a_char++;
                  }
                }
              }
            }
            for (j=0;j<number_a_char;j++){
              for (k=0;k<number_b_char;k++){
                if (line[j]==b_char[k]){
                  break;
                }
              }
              if (k<number_b_char){
                a_char[number_a_char_reduce] = line[j];
                number_a_char_reduce++;
              }
            }
            // write symbol if we have in both string
            for (j=0;j<number_b_char;j++){
              for (k=0;k<number_a_char;k++){
                if (line[j+b_start_symbol]==a_char[k]){
                  break;
                }
              }
              if (k<number_a_char){
                b_char[number_b_char_reduce] = line[j+b_start_symbol];
                number_b_char_reduce++;
              }
            }
//--------------------first_end-----------------------
/*            printf("%s\n",line);
            for (j = 0;j<(number_a_char_reduce);j++){
              printf("%c",a_char[j]);
            }
              printf("\n");
            for (j = 0;j<(number_b_char_reduce);j++){
              printf("%c",b_char[j]);
            }
              printf("\n");*/

        find(0,0);
        unsigned char n;
        for (n = 0;n<(max_common_char_number);n++){
          printf("%c",max_common_sequence_char[n]);
        }
          printf("\n");

        }
//            printf("0\n");
    }
    return 0;
}
//            printf("number_a_char_reduce,number_b_char_reduce,%i%i",number_a_char_reduce,number_b_char_reduce);
/*            printf("%s\n",line);
            for (j = 0;j<(number_a_char_reduce);j++){
              printf("%c",a_char[j]);
            }
              printf("\n");
            for (j = 0;j<(number_b_char_reduce);j++){
              printf("%c",b_char[j]);
            }
              printf("\n");*/

            // write symbol if we have in both string
//            printf("number_a_char,number_b_char,%i  %i",number_a_char,number_b_char);
/*            for (j = 0;j<(number_a_char);j++){
              printf("%c",a_char[j]);
            }
              printf("\n");
            for (j = 0;j<(number_b_char);j++){
              printf("%c",b_char[j]);
            }
              printf("\n");
*/
  