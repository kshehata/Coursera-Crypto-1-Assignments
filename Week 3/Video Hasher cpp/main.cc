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
  cout << "Usage:" << endl;
  cout << "  Generate: " << name << " [input file] [output file]" << endl;
  cout << "  Verify: " << name << " -V [input file] [first hash]" << endl;
  cout << "Calculates hashes for video file and inserts them in output" << endl;
}

int main(int argc, const char * argv[]) {
  if (argc < 3 || argc > 4) {
    usage(argv[0]);
    return 1;
  }

  if (argc == 3) {
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

  } else if (argc == 4) {
    if (string("-V").compare(argv[1])) {
      usage(argv[0]);
      return 1;
    }

    ifstream input(argv[2], std::ios::binary);
    if (!input) {
      cout << "Could not open input file" << endl;
      return 2;
    }
    cout << "Verifying hashes..." << endl;
    if (verify_hashes(input, hex2bytes(argv[3]))) {
      cout << "All hashes correct" << endl;
    } else {
      cout << "VERIFICATION FAILED!" << endl;
    }
  }
  return 0;
}
