//
// blocking_tcp_echo_client.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include <map>

using boost::asio::ip::tcp;

enum { max_length = 1024 };

/* Delsys EMG Tcp Ports */
const std::string PORT_COMM("50040");
const std::string PORT_DATA("50041");


/* Function Prototypes */
int GetKeyboardCommand(std::map<int, std::string> commands);



int main(int argc, char* argv[])
{
  // try
  // {
    if (argc != 2)
    {
      std::cerr << "Usage: tcp_client_emg <host>\n";
      return 1;
    }

    /* Commands */
    std::map<int,std::string> COMMANDS;
    COMMANDS[1] = "START\r\n\r\n";
    COMMANDS[2] = "STOP\r\n\r\n";


    boost::asio::io_context io_context;

    tcp::socket s(io_context);
    tcp::resolver resolver(io_context);
    boost::asio::connect(s, resolver.resolve(argv[1], PORT_DATA.c_str()));

    int key;
    while (true){
	key = GetKeyboardCommand(COMMANDS);
        boost::asio::write(s, boost::asio::buffer(COMMANDS[key].c_str(), strlen(COMMANDS[key].c_str())));
    }

    // char reply[max_length];
    // size_t reply_length = boost::asio::read(s,
    // boost::asio::buffer(reply, request_length));
    // std::cout << "Reply is: ";
    // std::cout.write(reply, reply_length);
    // std::cout << "\n";
  // }
  // catch (std::exception& e)
  // {
  //   std::cerr << "Exception: " << e.what() << "\n";
  // }

  return 0;
}


int GetKeyboardCommand(std::map<int, std::string> commands){
    char keyboard_input[256] = "";
    bool validInput = false;
    int keyboard_val;
    while (!validInput){
        printf("Input Command Number\n");
        printf("\t1\t- START\n");
        printf("\t2\t- STOP\n");
        // reset string to null
        memset(keyboard_input, 0, sizeof(keyboard_input));
        // get keyboard input
        fgets(keyboard_input, 256, stdin);
        // convert to integer
        keyboard_val = atoi(keyboard_input);

	if (commands[keyboard_val] != ""){
	    return keyboard_val;
	}
    }
}
