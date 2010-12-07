/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * Hex-Viewer                                                    *
 * Author: Arash Partow (2002-2010)                              *
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
   Description: This example demonstrates a simple hex-viewer
                application using the String Toolkit Library's
                bin to hex conversion functionality. The viewer
                takes input from the stdin, processes the input
                and outputs it to stdout. An example of the
                output:

   00000006D0  696474685D3B0A0A202020666F722820 idth];.....for(.
   00000006E0  3B203B20290A2020207B0A2020202020 ;.;.)....{......
   00000006F0  207374643A3A63696E2E726561642872 .std::cin.read(r
   0000000700  65696E746572707265745F636173743C einterpret_cast<
   0000000710  636861722A3E28627566666572292C64 char*>(buffer),d
   0000000720  6174615F7769647468293B0A20202020 ata_width);.....
   0000000730  20206966202821287374643A3A63696E ..if.(!(std::cin
   0000000740  2E656F662829207C7C207374643A3A63 .eof().||.std::c
   0000000750  696E2E626164282929290A2020202020 in.bad()))......
   0000000760  207B0A20202020202020202073747274 .{..........strt
   0000000770  6B3A3A636F6E766572745F62696E5F74 k::convert_bin_t
   0000000780  6F5F686578286275666665722C627566 o_hex(buffer,buf
   0000000790  666572202B20646174615F7769647468 fer.+.data_width
   00000007A0  2C6865785F627566666572293B0A2020 ,hex_buffer);...
   00000007B0  202020202020207374643A3A636F7574 .......std::cout
   00000007C0  203C3C207374643A3A686578203C3C20 .<<.std::hex.<<.
   00000007D0  7374643A3A73657466696C6C28273027 std::setfill('0'
   00000007E0  29203C3C207374643A3A736574772831 ).<<.std::setw(1
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

   unsigned char buffer[data_width];
   unsigned char hex_buffer[2 * data_width];

   for ( ; ; )
   {
      std::cin.read(reinterpret_cast<char*>(buffer),data_width);
      if (!(std::cin.eof() || std::cin.bad()))
      {
         strtk::convert_bin_to_hex(buffer,buffer + data_width,hex_buffer);
         //print the address offset
         std::cout << std::hex << std::uppercase << std::setfill('0') << std::setw(10) << current_address << "  ";

         //print the hexadecimal representation
         std::copy(hex_buffer,hex_buffer + (2 * data_width),std::ostream_iterator<unsigned char>(std::cout,""));
         std::cout << " ";

         //print the printable view representation
         strtk::convert_to_printable_chars(buffer,buffer + data_width);
         std::copy(buffer,buffer + data_width,std::ostream_iterator<unsigned char>(std::cout,""));
         std::cout << std::endl;
         current_address += data_width;
      }
      else
      {
         std::size_t read_in_width = 0;
         if (0 != (read_in_width = static_cast<std::size_t>(std::cin.gcount())))
         {
            strtk::convert_bin_to_hex(buffer,buffer + read_in_width,hex_buffer);
            std::cout << std::hex << std::uppercase << std::setw(10) << current_address << "  ";
            std::copy(hex_buffer,hex_buffer + (2 * read_in_width),std::ostream_iterator<unsigned char>(std::cout,""));
            std::cout << std::string(2 * (16 - read_in_width) + 1, ' ');
            strtk::convert_to_printable_chars(buffer,buffer + read_in_width);
            std::copy(buffer,buffer + read_in_width,std::ostream_iterator<unsigned char>(std::cout,""));
            std::cout << std::endl;
         }
         break;
      }
   }

   return 0;
}
