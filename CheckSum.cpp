#include <iostream>
#include <fstream>
#include <bitset>
#include <iomanip>

using namespace std;

int main(int argc, char *argv[]) {
    //read file in binary

    if (argc != 2) {
        cout << "Usage: CheckSum <filename>" << endl;
        return 1;
    }
    ifstream file(argv[1], ios::binary);
    if (!file) {
        cout << "Cannot open file." << endl;
        return 1;
    }


    //----debug-----
    // ifstream file("testfile" , ios::binary);
    //-----debug-----

    //calculate checksum
    //REMEMBER TO INITIALIZE
    bitset<17> checksum(0);
    char buffer[2] = {0};
    // cout << "      " << checksum << endl;
    while (file.read(buffer, 2)) {   //read 2 Byte at a time
        bitset<17> Next2Byte(buffer[1] << 0 | (buffer[0] << 8));
        //transform to unsigned long to perform addition
        unsigned long sum = checksum.to_ulong() + Next2Byte.to_ulong();
        //carry
        if (sum > 0xFFFF) {
            sum = (sum & 0xFFFF) + 1;
        }
        checksum = bitset<17>(sum);
        // cout << buffer[0] << setw(1) << buffer[1] << setw(1) << "   +" << Next2Byte << endl << "----------------------" << endl;
        // cout << "     =" << checksum << endl;
    }
    //close file
    file.close();
    //only get low 16 bits
    bitset<16> last16bits = bitset<16>(checksum.to_ulong() & 0xFFFF);
    string last16bits_str = last16bits.to_string();
    last16bits_str.insert(8, " ");
    // cout << endl;
    cout << "Checksum: "<< last16bits_str << endl;


    //make checksum in HEX
    cout << "Checksum in HEX: " << hex << last16bits.to_ulong() << endl;

}
