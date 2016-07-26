//
//  main.cpp
//  xterm256
//
//  Created by BlueCocoa on 2016/7/26.
//  Copyright © 2016 BlueCocoa. All rights reserved.
//

#include <iostream>
#include "xterm256.hpp"

using namespace std;

/**
 *  @brief  xterm256 instance
 */
xterm256 xterm;

/**
 *  @brief  Highlight definitions
 */
xterm256::color function_c(0xa6, 0xe2, 0x2e);
xterm256::color text(0xf8, 0xf8, 0xf2);
xterm256::color ns(0xf8, 0xf8, 0xf2);
xterm256::color keyword(0x66, 0xd9, 0xef);
xterm256::color op(0xf9, 0x26, 0x72);
xterm256::color number(0xae, 0x81, 0xff);
xterm256::color reason(0xa6, 0xe2, 0x2e);
xterm256::color attribute(0xa6, 0xe2, 0x2e);
xterm256::color value(0xe6, 0xdb, 0x74);

int main(int argc, const char * argv[]) {
    xterm << keyword << "HTTP"
          << op      << "/"
          << number  << "2"
          << text    << " "
          << number  << "200"
          << text    << " "
          << reason  << "OK"
          << "\e[0m\n";
}
