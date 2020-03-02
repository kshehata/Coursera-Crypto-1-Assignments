//  Created by Kareem Shehata on 2020-02-17.
//  Copyright © 2020 Kareem Shehata. All rights reserved.

#include "dlog.h"

#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::make_unique;
using std::string;
using std::unique_ptr;

void usage(const string& name) {
  cout << "Usage: " << name << "[g] [h] [p]" << endl;
  cout << "Calculates x that solve h = g**x mod p" << endl;
}

int main(int argc, const char * argv[]) {
  if (argc != 4) {
    usage(argv[0]);
    return 1;
  }

  mpz_class g(argv[1]);
  mpz_class h(argv[2]);
  mpz_class p(argv[3]);

  auto result = dlog(g, h, p);
  if (result == 0) {
    cout << "Could not find DLOG!" << endl;
  } else {
    cout << result.get_str(10);
  }
  return 0;
}
