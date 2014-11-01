/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * Hex-Viewer                                                    *
 * Author: Arash Partow (2002-2014)                              *
 * URL: http://www.partow.net/programming/strtk/index.html       *
 *                                                               *
 * Copyright notice:                                             *
 * Free use of the String Toolkit Library is permitted under the *
 * guidelines and in accordance with the most current version of *
 * the Common Public License.                                    *
 * http://www.opensource.org/licenses/cpl1.0.php                 *
 *                                                               *
 *****************************************************************
*/


/*
   Description: This example  demonstrates a  simple hex-viewer  application
                using the  String Toolkit  Library's bin  to hex  conversion
                functionality.  The  viewer  takes  input  from  the  stdin,
                processes the input and outputs it to stdout. An example  of
                the output:

   0000000AD0 73203D20303B0A0A202020756E736967 s.=.0;.....unsig
   0000000AE0 6E65642063686172206275666665725B ned.char.buffer[
   0000000AF0 646174615F77696474685D3B0A202020 data_width];....
   0000000B00 756E7369676E65642063686172206865 unsigned.char.he
   0000000B10 785F6275666665725B32202A20646174 x_buffer[2.*.dat
   0000000B20 615F77696474685D3B0A0A202020666F a_width];.....fo
   0000000B30 722028203B203B20290A2020207B0A20 r.(.;.;.)....{..
   0000000B40 20202020207374643A3A63696E2E7265 .....std::cin.re
   0000000B50 6164287265696E746572707265745F63 ad(reinterpret_c
   0000000B60 6173743C636861722A3E286275666665 ast<char*>(buffe
   0000000B70 72292C646174615F7769647468293B0A r),data_width);.
   0000000B80 2020202020206966202821287374643A ......if.(!(std:
   0000000B90 3A63696E2E656F662829207C7C207374 :cin.eof().||.st
   0000000BA0 643A3A63696E2E626164282929290A20 d::cin.bad()))..
   0000000BB0 20202020207B0A202020202020202020 .....{..........
   0000000BC0 737472746B3A3A636F6E766572745F62 strtk::convert_b
   0000000BD0 696E5F746F5F68657828627566666572 in_to_hex(buffer
   0000000BE0 2C627566666572202B20646174615F77 ,buffer.+.data_w

   Example command:
   cat strtk_hexview.cpp | ./strtk_hexview

*/


#include <cstddef>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <iomanip>

#include "strtk.hpp"


int main()
{
   /*
      Note: process takes input from stdin.
      usage: cat file.txt | strtk_hexview
   */
   static const std::size_t data_width = 16;
   std::size_t current_address = 0;

   char buffer[data_width];
   char hex_buffer[2 * data_width];

   std::cin.sync_with_stdio(false);

   for ( ; ; )
   {
      std::cin.read(reinterpret_cast<char*>(buffer),data_width);

      if (!(std::cin.eof() || std::cin.bad()))
      {
         strtk::convert_bin_to_hex(buffer,buffer + data_width,hex_buffer);
         //print the address offset
         std::cout << std::hex << std::uppercase << std::setfill('0') << std::setw(10) << current_address << ' ';

         //print the hexadecimal representation
         std::cout.write(hex_buffer,2 * data_width);
         std::cout << ' ';

         //print the printable view representation
         strtk::convert_to_printable_chars(buffer,buffer + data_width);
         std::cout.write(buffer,data_width);
         std::cout << '\n';
         current_address += data_width;
      }
      else
      {
         std::size_t read_in_width = static_cast<std::size_t>(std::cin.gcount());

         if (0 != read_in_width)
         {
            strtk::convert_bin_to_hex(buffer,buffer + read_in_width,hex_buffer);
            std::cout << std::hex << std::uppercase << std::setw(10) << current_address << ' ';
            std::cout.write(hex_buffer,(2 * read_in_width));
            std::cout << std::string(2 * (16 - read_in_width) + 1, ' ');
            strtk::convert_to_printable_chars(buffer,buffer + read_in_width);
            std::cout.write(buffer,read_in_width);
            std::cout << '\n';
         }

         break;
      }
   }

   return 0;
}
