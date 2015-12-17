#include <stdio.h>
#include <string.h>
#include <math.h>
typedef  unsigned char uint8_t ;
typedef  unsigned short uint16_t;
typedef  unsigned int uint32_t ;

typedef union __attribute__((packed)) uip_ip4addr_t {
  uint8_t  u8[4];			/* Initializer, must come first. */
  uint16_t u16[2];
} uip_ip4addr_t;

typedef struct __attribute__((packed)){
  uint8_t vhl,
    tos,
    len[2],
    ipid[2],
    ipoffset[2],
    ttl,
    proto;
  uint16_t ipchksum;
  uip_ip4addr_t srcipaddr, destipaddr;
}ipv4_hdr;

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


#define BUFSIZE 512

static uip_ip4addr_t ip64_hostaddr;
static uip_ip4addr_t ip64_netmask;
static uip_ip4addr_t ip64_draddr;

static uint16_t ipid;
static uint8_t ip64_hostaddr_configured = 0;

static uip_ip4addr_t ipv4_broadcast_addr;

/* Lifetimes for address mappings. */
#define SYN_LIFETIME     (CLOCK_SECOND * 20)
#define RST_LIFETIME     (CLOCK_SECOND * 30)
#define DEFAULT_LIFETIME (CLOCK_SECOND * 60 * 5)

/* TCP flag defines */
#define TCP_FIN 0x01
#define TCP_SYN 0x02
#define TCP_RST 0x04
/**
 * Convert an u16_t from host- to network byte order.
 *
 * @param n u16_t in host byte order
 * @return n in network byte order
 */
uint16_t
htons(uint16_t n)
{
  return ((n & 0xff) << 8) | ((n & 0xff00) >> 8);
}

/**
 * Convert an u16_t from network- to host byte order.
 *
 * @param n u16_t in network byte order
 * @return n in host byte order
 */
uint16_t
ntohs(uint16_t n)
{
  return htons(n);
}

/**
 * Convert an u32_t from host- to network byte order.
 *
 * @param n u32_t in host byte order
 * @return n in network byte order
 */
uint32_t
htonl(uint32_t n)
{
  return ((n & 0xff) << 24) |
    ((n & 0xff00) << 8) |
    ((n & 0xff0000UL) >> 8) |
    ((n & 0xff000000UL) >> 24);
}

/**
 * Convert an u32_t from network- to host byte order.
 *
 * @param n u32_t in network byte order
 * @return n in host byte order
 */
uint32_t
ntohl(uint32_t n)
{
  return htonl(n);
}

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
ip4_replace(uint8_t *ipv4packet, const uip_ip4addr_t ip_source,const uip_ip4addr_t ip_destination)
{
  ipv4_hdr *v4hdr;

  v4hdr = (ipv4_hdr *)ipv4packet;
  uip_ip4addr_t srcipaddr, destipaddr;
  v4hdr->srcipaddr = ip_source;
  v4hdr->destipaddr = ip_destination;
  v4hdr->ipchksum = 0;
  v4hdr->ipchksum = ~(ipv4_checksum(v4hdr));
  return 0xff;
}

int main(int argc, const char * argv[]) {
    FILE *file = fopen(argv[1], "r");
    char line[8096];
    unsigned int number;
    uip_ip4addr_t ip_source;
    uip_ip4addr_t ip_destination;
    while (fgets(line, 8096, file)) {
        // Do something with the line
        char point,tab;
        unsigned char j;
        unsigned int i;
        unsigned char ip_packet[1512];
        point = 0x2e;//"."
        tab = 0x20;  //" "
        int line_len = strlen(line);
        if (line_len>1){
          int len;
//                printf(" %i %i \n",line[n-1],line[n]);          
//--------------------find first ip address-----------------------
          ip_source.u8[0] = atoi(&line[0]);
          for (number=1,i=0;number<4 && i<line_len;i++){
            if (line[i]==point){
              ip_source.u8[number] = atoi(&line[i+1]);
              number++;
            }
          }
//--------------------find two ip address-------------------------
          for (;i<line_len;i++){
            if (line[i]==tab){
              ip_destination.u8[0] = atoi(&line[i]);
              i++;
              break;
            }
          }
          for (number=1;number<4 && i<line_len;i++){
            if (line[i]==point){
              ip_destination.u8[number] = atoi(&line[i+1]);
              number++;
            }
          }
//--------------------find ip packet-------------------------
          for (number=0;number<1512 && i<line_len;i++){
            if (line[i]==tab){
              ip_packet[number] = strtol(&line[i+1], NULL, 16);
//              ip_packet[number] = atoi(&line[i+1]);
              number++;
            }
          }
          ip4_replace(ip_packet,ip_source,ip_destination);

/*          for (j = 0;j<4;j++){
            printf("%i ",ip_source.u8[j]);
          }

          for (j = 0;j<4;j++){
            printf("%i ",ip_destination.u8[j]);
          }
          printf("\n");
  */
          for (j = 0;j<sizeof(ipv4_hdr);j++){
            printf("%02x ",ip_packet[j]);
          }


/*          for (i = 0;i<number;i++){
            printf("%i ",ip_packet[i]);
          }
  */
          printf("\n");

        }

//        printf("0\n");
    }
    return 0;
}
  