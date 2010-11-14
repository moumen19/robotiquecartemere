#ifndef DEF_STRATEGY
#define DEF_STRATEGY

    #include "Constraint.hpp"
    #include "Data.hpp"

    enum StrategyName
    {
        EMERGENCY_STOP,
        RUN
    };

    class Strategy
    {
        public:
            Strategy(Data &, Constraint &);
            ~Strategy();

            StrategyName get();
            void set(StrategyName);

        private:
            Constraint &a_constraint;
            Data &a_environmentData;
    };

#endif
