//  Copyright © 2020 Kareem Shehata. All rights reserved.


#include "video_hasher.h"

#include <iostream>
#include <string>

using namespace video_hasher;
using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::make_unique;
using std::string;
using std::unique_ptr;

void usage(const string& name) {
  cout << "Usage: " << name << " [input file] [output file]" << endl;
  cout << "Calculates hashes for video file and inserts them in output" << endl;
}

int main(int argc, const char * argv[]) {
  if (argc != 3) {
    usage(argv[0]);
    return -1;
  }
  return 0;
}
