#ifndef DEF_BUSRS232
#define DEF_BUSRS232

    #include <string>
    #include <SerialStream.h>
    #include <boost/any.hpp>

    #include "../Debug.hpp"

    class BusRS232
    {
        public:
            BusRS232();
            ~BusRS232();

            bool open(std::string port = "/dev/ttyS0");
            void close();

            boost::any receive();
            void send(boost::any);

	protected:
	    virtual char * onSend(const boost::any &, int *);
	    virtual boost::any onReceive(const char *, int);

        private:
            std::string a_port;
            std::fstream a_file;
	    LibSerial::SerialStream a_rs232;
    };

#endif
