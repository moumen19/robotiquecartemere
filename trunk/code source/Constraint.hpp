#ifndef DEF_CONSTRAINT
#define DEF_CONSTRAINT

    #include <vector>

    namespace ConstraintRule
    {
        enum ConstraintRule
        {
            SPEED = 1,
        };
    }

    class Constraint
    {
        public:
            Constraint();
            ~Constraint();

            double get(ConstraintRule::ConstraintRule);
            void set(ConstraintRule::ConstraintRule, double);

        private:
            std::vector<double> a_data;
    };

#endif
