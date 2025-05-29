#include <stdio.h>
#include <string.h>
#include <math.h>
#include <string>
#include <queue>
#include "regex"

typedef struct{
   std::string mName;
}VPortData_tct;

class VPortData
{
public:
   VPortData(void):mName(){};
   ~VPortData(void){};
   std::string mName;
};
class VSwitchData
{
public:
   VSwitchData(void):mPortList(){};
   ~VSwitchData(void){};

   int push(const char * aNamePtr){
      int result = EINVAL;
      if(NULL != aNamePtr)
      {
         VPortData portData;
         portData.mName    = std::string(aNamePtr);
         mPortList.push(portData);
         result = 1;
      }
      return result;
   }
   int pop(VPortData & aPortData){
      int result = EIO;
      if(!mPortList.empty()){
         aPortData = mPortList.front();
         mPortList.pop();
         result = 0;
      }
      return result;
   };
private:
   std::queue<VPortData> mPortList;
};
/*class get values form argv and 
   print out hex values in decimal format  */
class hex {
   /*unsigned uint64 value*/
    long long int dec_value;
    char input_string[48];
public:
      hex(int argc, const char * argv[]);
      void print();
   };
hex::hex(int argc, const char * argv[]) {
    if (argc == 2) {
         strcpy(input_string, argv[1]);
         /*if starts from 0x then handle as hex else as decimal*/
         if (input_string[0] == '0' && input_string[1] == 'x') {
            dec_value = strtoll(input_string, NULL, 16);
         } else {
            dec_value = strtoll(input_string, NULL, 10);
         }
         dec_value = !!dec_value;
    }   else {
        printf("Usage: hex <hex value> \n");
    }
}
void hex::print() {
   /*print dec_value in hex format dec_value could be bigger then long int*/
   printf("0x%llx - %lld\n", dec_value, dec_value);
   /*print dec_value byte by byte intepretation adaptive to value size*/
   for (int i = 0; ((long long int)1 << i*8) < dec_value; i++) {
      unsigned char current_byte = (dec_value >> (i*8)) & 0xff;
      printf("Byte %d: 0x%x - %u \n",i, current_byte, current_byte);
   }
}
const char* test_string[] = {"  Cleared:     Thu Jan  1 00:00:11 1970 \n",
"  References:  1              \n",
"  In4/Pass:    [ Packets: 12                  Bytes: 0                ]",
"  In4/Block:   [ Packets: qewr                  Bytes: 0                ]",
"  Out4/Pass:   [ Packets: 6                  Bytes: 360              ]",
"  Out4/Block:  [ Packets: 777                  Bytes: 0                ]",
"  In6/Pass:    [ Packets: 999                  Bytes: 0                ]",
"  In6/Block:   [ Packets: 1                  Bytes: 0                ]",
"  Out6/Pass:   [ Packets: 2                  Bytes: 76               ]",
"  Out6/Block:  [ Packets: 3                  Bytes: 0                ]",
"  References:  1              \n",
"  In4/Pass:    [ Packets: 1                  Bytes: 0                ]",
"  In4/Block:   [ Packets: 1                  Bytes: 0                ]",
"  Out4/Pass:   [ Packets: 1                  Bytes: 360              ]",
"  Out4/Block:  [ Packets: 0                  Bytes: 0                ]",
"  In6/Pass:    [ Packets: 1                  Bytes: 0                ]",
"  In6/Block:   [ Packets: 1                  Bytes: 0                ]",
"  Out6/Pass:   [ Packets: 1                  Bytes: 76               ]",
"  Out6/Block:  [ Packets: 1                  Bytes: 0                ]",

};  

int main(int argc, const char * argv[]) {
   int ip4InFramePass = 0;
   int ip4InFrameBlock = 0;
   int ip6InFramePass = 0;
   int ip6InFrameBlock = 0;
   int ip4OutFramePass = 0;
   int ip4OutFrameBlock = 0;
   int ip6OutFramePass = 0;
   int ip6OutFrameBlock = 0;   
   hex hex1(argc, argv);
   hex1.print();
   VSwitchData switchData;
   switchData.push("port1");
   switchData.push("port2");
   VPortData portData;
   switchData.pop(portData);
   printf("Port name: %s\n", portData.mName.c_str());
   switchData.pop(portData);
   printf("Port name: %s\n", portData.mName.c_str());
   for (const char* line : test_string) {
      std::regex re("([InOut]{2,3})(4|6)/([PassBlock]{4,5}):\\s*\\[\\s*Packets:\\s*(\\d+)\\s*Bytes:\\s*(\\d+)\\s*\\]");
      std::smatch match;
      std::string lineStr(line);
      if (std::regex_search(lineStr, match, re)) {
         printf("Matched: %s\n", line);
         int packets = std::stoi(match[4].str());
         if (match[1] == "In") {
            if (match[2] == "4") {
               if (match[3] == "Pass") {
                  ip4InFramePass += packets;
               } else {
                  ip4InFrameBlock += packets;
               }
            } else {
               if (match[3] == "Pass") {
                  ip6InFramePass += packets;
               } else {
                  ip6InFrameBlock += packets;
               }
            }
         } else {
            if (match[2] == "4") {
               if (match[3] == "Pass") {
                  ip4OutFramePass += packets;
               } else {
                  ip4OutFrameBlock += packets;
               }
            } else {
               if (match[3] == "Pass") {
                  ip6OutFramePass += packets;
               } else {
                  ip6OutFrameBlock += packets;
               }
            }
         }
      } else {
         printf("No match found for: %s\n", line);
      }

   }
   printf("IP4 In Pass: %d, IP4 In Block: %d, IP6 In Pass: %d, IP6 In Block: %d\n", 
         ip4InFramePass, ip4InFrameBlock,
         ip6InFramePass, ip6InFrameBlock);
   printf("IP4 Out Pass: %d, IP4 Out Block: %d, IP6 Out Pass: %d, IP6 Out Block: %d\n",
         ip4OutFramePass, ip4OutFrameBlock,
         ip6OutFramePass, ip6OutFrameBlock);   
   return 0;
}
