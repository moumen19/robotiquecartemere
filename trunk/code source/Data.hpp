#ifndef DEF_DATA
#define DEF_DATA

    #include <vector>

    namespace DataOption
    {
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

            double get(int, DataOption::Place);
            double get(int, int);
            void set(int, double);

        private:
            std::vector< std::vector<double> > a_data;
            std::vector<int> a_bufferSize;
    };

#endif
