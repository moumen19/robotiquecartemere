#ifndef DEF_BUSRS232
#define DEF_BUSRS232

    #include <string>
    //#include <SerialStream.h>
    #include <SerialPort.h>
    #include <boost/any.hpp>
    #include <pthread.h>

    #include "../Debug.hpp"

    class BusRS232
    {
        public:
            BusRS232(std::string port = "/dev/ttyUSB1", int bufferSize = 1024);
            ~BusRS232();

            bool open();
            void close();

            boost::any getData();
            void send(boost::any);

	    virtual bool isDataAvailable();

	protected:
	    virtual char * onSend(const boost::any &, int *);
	    virtual boost::any onReceive();
            static void * receive(void *);

        private:
            std::string a_port;
            //std::fstream a_file;
	    SerialPort * a_rs232; //LibSerial::SerialStream
            pthread_t a_thread;
            bool a_thread_active;
	    char * a_buffer;
	    int a_bufferSize;
	    int a_bufferWriteCursor;
	    int a_bufferReadCursor;
    };

#endif
