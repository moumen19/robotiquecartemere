#include <SerialStream.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>


using namespace LibSerial;

bool createCOMport(SerialStream &serial_port, std::string COMport_name, SerialStreamBuf::BaudRateEnum baudRate)
{
        serial_port.Open(COMport_name);
        if(!serial_port.IsOpen() || !serial_port.good())
        {
            std::cerr << "Pb port IsOpen";
            return false;
        }

        //
        // Set the baud rate of the serial port.
        //
        serial_port.SetBaudRate(baudRate) ;
        if ( ! serial_port.good() )
        {
            std::cerr << "Error: Could not set the baud rate." << std::endl ;
            return false;
        }
        //
        // Set the number of data bits.
        //
        serial_port.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
        if ( ! serial_port.good() )
        {
            std::cerr << "Error: Could not set the character size." << std::endl ;
            return false;
        }
        //
        // Disable parity.
        //
        serial_port.SetParity( SerialStreamBuf::PARITY_NONE ) ;
        if ( ! serial_port.good() )
        {
            std::cerr << "Error: Could not disable the parity." << std::endl ;
            return false;
        }
        //
        // Set the number of stop bits.
        //
        serial_port.SetNumOfStopBits( 1 ) ;
        if ( ! serial_port.good() )
        {
            std::cerr << "Error: Could not set the number of stop bits."<< std::endl ;
            return false;
        }
        //
        // Turn off hardware flow control.
        //
        serial_port.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE ) ; //FLOW_CONTROL_HARD
        if ( ! serial_port.good() )
        {
            std::cerr << "Error: Could not use hardware flow control."<< std::endl ;
            return false;
        }

        //my_serial_port.SetVMin(10);
        std::cout <<"current size of character buffer : " <<serial_port.VMin () <<std::endl;
        std::cout <<"current timing of character buffer in 10th of a second : " <<serial_port.VTime () <<std::endl;

        return true;
}


/*void testCOM()
{
    SerialStream serial_port ;

    //std::cout<< "isOpen ? : " << serial_port.IsOpen() <<std::endl;

    serial_port.Open( "/dev/ttyUSB0") ;

    if ( !serial_port.good() )
    {
        std::cerr << "[" << __FILE__ << " \\Line:" << __LINE__ << "]\n "
                  << "Error: Could not open serial port."
                  << std::endl ;
        exit(1) ;
    }
    //
    // Set the baud rate of the serial port.
    //
    serial_port.SetBaudRate( SerialStreamBuf::BAUD_9600 ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not set the baud rate." << std::endl ;
        exit(1) ;
    }
    //
    // Set the number of data bits.
    //
    serial_port.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not set the character size." << std::endl ;
        exit(1) ;
    }
    //
    // Disable parity.
    //
    serial_port.SetParity( SerialStreamBuf::PARITY_NONE ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not disable the parity." << std::endl ;
        exit(1) ;
    }
    //
    // Set the number of stop bits.
    //
    serial_port.SetNumOfStopBits( 1 ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not set the number of stop bits."
                  << std::endl ;
        exit(1) ;
    }
    //
    // Turn off hardware flow control.
    //
    serial_port.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE ) ; //FLOW_CONTROL_HARD // FLOW_CONTROL_NONE
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not use hardware flow control."
                  << std::endl ;
        exit(1) ;
    }

    char next_char ='e';
    while(getchar() != '\n')
    {
        sleep(1);
       // if(serial_port.rdbuf()->in_avail()) // && serial_port.good())
        {
            //if (my_serial_stream.available() > 0)

            serial_port >> next_char ;
            std::cout << next_char <<std::endl;
        }
        else
            std::cout<<".";
    }

    serial_port.Close();
}*/
