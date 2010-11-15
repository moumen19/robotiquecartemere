#ifndef DEF_ROBOT
#define DEF_ROBOT

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

            void init();
            void interruptuion_arret_urgence();

            void run();

        private:
            Data *a_sensorsData;
            Data *a_environmentData;
            Planning *a_planning;
            Constraint *a_constraint;
            Strategy *a_strategy;
            DataFusion *a_dataFusion;
            Communication *a_communication;
    };

#endif
