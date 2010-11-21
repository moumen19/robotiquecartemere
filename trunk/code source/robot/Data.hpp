#ifndef DEF_DATA
#define DEF_DATA

    #include <vector>
    #include <boost/any.hpp>

    #include "../Debug.hpp"

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

            boost::any get(int, DataOption::Place);
            boost::any get(int, int);
            void set(int, boost::any);

        private:
            std::vector< std::vector<boost::any> > a_data;
    };

#endif
