#ifndef GLOBALTYPES_H
#define GLOBALTYPES_H



//MessageProcessing
#define MAX_LOGICAL_ID	        			256
#define MAX_NODES                			256
#define MAX_FECS                                        256
#define MAX_REDUNDANT_LINKS     			4
#define SECRET_KEY_LENGTH				20
#define MAX_HASH_TABLE_LENGTH                           65536
#define TIMEOUT                                         1000000
#define SECURITY_REQUIRED                               1
#define HMAC_MIN_MSG_LENGTH				33
#define MIN_MESSAGE_LENGTH                              11
#define MAX_MESSAGE_LENGTH                              1024
#define MAX_MESSAGE_ENC_LENGTH                          1032
#define MAX_MESSAGE_IDS                                 255

#define HEADER_SIZE                                     10
#define MULTICAST_IP					"224.20.10.5"
#define MULTICAST_IP1					"225.20.10.5"
#define MULTICAST_IP2					"225.20.10.6"

//Database
#define QUERY_STRING_LENGTH				1024
#define MAX_GUI_MESSAGE_LENGTH                          MAX_REDUNDANT_LINKS*1024

#define WS_TABLE_FECID                                  0
#define WS_TABLE_FECDESCRIPTION                         1
#define WS_TABLE_NODEID                                 2
#define WS_TABLE_NODEDESCRIPTION                        3
#define WS_TABLE_IP                                     4
#define WS_TABLE_PORT                                   5
#define WS_TABLE_PASSWORD                               6


#define WS_MCTFEC_FECID                                 0
#define WS_MCTFEC_FECDESCRIPTION                        1
#define WS_MCTFEC_IP                                    2
#define WS_MCTFEC_PORT                                  3
#define WS_MCTFEC_LINK                                  4
#define WS_MCTFEC_PASSWORD                              5
#define WS_MCTFEC_P2MP                                  6

#define WS_MULTICAST_FECID                              0
#define WS_MULTICAST_MID                                1
#define WS_MULTICAST_DESCRIPTION                        2
#define WS_MULTICAST_NODEID                             3
#define WS_MULTICAST_P2MP                               4

#define MCTFEC_CONFIG_PARAMETER                         0
#define MCTFEC_CONFIG_VALUE                             1

#define WS_MESSAGE_PRIORITY_MSGID                       0
#define WS_MESSAGE_PRIORITY_PRIORITY                    1
#define WS_MESSAGE_PRIORITY_DESC                        2

//GUI
#define IPADDR                                     "IP :"
#define FECID                                      "FECID :"
#define NODEID                                     "NodeID :"
#define PORT                                       "Port :"
#define PASSWD                                     "Passwd :"
#define DESCRIPTION                                "Description :"
#define MILLISEC                                   " ms"

#define IPTRAFFIC                                  "IP Traffic"
#define ANY_INTERFACE                              "any"

#define TCP_PROTOCOL                               "tcp"
#define UDP_PROTOCOL                               "udp"
#define ICMP_PROTOCOL                              "icmp"
#define OTHER_PROTOCOL                             "other"

#define ERROR_LOGGER                               "Error"
#define EVENT_LOGGER                               "Event"
#define MESSAGE_LOGGER                             "Message"

#define LOGGER_IP                               "logger_ip"
#define LOGGER_PORT                             "logger_port"
#define UIM_IP                                  "uim_ip"
#define UIM_PORT                                "uim_port"
#define PROCESSING_PORT                         "processing_port"

#define MILLISECONDS                               1000
#define ANY_INTERFACE_SIZE                         3
#define MAX_NETWORK_INTERFACES                     3
#define WAN_PING_TIMEOUT                           600
#define LAN_PING_TIMEOUT                           50
#define LOGVIEWS_COLUMN_COUNT                      4
#define PI                                         3.14159265358979323846264338327950288419717
#define TWOPI                                      2.0 * PI
#define ARROWSIZE                                  6
#define LAN_VIEW_TIMER                             3000
#define WAN_VIEW_TIMER                             3000
#define MAX_LINES				   1500000
#define SCTP_RTO_MAX				   5


#pragma pack(1)

enum Modules
{
    POST,
    HAM,
    DHM,
    MPM,
    MLM,
    UIM
};

enum MsgType
{
    ERROR = 1,
    EVENT,
    MESSAGE,
    COMMAND,
    EXIT,
    STARTDEBUG,
    STOPDEBUG,
};

enum SocketModule
{
    SOCKET_POST,
    SOCKET_DHM,
    SOCKET_MPM,
    SOCKET_MLM,
    SOCKET_UIM
};

//GUI
class GuiNodePingStatus
{
    public :
       unsigned char  m_nodeid;
       unsigned char  m_linkcount;// pingchecker satus of node
       bool m_pingcheker[MAX_REDUNDANT_LINKS];
};
class FecNodePingStatus
{
    public :
           GuiNodePingStatus m_nodes[MAX_NODES];
};
class ApplicationStatus
{
    public :
        unsigned char       m_destinationId;
        unsigned char       m_sourceId;
        unsigned char       m_msgId;
        unsigned char       m_checksum;
        unsigned short      m_seqId;
        unsigned long       m_timeStamp;
        bool                m_hamstatus;
        char                m_command[128];
        char                m_localip[MAX_REDUNDANT_LINKS][16];
        int                 m_port;
};
class DatabaseChecksumStatus
{
    public :
        unsigned char       m_destinationId;
        unsigned char       m_sourceId;
        char                m_databasechecksum[128];
};
enum PingStatus
{
    PING_STATUS_FALSE ,
    PING_STATUS_TRUE
};
class Configuration
{
public:

    char                m_parameter[50];
    char                m_value[50];
};
enum NetworkViewProcessId
{
    LANVIEWID ,
    WANVIEWID ,
    POSTID
};
enum InterfaceId
{
    ETH0 ,
    ETH1,
    LO
};

enum ProtocolsId
{
    TCP,
    UDP,
    ICMP
};

//Database
class Socket
{
public:
    char                m_ipAddress[16];
    unsigned  int       m_port;
    unsigned char       m_broadcastFlag;
    unsigned char       m_linkType;
    char                m_secret[SECRET_KEY_LENGTH];
};

class NetworkInterface
{
public:
    char                m_ipAddress[MAX_REDUNDANT_LINKS][16];
    int                 m_count;
};

class NodeId
{
public:
    int		        m_nodeId;
    Socket              m_sock[MAX_REDUNDANT_LINKS];
    int 		m_availableLinks;
    int                 m_status;
    char                m_nodeDescription[20];
};

class NodeGroupId
{
public:
    int			m_gid;
    char                m_broadcast;
    int		        m_noOfNodes;
    int                 m_nodeIds[MAX_LOGICAL_ID];
    int                 m_p2mp[MAX_LOGICAL_ID];
};

class RemoteNodes
{
public:
    int        		m_nodeId;
    int		        m_fecId;
};

class FecId
{
public:
    int		        m_fecId;
    char                m_ips[MAX_REDUNDANT_LINKS][16];
    int                 m_port[MAX_REDUNDANT_LINKS];
    int			m_availableLinks;
    char                m_link[MAX_REDUNDANT_LINKS];
    char                m_fecDescription[20];
    char		m_passwd[20];
    int                 m_p2mp[MAX_REDUNDANT_LINKS];
};

//Message Processing
class FecHeader
{
public:
    unsigned char       m_destinationId;
    unsigned char       m_sourceId;
    unsigned char       m_msgId;
    unsigned char       m_checksum;
    unsigned short      m_seqId;
    unsigned long       m_timeStamp;
};

class MessagePriority
{
public:
    int                 m_msgID;
    unsigned int        m_priority;
};

class  QueueData
{
public:
    unsigned char       m_buff[1024];
    unsigned int        m_msgLen;
};

class HashHeader
{
public:
    FecHeader          *m_pfecHeader;
    int                 m_linkType;
    int                 m_count;
    unsigned long       m_timeStamp;
    HashHeader         *m_pnextHashHeader;
    HashHeader()
    {
        m_pfecHeader=NULL;
        m_pnextHashHeader=NULL;
    }
    ~HashHeader()
    {
      if(m_pfecHeader!=NULL)
      delete m_pfecHeader;
    }
};
//MessageLogger
class LoggerData {
public:
    char                m_startDate[25];
    enum Modules        m_module;
    enum MsgType        m_type;
    unsigned long       m_timestamp;
    char                m_descrption[100];
    char                m_remark[50];
    FecHeader           m_header;
    int                 m_linktype;
    int			m_messageLen;
};
struct FlowSocket
{
    char          interface[16];
    char          sip[16];
    char          dip[16];
    int           sport;
    int           dport;
    unsigned long timestamp;
    int           packet_rate;
    int           data_rate;
};

struct PacketFlow
{
    char          interface[16];
    bool          data_available;
    unsigned char sid;
    unsigned char did;
    unsigned char mid;
    FlowSocket    fsocket[256];
    int           no_flow;
    int           delay;
    unsigned long timestamp;
    int           packet_rate;
    int           data_rate;
    int           size;
};


#pragma pack()
#endif // GLOBALTYPES_H
