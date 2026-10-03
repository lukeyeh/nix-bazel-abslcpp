#include "greeting.h"

#include <string>
#include <vector>

#include "absl/strings/str_join.h"

std::string JoinWords(const std::vector<std::string>& words) {
  return absl::StrJoin(words, "-");
}
