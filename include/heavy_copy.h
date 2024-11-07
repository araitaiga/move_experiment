#pragma once
#include <chrono>
#include <iostream>
#include <thread>
#include <utility>
#include <vector>

class HeavyCopy
{
  // Copyに時間のかかるクラス
private:
  std::string str{""};

public:
  HeavyCopy() = default;
  explicit HeavyCopy(const std::string str) : str(str)
  {
    std::cout << "HeavyCopy created " << str << std::endl;
  };
  HeavyCopy(const HeavyCopy & ref) : str(ref.str)
  {
    std::cout << "HeavyCopy start copy " << ref.str << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "HeavyCopy copied" << std::endl;
  }
  HeavyCopy & operator=(const HeavyCopy & ref)
  {
    std::cout << "operator::HeavyCopy start copy " << ref.str << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "operator::HeavyCopy copied" << std::endl;
    str = ref.str;
    return *this;
  }

  std::string get() { return str; }
};

class HeavyCopyMove
{
  // Copyに時間のかかるクラス
  // move constructorあり
private:
  std::string str{""};

public:
  HeavyCopyMove() = default;
  explicit HeavyCopyMove(const std::string str) : str(str)
  {
    std::cout << "HeavyCopyMove created " << str << std::endl;
  };
  HeavyCopyMove(const HeavyCopyMove & ref) : str(ref.str)
  {
    std::cout << "HeavyCopyMove start copy " << ref.str << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "HeavyCopyMove copied" << std::endl;
  }
  HeavyCopyMove & operator=(const HeavyCopyMove & ref)
  {
    std::cout << "operator::HeavyCopyMove start copy " << ref.str << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "operator::HeavyCopyMove copied" << std::endl;
    str = ref.str;
    return *this;
  }

  // array全体のmoveの際はFugaのmove constructorが呼ばれる
  // move constructor内でstd::stringのmove constructorを呼ぶ
  HeavyCopyMove(HeavyCopyMove && ref) : str(std::move(ref.str))
  {
    std::cout << "HeavyCopyMove start move " << ref.str << std::endl;
    std::cout << "HeavyCopyMove moved" << std::endl;
  }
  HeavyCopyMove & operator=(HeavyCopyMove && ref)
  {
    std::cout << "operator::HeavyCopyMove start move " << ref.str << std::endl;
    std::cout << "operator::HeavyCopyMove moved" << std::endl;
    str = std::move(ref.str);
    return *this;
  }

  std::string get() { return str; }
};
