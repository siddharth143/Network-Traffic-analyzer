
//SYSTEM DEFINED HEADER FILES
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <iostream>


//USER DEFINED HEADER FILES
#include "pktbuffer.h"

using namespace std;

/******************************************************************************
FUNCTION NAME           :CircularPktBuffer()

DESCREPTION		:Constructor

PARAMETERS		:void

RETURNS			:void
*******************************************************************************/
CircularPktBuffer :: CircularPktBuffer():
        m_size(0),
        m_rear(-1),
        m_front(-1),
        m_priority(0),
//	m_mutexEntry(m_mutexEntry),
        m_pqueue(NULL)//,
//	m_condVariable(m_condVariable)
{
    m_pqueue = new QueueData[1024];
    pthread_mutex_init( &m_mutexEntry, NULL );
    pthread_cond_init( &m_condVariable, NULL );
}


/******************************************************************************
FUNCTION NAME           :	CircularPktBuffer()

DESCREPTION	        :       Constructor

PARAMETERS	        :       buffSize : size of the buffer


RETURNS	                :       void
*******************************************************************************/
CircularPktBuffer :: CircularPktBuffer(unsigned int buffSize):
        m_size(buffSize),
        m_rear(-1),
        m_front(-1),
        m_priority(0),
        //m_mutexEntry(m_mutexEntry),
        m_pqueue(NULL)//,
        //m_condVariable(m_condVariable)
{

    m_pqueue = new QueueData[buffSize];
    pthread_mutex_init( &m_mutexEntry, NULL );
    pthread_cond_init( &m_condVariable, NULL );
}

/******************************************************************************
FUNCTION NAME           :	CircularPktBuffer()

DESCREPTION	        :       Constructor

PARAMETERS	        :       buffSize : size of the buffer
                                priority : priority


RETURNS	                :       void
*******************************************************************************/
CircularPktBuffer :: CircularPktBuffer(unsigned int buffSize,int priority):
        m_size(buffSize),
        m_rear(-1),
        m_front(-1),
        m_priority(priority),
        //m_mutexEntry(m_mutexEntry),
        m_pqueue(NULL)//,
        //m_condVariable(m_condVariable)
{
    m_pqueue = new QueueData[buffSize];
    pthread_mutex_init( &m_mutexEntry, NULL);
    pthread_cond_init( &m_condVariable, NULL);
}

/******************************************************************************
FUNCTION NAME           :       CircularPktBuffer()

DESCREPTION             :       Copy Constructor

PARAMETERS              :       pktbuff: reference of CircularPktBuffer


RETURNS                 :       void
*******************************************************************************/

CircularPktBuffer :: CircularPktBuffer(const CircularPktBuffer &pktbuff):
        m_size(0),
        m_rear(-1),
        m_front(-1),
        m_priority(0),
        m_mutexEntry(pktbuff.m_mutexEntry),
        m_pqueue(pktbuff.m_pqueue),
        m_condVariable(pktbuff.m_condVariable)
{
}

/******************************************************************************
FUNCTION NAME           :operator=()

DESCREPTION             :assigment operator overloading

PARAMETERS              :pktbuff: reference to CircularPktBuffer

RETURNS                 :void
 *******************************************************************************/
CircularPktBuffer &CircularPktBuffer::operator=(const CircularPktBuffer& pktbuff)
{
#if DEBUG
    cout<< "CircularPktBuffer copy assignment operator" << endl;
#endif
return *this;
}

/******************************************************************************
FUNCTION NAME           :~CircularPktBuffer()

DESCREPTION		: Destructor

PARAMETERS		: void

RETURNS			: void
*******************************************************************************/
CircularPktBuffer :: ~CircularPktBuffer()
{
    pthread_mutex_lock( &m_mutexEntry );
    delete [] m_pqueue;

    pthread_mutex_unlock( &m_mutexEntry );

    pthread_mutex_destroy( &m_mutexEntry );

    pthread_cond_destroy( &m_condVariable );
}

/******************************************************************************
FUNCTION NAME           :	putMessage()

DESCREPTION	        :       puts the messages in the buffer

PARAMETERS	        :       pBuf : Message Buffer
                                len  : size of message


RETURNS	                :       void
*******************************************************************************/
void CircularPktBuffer ::putMessage(unsigned char *pBuf, int len)
{

    pthread_mutex_lock( &m_mutexEntry );

    m_rear += 1;
    m_rear = m_rear%m_size;
    m_pqueue[m_rear].m_msgLen = len;

    memcpy( &m_pqueue[m_rear].m_buff,  pBuf, m_pqueue[m_rear].m_msgLen );

    if ( m_front == m_rear )
    {
        m_front = (m_rear + 1)%m_size;
    }
    else if ( m_front == -1 )
    {
        m_front = 0;
    }
    //pthread_cond_signal(&m_condVariable);//Not required
    pthread_mutex_unlock( &m_mutexEntry );

}

/******************************************************************************
FUNCTION NAME           :	getMessage()

DESCREPTION	        :       gets  the messages from the buffer

PARAMETERS	        :       pBuf : it contains Message Buffer


RETURNS	                :       size of the message
*******************************************************************************/
int CircularPktBuffer :: getMessage( char* pBuf )
{
    int msgLen = 0;
    pthread_mutex_lock( &m_mutexEntry );
    if ( m_front < 0 )
    {
        pthread_mutex_unlock( &m_mutexEntry );
        return 0;
    }
    memcpy(pBuf, &m_pqueue[m_front], sizeof(QueueData)-4); //copying only message data ,leaving message len
    msgLen = m_pqueue[m_front].m_msgLen;
    if ( m_front == m_rear )
    {
        m_front = -1;
        m_rear = -1;
    }
    else
    {
        m_front += 1;
        m_front = m_front%m_size;
    }

    pthread_mutex_unlock( &m_mutexEntry );
    return msgLen;
}

/******************************************************************************
FUNCTION NAME           :	isEmpty()

DESCREPTION	        :       checks whether the buffer is empty or not

PARAMETERS	        :       void


RETURNS	                :       true : if the buffer is empty
                                false : if the buffer is not empty
*******************************************************************************/
bool CircularPktBuffer :: isEmpty()
{
    bool flag = false;
    if ( m_front < 0 )
    {
        flag = true;
    }
    return flag;
}

