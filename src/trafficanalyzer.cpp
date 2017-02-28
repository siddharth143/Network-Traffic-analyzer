#include "trafficanalyzer.h"
#include "trafficanalyzerdialog.h"


TrafficAnalyzer::TrafficAnalyzer(QObject *parent) :
    QThread(parent)
{

}
TrafficAnalyzer::TrafficAnalyzer(TADialog *parent, char *dev) :
    QThread(parent)
{
  dialog=parent;
  device_name=new char[100];
  strcpy(device_name,dev);
  pktbuff=new CircularPktBuffer(1024);
}



void TrafficAnalyzer::run()
{
    pcap_if_t *alldevsp , *device;
    pcap_t *handle; //Handle of the device that shall be sniffed

    char errbuf[100] , *devname , devs[100][100];
    int count = 1 , n;

    handle = pcap_open_live(device_name , 65536 , 1 , 0 , errbuf);

    if (handle == NULL)
    {
        fprintf(stderr, "Couldn't open device %s : %s\n" , device_name , errbuf);
        return;
    }

  printf("\nReady to capture eth1\n");
  pcap_loop(handle , -1 , &(TrafficAnalyzer::process), (unsigned char*)this);

}



void TrafficAnalyzer::process_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *buffer)
{


    int size = header->len;
    int payload_size=size-42;
//unsigned char buff[1024];
//memcpy(buff,buffer,size);
//Get the IP Header part of this packet , excluding the ethernet header
struct iphdr *iph = (struct iphdr*)(buffer + sizeof(struct ethhdr));
struct udphdr *udph=(struct udphdr*)(buffer + sizeof(struct ethhdr) + sizeof(struct iphdr));
    switch (iph->protocol)
    {
        case 1:  //ICMP Protocol

            print_icmp_packet( buffer , size);
            break;

        case 2:  //IGMP Protocol

            break;

        case 6:  //TCP Protocol

            print_tcp_packet(buffer , size);
            break;

        case 17: //UDP Protocol
            //printf("Received UDP packet on this inaterface\n");
            /*if((udph->source!=htons(9998))
                &&(udph->dest!=htons(9998))
                &&(udph->dest!=htons(9997))
                &&(udph->dest!=htons(9997))
                &&(udph->source>htons(1024))
                &&(udph->dest>htons(1024)))*/

            if(payload_size<=200)
            {
                if(htons(udph->source)==9999||htons(udph->dest)==9999)
                pktbuff->putMessage((unsigned char*)buffer,size);
                print_udp_packet(buffer , size);
            }
            break;

        default: //Some Other Protocol like ARP etc.
            break;
    }

}


void TrafficAnalyzer::process_ip_packet(const u_char * , int)
{

}

void TrafficAnalyzer::print_ip_packet(const u_char * , int)
{

}

void TrafficAnalyzer::print_tcp_packet(const u_char *  , int )
{

}

void TrafficAnalyzer::print_udp_packet(const u_char * , int)
{

}

void TrafficAnalyzer::print_icmp_packet(const u_char * , int )
{

}

void TrafficAnalyzer::PrintData (const u_char * , int)
{

}
