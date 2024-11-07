#include <array>
#include <chrono>
#include <iostream>
#include <thread>
#include <utility>
#include <vector>

class Fuga
{
private:
  size_t i{0};
  // Copyに時間のかかるクラス
public:
  Fuga() = default;
  explicit Fuga(const size_t i) : i(i) { std::cout << "Fuga created " << i << std::endl; };
  Fuga(const Fuga & ref) : i(ref.i)
  {
    std::cout << "Fuga start copy\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Fuga copied\n";
  }
  Fuga & operator=(const Fuga & ref)
  {
    std::cout << "operator::Fuga start copy\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "operator::Fuga copied\n";
    i = ref.i;
    return *this;
  }

  // array全体のmoveの際はFugaのmove constructorが呼ばれる
  // move constructor
  Fuga(Fuga && ref) : i(ref.i)
  {
    std::cout << "Fuga start move\n";
    std::cout << "Fuga moved\n";
  }
  Fuga & operator=(Fuga && ref)
  {
    std::cout << "operator::Fuga start move\n";
    std::cout << "operator::Fuga moved\n";
    i = ref.i;
    return *this;
  }

  size_t get() { return i; }
};

class FugaArray
{
public:
  std::array<Fuga, 10> fugas_;

  explicit FugaArray(std::array<Fuga, 10> fugas) : fugas_(std::move(fugas))
  {
    std::cout << "FugaArray created\n";
  }
};

int main()
{
  std::cout << "=====Create Fugas===== \n";

  std::array<Fuga, 10> base_fugas;
  for (size_t i = 0; i < 10; i++) {
    base_fugas.at(i) = Fuga(i);
  }

  std::cout << "=====Copy Fugas===== \n";
  auto start = std::chrono::system_clock::now();
  FugaArray fuga_container1(base_fugas);
  auto end = std::chrono::system_clock::now();
  std::cout << "Copy: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";

  std::cout << "=====Move Fugas===== \n";
  start = std::chrono::system_clock::now();
  FugaArray fuga_container2(std::move(base_fugas));
  end = std::chrono::system_clock::now();
  std::cout << "Move: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";

  std::cout << "=====Cannot Access Base Fugas===== \n";
  for (size_t i = 0; i < 10; i++) {
    std::cout << base_fugas.at(i).get() << std::endl;
  }

  std::cout << "=====Can Access FugaArray===== \n";
  for (size_t i = 0; i < 10; i++) {
    std::cout << fuga_container2.fugas_.at(i).get() << std::endl;
  }

  return 0;
}
