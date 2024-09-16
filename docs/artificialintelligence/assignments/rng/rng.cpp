// add your imports here
#include <fstream>
#include <iostream>
#include <istream>
const std::string TEST_FOLDER = "\\tests\\";
unsigned int xorShift(unsigned int seed, int r1, int r2);
unsigned short sm64Rand(unsigned short seed);
int main(){
  // code here
  //unsigned int seed, N, min, max;
  /*std::cin >> seed >> N >> min >> max;
  unsigned int i;
  for(i = N; i >= 1; i--)
  {
    //Run xor shift
    seed = xorShift(seed, min, max);
  }*/
  unsigned short seed = 0;
  for(int i = 0; i < 10; i++) {
    std::cout << std::to_string(seed = sm64Rand(seed)) << "\n";
  }
}
//The purpose of this function is to take the number and xor shift it to output a pseudo-random number
    unsigned int xorShift(unsigned int seed, int r1, int r2)
{
  seed = seed xor (seed << 13);
  seed = seed xor (seed >> 17);
  seed = seed xor (seed << 5);
  int value = r1 + (seed % (r2 - r1 + 1)); //clamps the value to between r1 and r2
          //output the new values
          std::cout << value << std::endl;
  return seed;
}

unsigned short sm64Rand(unsigned short seed) {
  if(seed == 0x560A)
    seed = 0;
  unsigned short a = (seed & 0xFF) << 8;
  a = a ^ seed;

  //Seed becomes a with the bytes swapped
  seed = ((a & 0x00FF) << 8) | ((a & 0xFF00) >> 8);

  a = ((a & 0xFF) << 1) ^ seed; //Last byte shifted left 1 bit

  short b = (a >> 1) ^ 0xFF80;

  if((a & 1) == 0) {
    if(b == 0xAA55)
      seed = 0;
    else
      seed = b ^ 0x1FF4;
  }
  else
    seed = b ^ 0x8180;

  return seed;
}

