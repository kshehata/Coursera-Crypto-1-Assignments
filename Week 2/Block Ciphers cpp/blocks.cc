#include "blocks.h"

bool check_and_remove_pad(byte_array& m) {
  if (m.size() <= 0) {
    return true;
  }

  int pad_len = m.back();
  if (pad_len > m.size()) {
    return false;
  }

  for (int i = m.size() - 1; i >= m.size() - pad_len; --i) {
    if (m[i] != pad_len) {
      return false;
    }
  }

  m.resize(m.size() - pad_len);
  return true;
}
