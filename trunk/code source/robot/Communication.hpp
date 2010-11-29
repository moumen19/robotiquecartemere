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
    #include <pthread.h>
    #include <SerialStream.h>
    //#include <boost/thread.hpp>

    #include "../Debug.hpp"

    #include "Data.hpp"
    #include "Planning.hpp"
    //#include "BusRS232.hpp"
    //#include "BusI2C.hpp"

    #define _BUFFER_ASSERVISSEMENT_SIZE 13
    #define _BUFFER_SENSOR_SIZE 5

    namespace Port
    {
        enum Port
        {
            ASSERVISSEMENT,
            SENSOR
        };
    }

    class Communication
    {
        public:
            Communication(Data *, Data *, Constraint *, Planning *);
            ~Communication();

            void send(Port::Port, std::string);
            //void receive();

            // Gestion du thread
            bool isActive();
            void start();
            void stop();

        protected:
            static void * run(void *);

        private:
            //BusRS232 *a_rs232;
            //BusI2C *a_i2c;
            LibSerial::SerialStream a_rs232Asservissement;
            LibSerial::SerialStream a_rs232Sensor;
            char a_bufferAsservissement[_BUFFER_ASSERVISSEMENT_SIZE];
            char a_bufferSensor[_BUFFER_SENSOR_SIZE];
            int a_bufferAsservissementCursor;
            int a_bufferSensorCursor;

            Data *a_sensorsData;
            Data *a_environmentData;
            Constraint *a_constraint;
            Planning *a_planning;

            //boost::thread *a_thread;
            pthread_t a_thread;
            bool a_thread_active;
    };

#endif
