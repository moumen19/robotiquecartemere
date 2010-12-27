    #ifndef WORKER_HPP_INCLUDED
#define WORKER_HPP_INCLUDED

#include <iostream>
#include <exception>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/date_time.hpp>

class Worker
{
public:
    Worker(boost::mutex *ptrToRessMutex)
    {
        m_mutex=ptrToRessMutex;

        // the thread is not-a-thread until we call start()
    }

    ~Worker()
    {
        m_mutex=NULL;
    }

    void start(int threadId)
    {
        //static boost::mutex m_mutex;
        m_Thread = boost::thread(&Worker::workerMethod, this, threadId);

        try{
//            std::cout<<"etat du mutex: "<<m_mutex.try_lock()<<std::endl;
//
//            std::cout<<"etat du mutex: "<<m_mutex.try_lock()<<std::endl;
           // m_Thread.interrupt();

        }
        catch(std::exception e)
        {
            std::cout<< e.what();
        }

    }

    void join(){ m_Thread.join(); }

    void workerMethod(int threadId)
    {
        std::cout<<"("<<threadId<<")wantsToWork"<<std::endl;

        // equivalets:

        //boost::lock_guard<boost::mutex> lock(*m_mutex);
        boost::mutex::scoped_lock sl(*m_mutex);

        std::cout<<"("<<threadId<<")StartsToWork"<<std::endl;

//
//
//        // useless
//        std::cout<<threadId<<"=Ok, mutex unlocked (Id réel=";
//

        // work
        boost::posix_time::milliseconds workTime(30);
        for(int i=0;i<30;i++)
        {
            for(int j=0;j<threadId;j++)
                std::cout<<"\t";

            std::cout << "Worker"<<threadId<<": " <<i<< std::endl;
            boost::this_thread::sleep(workTime);
        }

        // release
//        try{
//            m_mutex->unlock();
//        }
//        catch(std::exception e)
//        {
//            std::cout<< e.what();
//        }
    }

private:
    boost::mutex * m_mutex;

    boost::thread m_Thread;
};

// STATIC: must exist outside the class
//boost::mutex Worker::m_mutex;

#endif // WORKER_HPP_INCLUDED
