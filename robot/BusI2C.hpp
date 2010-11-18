#ifndef DEF_BUSI2C
#define DEF_BUSI2C

    #include <string>

    #include "../Debug.hpp"

    class BusI2C
    {
        public:
            BusI2C();
            ~BusI2C();

            bool open(std::string);
            void close();

            int getSlave();
            void setSlave(int);

            std::string receive();
            void send(std::string);

        private:
            int a_bus;
            int a_slave;
    };

#endif
