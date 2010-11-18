#ifndef DEF_CONSTRAINT
#define DEF_CONSTRAINT

    #include <vector>

    #include "../Debug.hpp"

    class Constraint
    {
        public:
            Constraint();
            ~Constraint();

            double get(int);
            void set(int, double);

        private:
            std::vector<double> a_data;
    };

#endif
