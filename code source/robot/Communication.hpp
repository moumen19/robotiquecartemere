#ifndef DEF_COMMUNICATION
#define DEF_COMMUNICATION

    #include <string>

    #include "../Debug.hpp"

    #include "Data.hpp"
    #include "Planning.hpp"
    #include "BusRS232.hpp"
    #include "BusI2C.hpp"

    namespace Port
    {
        enum Port
        {
            RS232,
            I2C
        };
    }

    class Communication
    {
        public:
            Communication(Data *, Data *, Constraint *, Planning *);
            ~Communication();

            void send(Port::Port, std::string);
            void receive();

        private:
            BusRS232 a_rs232;
            BusI2C a_i2c;
            Data *a_sensorsData;
            Data *a_environmentData;
            Constraint *a_constraint;
            Planning *a_planning;
    };

#endif