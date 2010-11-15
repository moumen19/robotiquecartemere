#ifndef DEF_STRATEGY
#define DEF_STRATEGY

    #include "Constraint.hpp"
    #include "Data.hpp"

    class Strategy
    {
        public:
            Strategy(Data *, Constraint *);
            ~Strategy();

            int get();
            void set(int);

        private:
            Constraint *a_constraint;
            Data *a_environmentData;
            int a_strategy;
    };

#endif
