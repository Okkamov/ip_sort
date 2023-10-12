#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#define OCTET_COUNT 4

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

// Для ревьювера. Реализовал максимально отказавшись от принципов KISS и YAGNI
// в надежде получить побольше комментариев по применению ООП и лямбд.
// Разумеется можно было всё cделать через обычные функции и сортировку
// контейнера vector<vector<string>>. Можно было и с использованием ranges 20-го
// стандарта, но надо ли?

using std::string, std::vector;

namespace otus {

class IP {
  vector<uint8_t> ip_{};

public:
  IP() = default; //Пересмотрел лекцию по инициализации добавил дефолтный конструктор

  IP(string ipstr) {
    for (auto &octet : split(ipstr, '.')) {
      ip_.push_back(static_cast<uint8_t>(stoi(octet)));
    }
  }

  vector<uint8_t> get_ip() { return ip_; }
  bool operator==(const IP &other) const { return ip_ == other.ip_; }

  void ip_to_uint32(uint32_t &ip32, const uint8_t &ip) const {
    ip32 = (ip32 << 8) | ip;
  }

  bool operator<(const IP &other) const {

    uint32_t ipl{0}, ipr{0};
    for (int i = 0; i < OCTET_COUNT; ++i) {
      ip_to_uint32(ipl, ip_.at(i));
      ip_to_uint32(ipr, other.ip_.at(i));
    }
    return (ipl < ipr ? true : false);
  }

  bool operator>(const IP &other) const {
    auto ita = other.ip_.begin();
    uint32_t ipl{0}, ipr{0};
    for_each(ip_.begin(), ip_.end(), [&](auto &itb) {
      ip_to_uint32(ipl, itb);
      ip_to_uint32(ipr, *ita++);
    });
    return (ipl > ipr ? true : false);
  }
  void print() {
    string ipstr{};
    for_each(std::begin(ip_), std::end(ip_),
             [&ipstr](auto &octet) { ipstr += std::to_string(octet) + "."; });
    //Для ревьювера. Цикл здесь смотрится даже более органично:
    /*for (auto octet : ip_) {
      ipstr += std::to_string(octet) + ".";
    }*/
    ipstr.pop_back();
    std::cout << ipstr << std::endl;
  }

  uint8_t at(const int &idx) { return ip_.at(idx); }
};

class IPPool : IP {
  std::multiset<IP, std::greater<>> ip_pool{};

public:
  IPPool() = default;

  void insert(IP &ip) { ip_pool.insert(ip); }

  void print() {
    for (auto ip : ip_pool) {
      ip.print();
    }
  }
  void print(vector<int> filter) {
    for (auto ip : ip_pool) {
      bool is_equal = false;
      for (int i = 0; i < static_cast<int>(filter.size()); ++i) {
        if (+(ip.at(i)) != static_cast<uint8_t>(filter.at(i))) {
          is_equal = false;
          break;
        } else {
          is_equal = true;
        }
      }
      if (is_equal)
        ip.print();
    }
  }
  void print(int filter) {
    for (auto ip : ip_pool) {
      for (int i = 0; i < OCTET_COUNT; ++i) {
        if (+(ip.at(i)) == static_cast<uint8_t>(filter)) {
          ip.print();
          break;
        }
      }
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
    ipp.print();             // no filtering
    ipp.print({{1}});        // ip = filter(1) by mask
    ipp.print({{46}, {70}}); // ip = filter(46, 70) by mask
    ipp.print(46);           // ip = filter_any(46)

  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}