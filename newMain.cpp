#include <iostream>
#include <fstream>
//#include <vector>
#include "Room.hpp"
using namespace std;
int main()
{
    string fileName = "text1.txt";
    ifstream fout;
    fout.open(fileName);
    Room room1(fout);
    room1.start();


    return 0;
}
