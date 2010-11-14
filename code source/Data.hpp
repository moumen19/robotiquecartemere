#ifndef DEF_DATA
#define DEF_DATA

    #include <vector>

    namespace DataOption
    {
        enum DataName
        {
            IR_10,
            IR_20,
            IR_30,
            IR_40
        };

        enum Place
        {
            LAST,
            FIRST
        };
    }

    class Data
    {
        public:
            Data();
            ~Data();

            double get(DataOption::DataName, DataOption::Place);
            double get(DataOption::DataName, int);
            void set(DataOption::DataName, double);

        private:
            std::vector< std::vector<double> > a_data;
            std::vector<int> a_bufferSize;
    };

#endif
