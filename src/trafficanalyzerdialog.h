#ifndef TRAFFICANALYZERDIALOG_H
#define TRAFFICANALYZERDIALOG_H
#include "mainwindow.h"
#include "posixthread.h"
#include "globaltypes.h"
#include <qdialog.h>
#include <qthread.h>
#include <qtablewidget.h>
#include <qtreewidget.h>
class TrafficAnalyzer;



class TADialog : public MainWindow, public PosixThread
{
    Q_OBJECT
public slots:
    void onCellClicked(int,int);
public:
    TADialog(QWidget *parent = 0);
    ~TADialog();
    void execute();
     void timerEvent(QTimerEvent *event);
    void getTime(char *ptime_string);
    void intToStr(unsigned long ltim, char *pstim);
    unsigned long getTimeStamp();

    TrafficAnalyzer *ta_eth0,*ta_eth1;
    int length,i;
    char m_pTime[1024];
    PacketFlow *pktflow;
    int pktflow_count;
    QTableWidget *DataTable;
    QTreeWidget  *DetailView;
};

#endif // TRAFFICANALYZERDIALOG_H
