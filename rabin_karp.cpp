#include <iostream>
#include <string>
#include <vector>

#include <math.h>
#include <stdlib.h>

#define D 256
#define Q 257

using namespace std;

void search_rk(string str, string pattern)
{
  unsigned int str_len = str.length();
  unsigned int pattern_len = pattern.length();
  unsigned int window_hash = 0;
  unsigned int pattern_hash = 0;

  const char * c_str = str.c_str();
  const char * c_pattern = pattern.c_str();

  // Calculate hash for pattern.
  // H(pattern) = (0 + pattern[0]) % Q + (H(0) * 256 + pattern[1]) % Q + ... + () % Q
  for (int i = 0; i < pattern_len; i++)
  {
    pattern_hash += (int) pattern[i];
    window_hash += (int) str[i];
  }

  if (pattern_hash == window_hash)
  {
    cout << "Found" << endl;
  }

  for (int i = 1; i < (str_len - pattern_len + 1); i++)
  {
    // Calculate new window hash as:
    // H(str[a + 1...m + 1]) = H(str[a..m]) - H(str[a]) + H(str[m + 1])
    window_hash -= (int) str[i - 1];
    window_hash += (int) str[i + pattern_len - 1];

    // Check if current window hash matches pattern hash
    if (window_hash == pattern_hash)
    {
      // If hash matches, iterate to check if characters match.
      if (!strncmp(c_str + i, c_pattern, pattern_len))
      {
        cout << "Found" << endl;
      }
    }
  }
}

int main(int argc, char *argv[])
{
  search_rk("My name is XYZ", "XYZ");
  return 0;
}
