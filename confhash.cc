#include <string_view>
#include <cstdint>
#include <iostream>
#include <fmt/format.h>

using CephLit = uint64_t;

constexpr CephLit hashstr(const char* s, size_t index = 0) {
    return s + index == nullptr || s[index] == '\0' ? 55 : hashstr(s, index + 1) * 33 + (unsigned char)(s[index]);
}

constexpr CephLit operator""_xhash(const char* s, size_t l)
{
  return hashstr(s);
}





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
  auto h1 = "hello"_xhash;
  auto h2 = "world"_xhash;

  prs("world");
  prs("sss");
  prs("hello");
}

