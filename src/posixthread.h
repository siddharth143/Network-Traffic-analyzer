

#ifndef POSIXTHREAD_H
#define POSIXTHREAD_H

#include <pthread.h>
#include <sys/types.h>

enum ThreadState {
    THREAD_IDLE,
    THREAD_RUNNING,
    THREAD_DEAD,
    THREAD_GCR,
};

enum ThreadCancelType {
    TYPE_DEFER = PTHREAD_CANCEL_DEFERRED,
    TYPE_ASYNC = PTHREAD_CANCEL_ASYNCHRONOUS
             };

class PosixThread
{
public:

    //default constructor
    PosixThread();

    //virtual destructor
    virtual ~PosixThread()=0;

    //MEMBER FUNCTION DECLARATION :

    //Starts the main thread routine.
    bool  start();

    //Sets the thread state
    bool  setThreadState(ThreadState state);

    //gets the current state of the thread.
    int   getThreadState() const;

    //Terminates the running thread.
    void  terminate();

    //Exits the thread.
    void  exit();

protected:
    //MEMBER FUNCTION DECLARATION :

    //pure Virtual function
    virtual void  execute() = 0;

    //threadFunction is used with the posix-api for implementing this wrapper
    static  void* threadFunction(void *);

    //Disables the thread cancellation
    int     disableCancel();

    //Enables the thread cancellation
    int     enableCancel();

    //Sets the cancel type of the thread
    int     setCancelType( ThreadCancelType argType = TYPE_DEFER );

    //Starts the thread routine
    void    run();

    //detach the thread
    void    detach(void) const;

    //Relinquishes the processor voluntarily
    void    yield(void)  const;

    //returns the self id
    int     self()       const;

    //joins the calling thread to the thread routine
    int     join(void);

    //Sets the scheduling pririty for the thread
    bool    setPriority(int priority);

    //returns the priority level of the thread
    int     getPriority();

private:
    //OBJECT CREATION :

    //Thread Id
    pthread_t    m_threadId;

    //Thread State
    int         m_threadState;

};

#endif // POSIXTHREAD_H

