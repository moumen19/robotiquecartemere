#ifndef DEF_DATA
#define DEF_DATA

    #include <vector>

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

            void* get(int, DataOption::Place);
            void* get(int, int);
            void set(int, void*);

        private:
            std::vector< std::vector<void*> > a_data;
            std::vector<int> a_bufferSize;
    };

#endif
