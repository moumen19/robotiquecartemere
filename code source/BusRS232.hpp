#ifndef DEF_BUSRS232
#define DEF_BUSRS232

    #include <string>

    class BusRS232
    {
        public:
            BusRS232();
            BusRS232(std::string);
            ~BusRS232();

            bool open(std::string);
            void close();

            std::string receive();
            void send(std::string);

        private:
            int a_bus;
    };

#endif
