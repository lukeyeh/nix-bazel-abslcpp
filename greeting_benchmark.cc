#include <string>
#include <vector>

#include "benchmark/benchmark.h"
#include "greeting.h"

namespace {

void BM_JoinWords(benchmark::State& state) {
  std::vector<std::string> words(state.range(0), "foo");
  for (auto _ : state) {
    benchmark::DoNotOptimize(JoinWords(words));
  }
}
BENCHMARK(BM_JoinWords)->Range(1, 1 << 10);

}  // namespace
