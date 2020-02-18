#ifndef BLOCKS_BLOCKS_H
#define BLOCKS_BLOCKS_H

#include "utils.h"

bool check_and_remove_pad(string& m);

string decrypt_cbc(const byte_array& key, const byte_array& ct);

string decrypt_ctr(const byte_array& key, byte_array& ct);

#endif  // BLOCKS_UTILS_H
