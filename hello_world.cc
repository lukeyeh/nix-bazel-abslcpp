#include <iostream>
#include <string>
#include <vector>

#include "greeting.h"

int main() {
  std::vector<std::string> v = {"foo", "bar", "baz"};
  std::string s = JoinWords(v);

  std::cout << "Joined string: " << s << "\n";

  return 0;
}
