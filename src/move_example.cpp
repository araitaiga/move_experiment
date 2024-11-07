#include <chrono>
#include <iostream>
#include <thread>
#include <utility>
#include <vector>

class Fuga
{
private:
  size_t i;
  // Copyに時間のかかるクラス
public:
  explicit Fuga(const size_t i) : i(i) { std::cout << "Fuga created " << i << std::endl; };
  Fuga(const Fuga & rhs) : i(rhs.i)
  {
    std::cout << "Fuga start copy\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Fuga copied\n";
  }

  size_t get() { return i; }
};

class FugaContainer
{
public:
  std::unique_ptr<Fuga> fuga_ptr;

  FugaContainer(const size_t i) : fuga_ptr(std::make_unique<Fuga>(i))
  {
    std::cout << "FugaContainer created as " << i << std::endl;
  };

  // copy constructor
  FugaContainer(const FugaContainer & rhs) : fuga_ptr(std::make_unique<Fuga>(*rhs.fuga_ptr))
  {
    std::cout << "FugaContainer start copy. wait...\n";
    std::cout << "FugaContainer copied\n";
  }
  // move constructor
  FugaContainer(FugaContainer && rhs) : fuga_ptr(std::move(rhs.fuga_ptr))
  {
    std::cout << "FugaContainer start move\n";
    std::cout << "FugaContainer moved\n";
  }

  // copy assignment operator
  FugaContainer & operator=(const FugaContainer & rhs)
  {
    std::cout << "assignment::FugaContainer start copy. wait...\n";
    fuga_ptr = std::make_unique<Fuga>(*rhs.fuga_ptr);
    std::cout << "assignment::FugaContainer assignment copied\n";
    return *this;
  }
  // move assignment operator
  FugaContainer & operator=(FugaContainer && rhs)
  {
    std::cout << "assignment::FugaContainer start move\n";
    fuga_ptr = std::move(rhs.fuga_ptr);
    std::cout << "assignment::FugaContainer moved\n";
    return *this;
  }
};

void copyFugaContainer(const FugaContainer & base_fuga_container, FugaContainer & fuga_container)
{
  std::cout << "copyFugaContainer\n";
  fuga_container.fuga_ptr.reset();
  size_t new_value = base_fuga_container.fuga_ptr->get() + 1;
  fuga_container.fuga_ptr = std::make_unique<Fuga>(new_value);
}

FugaContainer moveFugaContainer(const FugaContainer & base_fuga_container)
{
  std::cout << "moveFugaContainer\n";
  size_t new_value = base_fuga_container.fuga_ptr->get() + 1;
  // RVO. move必要なし
  return FugaContainer(new_value);
}

int main()
{
  std::cout << "=====Copy Constructor===== \n";
  auto fuga_container1 = FugaContainer(1);
  auto start = std::chrono::system_clock::now();
  FugaContainer fuga_container2(fuga_container1);
  auto end = std::chrono::system_clock::now();
  std::cout << "Copy: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";

  std::cout << "=====Copy Assignment===== \n";
  start = std::chrono::system_clock::now();
  FugaContainer fuga_container3 = fuga_container1;
  end = std::chrono::system_clock::now();
  std::cout << "Copy Assignment: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";

  std::cout << "=====Move Constructor===== \n";
  auto fuga_container4 = FugaContainer(3);
  start = std::chrono::system_clock::now();
  FugaContainer fuga_container5(std::move(fuga_container4));
  end = std::chrono::system_clock::now();
  std::cout << "Move: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";

  std::cout << "=====Move Assignment===== \n";
  start = std::chrono::system_clock::now();
  FugaContainer fuga_container6 = std::move(fuga_container5);
  end = std::chrono::system_clock::now();
  std::cout << "Move Assignment: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";

  std::cout << "=====copyFugaContainer===== \n";
  auto fuga_container7 = FugaContainer(4);
  auto fuga_container8 = FugaContainer(5);
  start = std::chrono::system_clock::now();
  copyFugaContainer(fuga_container7, fuga_container8);
  end = std::chrono::system_clock::now();
  std::cout << "copyFugaContainer: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";

  std::cout << "=====moveFugaContainer===== \n";
  auto fuga_container9 = FugaContainer(6);
  start = std::chrono::system_clock::now();
  FugaContainer fuga_container10 = moveFugaContainer(fuga_container9);
  end = std::chrono::system_clock::now();
  std::cout << "moveFugaContainer: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";

  start = std::chrono::system_clock::now();
  FugaContainer fuga_container11 = moveFugaContainer(std::move(fuga_container9));
  end = std::chrono::system_clock::now();
  std::cout << "moveFugaContainer: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";

  return 0;
}
