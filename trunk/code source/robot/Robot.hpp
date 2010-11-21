/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier Robot.hpp
 *
 */

#ifndef DEF_ROBOT
#define DEF_ROBOT

    #include <pthread.h>

    #include "../Debug.hpp"

    #include "Data.hpp"
    #include "Planning.hpp"
    #include "Constraint.hpp"
    #include "Strategy.hpp"
    #include "DataFusion.hpp"
    #include "Communication.hpp"

    class Robot
    {
        public:
            Robot();
            ~Robot();

            void interruptuion_arret_urgence();

            bool isCommunicationActive();
            void startCommunication();
            void stopCommunication();

            // Gestion du thread
            bool isActive();
            void start();
            void stop();
            void wait();

        protected:
            static void * run(void *);

        private:
            Data *a_sensorsData;
            Data *a_environmentData;
            Planning *a_planning;
            Constraint *a_constraint;
            Strategy *a_strategy;
            DataFusion *a_dataFusion;
            Communication *a_communication;

            pthread_t a_thread;
            bool a_thread_active;
    };

#endif
