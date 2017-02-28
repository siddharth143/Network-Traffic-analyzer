#include "trafficanalyzerdialog.h"
#include "trafficanalyzer.h"
#include <math.h>
#include<sys/time.h>
#include<stdio.h>


TADialog::TADialog(QWidget *parent)


{
    DataTable=new QTableWidget(this);
    DataTable->setColumnCount(8);


    DataTable->setHorizontalHeaderLabels(QString("TimeStamp:Interface:SourceID:DestinationID:MessageID:Length:PacketRate:DataRate").split(":"));
    DataTable->setColumnWidth(0,150);
    DataTable->setColumnWidth(1,125);
    DataTable->setColumnWidth(2,125);
    DataTable->setColumnWidth(3,125);
    DataTable->setColumnWidth(4,125);
    DataTable->setColumnWidth(5,125);
    DataTable->setColumnWidth(6,125);

    setGeometry(0,0,1024,768);
    setCentralWidget(DataTable);

    connect(DataTable,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(onCellClicked(int,int)));

    pktflow=NULL;
    pktflow_count=0;
    ta_eth0 = new TrafficAnalyzer(this,"eth0");
    ta_eth1 = new TrafficAnalyzer(this,"lo");
    ta_eth0->start();
    ta_eth1->start();
    i=1;
    start();
    this->startTimer(1000);
}

TADialog::~TADialog()
{
    //delete ui;
}
void TADialog::execute()
{

    char buff[1024],time_buffer[1024],*udp_header,srcip[16],dstip[16];

    struct udphdr *udph;
    struct iphdr *iph;
    FecHeader *fecheader;
    int sid,did,mid,count=0,r=0,skt=0,pktrate=0,datarate;
    struct timeval *now=new struct timeval;
    unsigned long timestamp;
    char ip[16];


     while(1)
    {
      if((!ta_eth0->pktbuff->isEmpty())||(!ta_eth1->pktbuff->isEmpty()))
        {
            if(!ta_eth0->pktbuff->isEmpty())
            {
                length=ta_eth0->pktbuff->getMessage(buff);
                iph=(struct iphdr*)(buff+sizeof(struct ethhdr));
                udph = (struct udphdr*)(buff + sizeof(struct ethhdr) + sizeof(iphdr));
                udp_header=buff + sizeof(struct ethhdr) + sizeof(iphdr);
                strcpy(srcip,inet_ntoa(*(struct in_addr*) &iph->saddr));
                strcpy(dstip,inet_ntoa(*(struct in_addr*) &iph->daddr));
                fecheader=(FecHeader*)(udp_header+sizeof(udphdr));
                sid=fecheader->m_sourceId;
                did=fecheader->m_destinationId;
                mid=fecheader->m_msgId;
                //printf("\nMessage is %d %d %d\n",fecheader->m_sourceId,fecheader->m_destinationId,fecheader->m_msgId);

                //Fill the array
                for(count=0;count<pktflow_count;count++)
                {
                    if((pktflow[count].sid==sid)&&
                       (pktflow[count].did==did)&&
                       (pktflow[count].mid==mid)&&
                       (strcmp(pktflow[count].interface,"Eth0")==0))
                    {
                        pktflow[count].timestamp=getTimeStamp();
                        pktflow[count].size=length-42;
                        pktflow[count].packet_rate++;//=1000000/(timestamp-pktflow[count].timestamp);
                        pktflow[count].data_rate+=(length-42);
                        for(skt=0;skt<pktflow[count].no_flow;skt++)
                        {
                            //printf("%s->%s and ",pktflow[count].fsocket[skt].sip,pktflow[count].fsocket[skt].dip);
                            //printf("%s->%s\n",srcip,dstip);
                            if((strcmp(pktflow[count].fsocket[skt].sip,srcip)==0)&&
                               (strcmp(pktflow[count].fsocket[skt].dip,dstip)==0))
                            {

                                break;
                            }
                        }
                        if(skt==pktflow[count].no_flow)
                        {
                            pktflow[count].fsocket[skt].timestamp=getTimeStamp();
                            strcpy(pktflow[count].fsocket[skt].interface,"Eth0");
                            strcpy(pktflow[count].fsocket[skt].sip,srcip);
                            strcpy(pktflow[count].fsocket[skt].dip,dstip);
                            pktflow[count].no_flow++;
                            //printf("New Socket Entry count=%d is", pktflow[count].no_flow);
                            //printf("%s->%s\n\n",srcip,dstip);
                        }

                        break;
                    }

                }
                if(count==pktflow_count)
                {
                         //printf("New Entry\n");
                         pktflow_count++;
                         pktflow=(struct PacketFlow*)realloc(pktflow,sizeof(struct PacketFlow)*pktflow_count);
                         pktflow[count].data_available=true;
                         strcpy(pktflow[count].interface,"Eth0");
                         strcpy(pktflow[count].fsocket[0].interface,"Eth0");
                         pktflow[count].sid=sid;
                         pktflow[count].did=did;
                         pktflow[count].mid=mid;
                         pktflow[count].timestamp=getTimeStamp();
                         pktflow[count].packet_rate=1;
                         pktflow[count].data_rate=length-42;
                         pktflow[count].no_flow=1;
                         pktflow[count].size=length-42;
                         strcpy(pktflow[count].fsocket[0].sip,srcip);
                         strcpy(pktflow[count].fsocket[0].dip,dstip);
                         pktflow[count].fsocket[0].sport=htons(udph->source);
                         pktflow[count].fsocket[0].sport=htons(udph->dest);


                }


            }
            if(!ta_eth1->pktbuff->isEmpty())
            {
                length=ta_eth1->pktbuff->getMessage(buff);
                iph=(struct iphdr*)(buff+sizeof(struct ethhdr));
                udph = (struct udphdr*)(buff + sizeof(struct ethhdr) + sizeof(iphdr));
                udp_header=buff + sizeof(struct ethhdr) + sizeof(iphdr);
                strcpy(srcip,inet_ntoa(*(struct in_addr*) &iph->saddr));
                strcpy(dstip,inet_ntoa(*(struct in_addr*) &iph->daddr));
                fecheader=(FecHeader*)(udp_header+sizeof(udphdr));
                sid=fecheader->m_sourceId;
                did=fecheader->m_destinationId;
                mid=fecheader->m_msgId;
                //printf("\nMessage is %d %d %d\n",fecheader->m_sourceId,fecheader->m_destinationId,fecheader->m_msgId);

                //Fill the array
                for(count=0;count<pktflow_count;count++)
                {
                    if((pktflow[count].sid==sid)&&
                       (pktflow[count].did==did)&&
                       (pktflow[count].mid==mid)&&
                       (strcmp(pktflow[count].interface,"Eth1")==0))
                    {
                        pktflow[count].timestamp=getTimeStamp();
                        pktflow[count].size=length-42;
                        pktflow[count].packet_rate++;//=1000000/(timestamp-pktflow[count].timestamp);
                        pktflow[count].data_rate+=(length-42);
                        for(skt=0;skt<pktflow[count].no_flow;skt++)
                        {

                            if((strcmp(pktflow[count].fsocket[skt].sip,srcip)==0)&&
                               (strcmp(pktflow[count].fsocket[skt].dip,dstip)==0))
                            {

                                break;
                            }
                        }
                        if(skt==pktflow[count].no_flow)
                        {
                            pktflow[count].fsocket[skt].timestamp=getTimeStamp();
                            strcpy(pktflow[count].fsocket[skt].interface,"Eth1");
                            strcpy(pktflow[count].fsocket[skt].sip,srcip);
                            strcpy(pktflow[count].fsocket[skt].dip,dstip);
                            pktflow[count].no_flow++;
                            //printf("New Socket Entry count=%d is", pktflow[count].no_flow);
                            printf("%s->%s\n\n",srcip,dstip);
                        }

                        break;
                    }

                }
                if(count==pktflow_count)
                {
                         //printf("New Entry\n");
                         pktflow_count++;
                         pktflow=(struct PacketFlow*)realloc(pktflow,sizeof(struct PacketFlow)*pktflow_count);
                         pktflow[count].data_available=true;
                         strcpy(pktflow[count].interface,"Eth1");
                         strcpy(pktflow[count].fsocket[0].interface,"Eth1");
                         pktflow[count].sid=sid;
                         pktflow[count].did=did;
                         pktflow[count].mid=mid;
                         pktflow[count].timestamp=getTimeStamp();
                         pktflow[count].packet_rate=1;
                         pktflow[count].data_rate=length-42;
                         pktflow[count].no_flow=1;
                         pktflow[count].size=length-42;
                         strcpy(pktflow[count].fsocket[0].sip,srcip);
                         strcpy(pktflow[count].fsocket[0].dip,dstip);
                         pktflow[count].fsocket[0].sport=htons(udph->source);
                         pktflow[count].fsocket[0].sport=htons(udph->dest);


                }

            }


        }
      else
      {
          usleep(1);
          continue;
      }
    }
}
void  TADialog :: getTime(char *ptime_string)
{
    struct timeval m_timeVal;


    gettimeofday( &m_timeVal, NULL);
    struct tm *temp_tm;

    temp_tm= localtime( (time_t *)&m_timeVal.tv_sec );
    if(temp_tm != NULL)
    {
        sprintf( ptime_string, "%02d:%02d:%02d:%06d",
                 temp_tm->tm_hour,
                 temp_tm->tm_min,
                 temp_tm->tm_sec,m_timeVal.tv_usec);
    }

}

void  TADialog :: intToStr(unsigned long ltim, char *pstim)
{
    struct tm *temp_tm;
    struct timeval tmvl;
    tmvl.tv_sec=ltim/1000000;
    tmvl.tv_usec=ltim%1000000;
    temp_tm= localtime( (time_t *)&tmvl.tv_sec );
    if(temp_tm != NULL)
    {
        sprintf( pstim, "%02d:%02d:%02d:%06d",
                 temp_tm->tm_hour,
                 temp_tm->tm_min,
                 temp_tm->tm_sec,ltim%1000000);
    }
}
unsigned long TADialog :: getTimeStamp()
{
    struct timeval now;
    gettimeofday(&now,NULL);
    return(now.tv_sec*1000000+now.tv_usec);
}
void TADialog ::onCellClicked(int r, int c)
{
    int sid,did,mid;
    char interface[100];
    QStringList labels;
    sid=DataTable->item(r,2)->text().toInt();
    did=DataTable->item(r,3)->text().toInt();
    mid=DataTable->item(r,4)->text().toInt();
    strcpy(interface,DataTable->item(r,1)->text().toAscii());
    //printf("\n%d %d %d",sid,did,mid);
    DetailView = new QTreeWidget();
    DetailView->setColumnCount(3);
    labels<<"SID="+DataTable->item(r,2)->text()<<"DID="+DataTable->item(r,3)->text()<<"MID="+DataTable->item(r,4)->text();
    DetailView->setHeaderLabels(labels);
    for(int count=0;count<pktflow_count;count++)
    {
         if((pktflow[count].sid==sid)&&
           (pktflow[count].did==did)&&
           (pktflow[count].mid==mid)&&
           strcmp(pktflow[count].interface,interface)==0)
        {
            for(int skt=0;skt<pktflow[count].no_flow;skt++)
             {
                //printf("%s->%s\n",pktflow[count].fsocket[skt].sip,pktflow[count].fsocket[skt].dip);
                QTreeWidgetItem *row=new QTreeWidgetItem(DetailView);
                row->setText(0,QString(pktflow[count].fsocket[skt].sip));
                row->setText(1,QString("------------------------->"));
                row->setText(2,QString(pktflow[count].fsocket[skt].dip));

            }
         }
    }

    DetailView->setColumnWidth(0,150);
    DetailView->setColumnWidth(1,150);
    DetailView->setColumnWidth(2,150);
    DetailView->setGeometry(0,0,400,400);
    DetailView->show();

}
void TADialog::timerEvent(QTimerEvent *event)
{
    int r=0,count=0;
    char time[100];
    struct timeval now;
    unsigned long pres_time;

 //DataTable->setRowCount(0);
    for(count=0;count<pktflow_count;count++)
    {printf("\nPacket flow count=%d",pktflow_count);
            gettimeofday(&now,NULL);
            pres_time=now.tv_sec*1000000+now.tv_usec;
            if(pres_time-pktflow[r].timestamp<=5000000)
            {
                    DataTable->setRowCount(r+1);
                    QTableWidgetItem *new_sid = new QTableWidgetItem(QString::number(pktflow[r].sid));
                    new_sid->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    QTableWidgetItem *new_did = new QTableWidgetItem(QString::number(pktflow[r].did));
                    new_did->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    QTableWidgetItem *new_mid = new QTableWidgetItem(QString::number(pktflow[r].mid));
                    new_mid->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    QTableWidgetItem *new_length = new QTableWidgetItem(QString::number(pktflow[r].size ));
                    new_length->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    intToStr(pktflow[r].timestamp,time);
                    QTableWidgetItem *new_timestamp = new QTableWidgetItem(QString(time));
                    new_timestamp->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    QTableWidgetItem *new_interface = new QTableWidgetItem(QString(pktflow[r].interface));
                    new_interface->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    QTableWidgetItem *new_pktrate = new QTableWidgetItem(QString::number(pktflow[r].packet_rate));
                    new_pktrate->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    QTableWidgetItem *new_datarate = new QTableWidgetItem(QString::number((double)(pktflow[r].data_rate*pktflow[r].packet_rate)/1000.0)+"KB/s");
                    new_datarate->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    DataTable->setItem(r,0,new_timestamp);
                    DataTable->setItem(r,1,new_interface);
                    DataTable->setItem(r,2,new_sid);
                    DataTable->setItem(r,3,new_did);
                    DataTable->setItem(r,4,new_mid);
                    DataTable->setItem(r,5,new_length);
                    DataTable->setItem(r,6,new_pktrate);
                    DataTable->setItem(r,7,new_datarate);
                    pktflow[r].packet_rate=0;
                    pktflow[r].data_rate=0;
                    r++;
              }

    }


}
