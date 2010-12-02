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

            bool open();	// configure automatiquement la connexion
            void close();

            boost::any getData();	// réception d'un char	//monChar = boost::any_cast<char>(monAny);
            void send(boost::any);	// envoi d'un string

	    virtual bool isDataAvailable();	// Si j'ai au moins un octet en attente de lecture

	protected:
	    virtual SerialPort::DataBuffer onSend(const boost::any &);
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
            
	    static pthread_mutex_t a_mutex;
    };

#endif
