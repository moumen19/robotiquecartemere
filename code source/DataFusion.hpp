#ifndef DEF_DATAFUSION
#define DEF_DATAFUSION

    #include "Data.hpp"

    class DataFusion
    {
        public:
            DataFusion(Data &, Data &);
            ~DataFusion();

            void run();

        private:
            Data &a_sensorsData;
            Data &a_environmentData;
    };

#endif
