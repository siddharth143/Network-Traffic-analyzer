#include <cstdio>
#include <iostream>

#include "posixthread.h"

using namespace std;

/******************************************************************************
FUNCTION NAME           :PosixThread()

DESCREPTION		:Constructor

PARAMETERS		:void

RETURNS			:void
*******************************************************************************/
PosixThread::PosixThread():
             m_threadId(0),
             m_threadState(THREAD_IDLE)
{
};

/******************************************************************************
FUNCTION NAME           :~PosixThread()

DESCREPTION		: Destructor

PARAMETERS		: void

RETURNS			: void
*******************************************************************************/
PosixThread :: ~PosixThread()
{
};

/******************************************************************************
FUNCTION NAME           : run()

DESCREPTION		: Starts the thread routine

PARAMETERS		: void

RETURNS			: void
*******************************************************************************/
void PosixThread :: run()
{
    setThreadState(THREAD_RUNNING);
    execute();
    setThreadState(THREAD_DEAD);
};

/******************************************************************************
FUNCTION NAME           : threadFunction()

DESCREPTION		: Static method used with the posix-api for implementing this wrapper

PARAMETERS		: void*

RETURNS			: void*
*******************************************************************************/
void* PosixThread :: threadFunction(void* argPtr)
{
    PosixThread* thrd = reinterpret_cast<PosixThread*>(argPtr);

    if (thrd) {
        thrd->run();
    }
    return NULL;
};

/******************************************************************************
FUNCTION NAME           : start()

DESCREPTION		: Starts the main thread routine.

PARAMETERS		: bool

RETURNS			: void
*******************************************************************************/
bool PosixThread::start()
{
    int result = pthread_create( &m_threadId, NULL,
                                 &(PosixThread::threadFunction), this );

    if ( result == 0 ) {
        return true;
    } else {
        setThreadState(THREAD_DEAD);
        cout<< "PosixThread::start() - Failed to create thread. "
                << m_threadId <<endl;
        return false;
    }
};

/******************************************************************************
FUNCTION NAME           : self()

DESCREPTION		: it returns the self id

PARAMETERS		: int

RETURNS			: void
*******************************************************************************/
int PosixThread ::self() const
{
    return (int) m_threadId;
};

/******************************************************************************
FUNCTION NAME           : detach()

DESCREPTION		: detach the thread

PARAMETERS		: int

RETURNS			: void
*******************************************************************************/
void PosixThread ::detach(void) const
{
    pthread_t tempId = m_threadId;

    if ( pthread_detach(m_threadId) != 0 ) {
        cout<< "PosixThread::detach() - Failed. "<< tempId <<endl;
    }
    tempId = 0;
};

/******************************************************************************
FUNCTION NAME           : join()

DESCREPTION		: joins the calling thread to the thread routine,generally
                          called my the main thread for graceful termination of running child threads if any

PARAMETERS		: int

RETURNS			: void
*******************************************************************************/
int PosixThread ::join(void)
{
    int Status = 0;

    if ( m_threadId > 0 ) {
        Status = pthread_join(m_threadId, (void**)0 );

        if (  Status != 0 ) {
            cout<< "PosixThread::join() - Failed to join thread. "
                    << m_threadId <<endl;
        }
    } else {
        Status = -1; /* Temporary error-code */
        cout<< "PosixThread::join() - Thread is not running \n";
    }
    return Status;
};

/******************************************************************************
FUNCTION NAME           : setPriority()

DESCREPTION		: Sets the scheduling pririty for the thread.

PARAMETERS		: int

RETURNS			: bool
*******************************************************************************/
bool PosixThread ::setPriority( int priority)
{
    if ( (priority> 10) || (priority< 0)) {
        return false;
    }

    struct sched_param threadParam;
    threadParam.sched_priority = priority;

    int result =  pthread_setschedparam( m_threadId, SCHED_OTHER, &threadParam );

    if ( result != 0 ) {
        cout<< "PosixThread::setPriority() - Failed to set thread priority. "
                << m_threadId <<endl;
        return false;
    } else {
        return true;
    }

};

/******************************************************************************
FUNCTION NAME           : getPriority()

DESCREPTION		: returns the priority level of the thread

PARAMETERS		: void

RETURNS			: priority level
*******************************************************************************/
int PosixThread ::getPriority()
{
    int    policy = 0;
    struct sched_param threadParameter;

    int result = pthread_getschedparam(m_threadId, &policy, &threadParameter);

    if ( result == 0) {
        cout<< "PosixThread::getPriority() - Failed to get priority. "
                <<  m_threadId <<endl;
        return -1;
    } else {
        return threadParameter.sched_priority;
    }

};

/******************************************************************************
FUNCTION NAME           : yield()

DESCREPTION		: Relinquishes the processor voluntarily.

PARAMETERS		: void

RETURNS			: void
*******************************************************************************/
void  PosixThread ::yield(void) const
{
    sched_yield();
};

/******************************************************************************
FUNCTION NAME           : terminate()

DESCREPTION		: Terminates the running thread.

PARAMETERS		: void

RETURNS			: void
*******************************************************************************/
void PosixThread ::terminate()
{
    int result = pthread_cancel( m_threadId );

    if (  result != 0) {
        cout<< "PosixThread::terminate() - Failed to cancel thread. "
                << m_threadId <<endl;
    } else {
#ifdef DEBUG
        cout<< "PosixThread::terminate() - Cancelled thread. "
                << m_threadId <<endl;
#endif // DEBUG
    }
};

/******************************************************************************
FUNCTION NAME           : exit()

DESCREPTION		: Exits the thread.

PARAMETERS		: void

RETURNS			: void
*******************************************************************************/
void PosixThread ::exit()
{
    pthread_exit((void**)0);
};

/******************************************************************************
FUNCTION NAME           : setThreadState()

DESCREPTION		: Sets the thread state.

PARAMETERS		: argState : Thread state

RETURNS			: true by default
*******************************************************************************/
bool PosixThread::setThreadState(ThreadState argState  )
{
    m_threadState= argState;
    return true;
};


/******************************************************************************
FUNCTION NAME           : getThreadState()

DESCREPTION		: returns the current state of the thread.

PARAMETERS		: void

RETURNS			: thread state
*******************************************************************************/
int PosixThread ::getThreadState() const
{
    int state = m_threadState;
    return state;
};


/******************************************************************************
FUNCTION NAME           : enableCancel()

DESCREPTION		: Enables the thread cancellation.

PARAMETERS		: void

RETURNS			: 0 on Success, else posix error-code
*******************************************************************************/
int PosixThread ::enableCancel()
{
    int result = pthread_setcancelstate( PTHREAD_CANCEL_ENABLE, NULL );

    if ( result != 0 ) {
        cout<< "PosixThread::enableCancel() - Failed" << m_threadState<<endl;
        return (result);
    }
    pthread_testcancel();
    return (0);
};


/******************************************************************************
FUNCTION NAME           : disableCancel()

DESCREPTION		: Disables the thread cancellation.

PARAMETERS		: void

RETURNS			: 0 on Success, else posix error-code
*******************************************************************************/
int PosixThread ::disableCancel()
{
    pthread_testcancel();
    int result = pthread_setcancelstate( PTHREAD_CANCEL_DISABLE, NULL );

    if ( result != 0 ) {
        cout<< "PosixThread::disableCancel() - Failed" << m_threadState<<endl;
    }

    return (result);
};

/******************************************************************************
FUNCTION NAME           : setCancelType()

DESCREPTION		: Sets the cancel type of the thread.

PARAMETERS		: argType : Type of cancel

RETURNS			: 0 on Success, else posix error-code
*******************************************************************************/
int PosixThread ::setCancelType( ThreadCancelType argType )
{
    int result = pthread_setcanceltype( argType, NULL );

    if ( result != 0 ) {
        printf( "PosixThread::setCancelType() - Failed. %d \n", m_threadState);
    } else {;}

    return (result);
};

