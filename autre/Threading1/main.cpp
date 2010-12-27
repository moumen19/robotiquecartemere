
#include <boost/thread/mutex.hpp>
#include "Worker.hpp"


int main(int argc, char* argv[])
{
    std::cout << "main: startup" << std::endl;


    boost::mutex RessMutex1;

    Worker worker1(&RessMutex1),
            worker2(&RessMutex1),
            worker3(&RessMutex1);

    worker3.start(3);
    worker1.start(1);
    worker2.start(2);

    worker3.join();
    worker2.join();
    worker1.join();

    std::cout << "main: done" << std::endl;

    return 0;
}

// //http://antonym.org/2009/05/threading-with-boost---part-i-creating-threads.html
//#include <iostream>
//#include <boost/thread/thread.hpp>
//#include <boost/thread/mutex.hpp>
//#include <boost/date_time.hpp>
//
//void workerFunc(int threadId, boost::mutex &matmut)
//{
//   //matmut
//    boost::posix_time::milliseconds workTime(30);
//
//    for(int i=0;i<30;i++)
//    {
//        for(int j=0;j<threadId;j++)
//            std::cout<<"\t";
//
//        std::cout << "Worker"<<threadId<<": " <<i<< std::endl;
//        boost::this_thread::sleep(workTime);
//    }
//
//}
//
//int main(int argc, char* argv[])
//{
//    boost::posix_time::ptime T1;
//    T1 = boost::posix_time::microsec_clock::local_time();
//
//
//
//    boost::mutex MUT;
//    boost::thread workerThread1(workerFunc,1,MUT);
//    boost::thread workerThread2(workerFunc,2,MUT);
//    boost::thread workerThread3(workerFunc,3,MUT);
//
//
//    boost::posix_time::ptime T2;
//    T2 = boost::posix_time::microsec_clock::local_time();
//
//    boost::posix_time::time_period Tp(T1, T2);
//    std::cout << "elapsed time: "<<boost::posix_time::to_simple_string(Tp.length()) << std::endl;
//
//    return 0;
//}
