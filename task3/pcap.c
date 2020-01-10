#include <stdio.h>
#include <string.h>  
#include <stdlib.h>  
#include <pcap.h>  
#include <ctype.h>
  
void strings(u_char *arg, const struct pcap_pkthdr *pkthdr, const u_char *packet) {  
  int *cnt = (int *)arg;
  printf("-------------------------\n");
  printf("Count number: %d\nReceived size: %d\nContent:\n", ++(*cnt), pkthdr->len);  
  for (int i = 0; i < pkthdr->len; i++) {
    if (isprint(packet[i])) printf("%c", packet[i]);
    if ((i % 64 == 0 && i != 0) || i == pkthdr->len-1) printf("\n"); 
  }
}  
  
int main() {  
  char err[PCAP_ERRBUF_SIZE];
  memset(err, 0, PCAP_ERRBUF_SIZE);  
  char *device = pcap_lookupdev(err);
  printf("Device using is %s\n", device);
  int cnt = 0;
  pcap_loop(pcap_open_live(device, 2048/*max 2-bit capture*/, 1, 512, err),
    -1, strings, (u_char *)&cnt);
  return 0;  
}  