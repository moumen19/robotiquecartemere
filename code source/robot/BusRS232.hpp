#ifndef DEF_BUSRS232
#define DEF_BUSRS232

    #include <string>
    #include <fstream>

    #include "../Debug.hpp"

    class BusRS232
    {
        public:
            BusRS232();
            ~BusRS232();

            bool open(std::string port = "/dev/ttyS0");
            void close();

            std::string receive();
            void send(std::string);

        private:
            std::string a_port;
            bool a_isOpen;
            std::fstream a_file;
    };

#endif
