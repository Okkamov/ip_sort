#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <set>
#include <string>
#include <vector>

std::vector<std::string> split(const std::string &str, char d) {
  std::vector<std::string> r;

  std::string::size_type start = 0;
  std::string::size_type stop = str.find_first_of(d);
  while (stop != std::string::npos) {
    r.push_back(str.substr(start, stop - start));

    start = stop + 1;
    stop = str.find_first_of(d, start);
  }

  r.push_back(str.substr(start));

  return r;
}

using std::string, std::vector;
namespace otus {
class IP {

  using container = std::array<uint8_t, 4>;
  container ip_;

public:
  IP() = default;

  IP(const string &ipstr) {
    int i = 0;
    for (const auto &octet : split(ipstr, '.')) {
      ip_.at(i) = static_cast<uint8_t>(stoi(octet));
      ++i;
    }
  }

  bool operator<(const IP &other) const { return ip_ < other.ip_; }

  bool operator>(const IP &other) const { return ip_ > other.ip_; }

  using iterator = typename container::iterator;
  iterator begin() { return ip_.begin(); }
  iterator end() { return ip_.end(); }

  void print() {
    string ipstr{};
    for (auto octet : ip_) {
      ipstr += std::to_string(octet) + ".";
    }
    ipstr.pop_back();
    std::cout << ipstr << std::endl;
  }

  uint8_t at(const int &idx) { return ip_.at(idx); }
};

class IPPool : IP {
  std::multiset<IP, std::greater<>> ip_pool;

public:
  void insert(const IP &ip) { ip_pool.insert(ip); }

  void print() {
    for (auto ip : ip_pool) {
      ip.print();
    }
  }

  template <class Predicat> void print(Predicat filter) {
    for (auto ip : ip_pool) {
      if (filter(ip))
        ip.print();
    }
  }
};

} // namespace otus

int main() {
  try {
    otus::IPPool ipp;
    for (std::string line; std::getline(std::cin, line);) {
      otus::IP ip{split(line, '\t').at(0)};
      ipp.insert(ip);
    }

    ipp.print(); // no filtering

    
    vector<uint8_t> octets{1}; // ip = filter(1) by mask
    auto filter = [&](otus::IP &ip) -> bool {
      for (int i = 0; i < static_cast<int>(octets.size()); ++i)
        if (ip.at(i) != octets.at(i))
          return false;
      return true;
    };
    ipp.print(filter); // end ip = filter(1) by mask

    octets.clear(); // ip = filter(46, 70) by mask
    octets = {46, 70};
    ipp.print(filter);

    uint8_t octet = 46; // ip = filter_any(46)
    auto filter_any = [&](otus::IP &ip) -> bool {
      return std::any_of(ip.begin(), ip.end(),
                         [&](uint8_t &i) { return i == octet; });
    };
    ipp.print(filter_any);

  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
