#include <iostream>
#include <string>
#include <vector>

#include <math.h>
#include <stdlib.h>

#include "Search.h"

#define D 256

using namespace std;

void search_rk(char * str, unsigned int str_len, char * pattern, unsigned int pattern_len, vector<unsigned int> &positions)
{
  unsigned int window_hash = 0;
  unsigned int pattern_hash = 0;

  // Calculate hash for pattern.
  // H(s) = (s[0] * D ^ (len - 1)) + ... + (s[len - 1] * D ^ (len - len))
  for (int i = 0; i < pattern_len; i++)
  {
    pattern_hash += ((int) pattern[i]) * pow(D, pattern_len - i - 1);
    window_hash += ((int) str[i]) * pow(D, pattern_len - i - 1);
  }

  for (int i = 0; i < (str_len - pattern_len + 1); i++)
  {
    // Check if current window hash matches pattern hash
    if (window_hash == pattern_hash)
    {
      // If hash matches, iterate to check if characters match.
      if (!strncmp(str + i, pattern, pattern_len))
      {
        positions.push_back(i);
      }
    }

    // Calculate new window hash as:
    // H(s[a + 1...m + 1]) = (H(s[a..m]) - H(s[a])) * D + H(str[m + 1])
    // Multiply D because we're sliding the window and we need to match match
    // the exponent of the pattern.
    window_hash -= (((int) str[i]) * pow(D, pattern_len - 1));
    window_hash *= D;
    window_hash += ((int) str[i + pattern_len]);
  }
}
