#ifndef DEF_DEBUG
#define DEF_DEBUG

    #define _DEBUG_MODE 1

    #include <iostream>
    #include <vector>

    typedef int Debug_Configuration;
    #define CONSOLE_DISPLAY 1
    #define FILE_DISPLAY 2

    enum Debug_Priority
    {
        FATAL,
        WARNING,
        INFORMATION
    };

    struct Message
    {
        std::string title;
        std::string content;
        Debug_Priority priority;
        bool isEmpty;
    } typedef Message;

    class _DEBUG
    {
        public:
            static void addMessage(std::string, std::string, Debug_Priority);
            static void clearAllMessage();
            static void configuration(Debug_Configuration, std::string);
            static Message getMessage(unsigned int);
            static Message getNextMessage();
            static int getNumMessages();

        private:
            static std::vector<Message> a_messages;
            static unsigned int a_cursor;
            static Debug_Configuration a_configuration;
            static std::string a_filename;
    };

#endif
