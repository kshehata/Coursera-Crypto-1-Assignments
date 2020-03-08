//  Copyright © 2020 Kareem Shehata. All rights reserved.


#include "video_hasher.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace video_hasher;
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;

void usage(const string& name) {
  cout << "Usage: " << name << " [input file] [output file]" << endl;
  cout << "Calculates hashes for video file and inserts them in output" << endl;
}

int main(int argc, const char * argv[]) {
  if (argc != 3) {
    usage(argv[0]);
    return 1;
  }
  ifstream input(argv[1], std::ios::binary);
  if (!input) {
    cout << "Could not open input file" << endl;
    return 2;
  }
  ofstream output(argv[2], std::ios::binary);
  if (!output) {
    cout << "Could not open output file" << endl;
    return 3;
  }

  cout << bytes2hex(hash_file(input, output)) << endl;
  return 0;
}
