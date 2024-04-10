#include <benchmark/benchmark.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

using ConfLit = uint64_t;

constexpr ConfLit hashstr(const char* s, size_t index = 0)
{
  return s[index] == '\0' ? 55 : hashstr(s, index + 1) * 33 + (unsigned char)(s[index]);
}

consteval ConfLit operator""_xhash(const char* s, size_t l)
{
  return hashstr(s);
}


// define some possible flags

constexpr ConfLit SCRUB_W_RESERVER = "scrub_w_reserver"_xhash;
constexpr ConfLit SOMETHING_AFTER_1917 = "something_after_1917"_xhash;

// create a vector of available flags
// (let's say - only those divided by 3)

using all_flags_t = std::vector<ConfLit>;

// a pre-prepared hashes for all opt_0xx flags
std::array<ConfLit, 64> opt_flags;

all_flags_t init_cur_flags()
{
  // first - prepare our array of all possible flags
  for (int i = 0; i < 64; i++) {
    auto flag_name = fmt::format("opt_{:03d}", i);
    opt_flags[i] = hashstr(flag_name.c_str());
  }

  all_flags_t flags{SCRUB_W_RESERVER, SOMETHING_AFTER_1917};

  // do it the least optimal way
  for (int i = 3; i < 64; i += 3) {
    auto flag_name = fmt::format("opt_{:03d}", i);
    flags.push_back(hashstr(flag_name.c_str()));
  }

  return flags;
}


// a function to check if a flag is set
inline bool is_flag_set(ConfLit flag, const all_flags_t& flags)
{
  return std::find(flags.begin(), flags.end(), flag) != flags.end();
}



int cnt_matches(const all_flags_t& cur)
{
  int cnt = 0;
  for (int i = 63; i > 0; --i) {
    if (is_flag_set(opt_flags[i], cur)) {
      cnt++;
    }
  }
  return cnt;
}

static void test_hash_based(benchmark::State& state)
{
  auto cur_set = init_cur_flags();
  // std::cout << fmt::format("{}: count:{}\n", __func__, cnt_matches(cur_set));
  for (auto _ : state) {
    benchmark::DoNotOptimize(cnt_matches(cur_set));
  }
}

BENCHMARK(test_hash_based);

// ------------------------------------------------------------------------------------------------------------ //

// compare against just checking a bit (1 instruction, probably...)
inline bool is_flag_set_bit(uint64_t flag, uint64_t flags)
{
  return flags & flag;
}


uint64_t init_bit_flags()
{
  uint64_t flags = 0;
  for (int i = 3; i < 64; i += 3) {
    flags |= (0x1 << i);
  }

  return flags;
}

int cnt_bit_matches(uint64_t cur)
{
  int cnt = 0;
  for (int i = 0; i < 64; i++) {
    if (is_flag_set_bit((0x01ULL << i), cur)) {
      cnt++;
    }
  }
  return cnt;
}

static void test_bit_based(benchmark::State& state)
{
  uint64_t cur_set = init_bit_flags();
  //std::cout << fmt::format("{}: {:x} count:{}\n", __func__, cur_set, cnt_bit_matches(cur_set));
  for (auto _ : state) {
    benchmark::DoNotOptimize(cnt_bit_matches(cur_set));
  }
}

BENCHMARK(test_bit_based);


// ------------------------------------------------------------------------------------------------------------ //




#if 1

BENCHMARK_MAIN();

#else
void prs(char* inp)
{
  switch (hashstr(inp)) {
    case "world"_xhash: std::cout << "as in world\n"; break;
    case "hello"_xhash: std::cout << "as in hello\n"; break;
    default: std::cout << "xxxx\n"; break;
  }
}

int main()
{
  //   auto h1 = "hello"_xhash;
  //   auto h2 = "world"_xhash;

  prs((const char*)("world"));
  prs("sss");
  prs("hello");
}
#endif
