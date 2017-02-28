#ifndef TRAFFICANALYZER_H
#define TRAFFICANALYZER_H

#include "trafficanalyzerdialog.h"
#include "pktbuffer.h"


#include <QThread>
#include<pcap.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<sys/socket.h>
#include<arpa/inet.h>
#include<net/ethernet.h>
#include<netinet/ip_icmp.h>
#include<netinet/udp.h>
#include<netinet/tcp.h>
#include<netinet/ip.h>

class TADialog;
class TrafficAnalyzer : public QThread
{
Q_OBJECT

public:
    explicit TrafficAnalyzer(QObject *parent = 0);
    TrafficAnalyzer(TADialog *parent,char *dev);
    TADialog *dialog;



public:
    void run();

     void process_ip_packet(const u_char * , int);
     void print_ip_packet(const u_char * , int);
     void print_tcp_packet(const u_char *  , int );
     void print_udp_packet(const u_char * , int);
     void print_icmp_packet(const u_char * , int );
     void PrintData (const u_char * , int);
static void process(u_char *args, const struct pcap_pkthdr *header, const u_char *buffer)
    {
        TrafficAnalyzer* ta = reinterpret_cast<TrafficAnalyzer*>(args);
        ta->process_packet(args,header,buffer);
    };
     void process_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *buffer);

public:
    FILE *logfile;
    char *device_name;
    struct sockaddr_in source,dest;
    int tcp,udp,icmp,others,igmp,total,i,j;
    CircularPktBuffer *pktbuff;
};

#endif // TRAFFICANALYZER_H
