/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier Communication.hpp
 *
 */

#ifndef DEF_COMMUNICATION
#define DEF_COMMUNICATION

    #include <string>
//    #include <pthread.h>
#include <boost/thread.hpp>

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

            // Gestion du thread
            bool isActive();
            void start();
            void stop();

        protected:
            static void * run(void *);

        private:
            BusRS232 *a_rs232;
            BusI2C *a_i2c;
            Data *a_sensorsData;
            Data *a_environmentData;
            Constraint *a_constraint;
            Planning *a_planning;

            boost::thread *a_thread; //pthread_t
            bool a_thread_active;
    };

#endif
