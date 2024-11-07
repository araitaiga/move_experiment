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
  HeavyCopy(const HeavyCopy & rhs) : str(rhs.str)
  {
    std::cout << "HeavyCopy start copy " << rhs.str << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "HeavyCopy copied" << std::endl;
  }
  HeavyCopy & operator=(const HeavyCopy & rhs)
  {
    std::cout << "operator::HeavyCopy start copy " << rhs.str << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "operator::HeavyCopy copied" << std::endl;
    str = rhs.str;
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
  HeavyCopyMove(const HeavyCopyMove & rhs) : str(rhs.str)
  {
    std::cout << "HeavyCopyMove start copy " << rhs.str << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "HeavyCopyMove copied" << std::endl;
  }
  HeavyCopyMove & operator=(const HeavyCopyMove & rhs)
  {
    std::cout << "operator::HeavyCopyMove start copy " << rhs.str << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "operator::HeavyCopyMove copied" << std::endl;
    str = rhs.str;
    return *this;
  }

  // array全体のmoveの際はFugaのmove constructorが呼ばれる
  // move constructor内でstd::stringのmove constructorを呼ぶ
  HeavyCopyMove(HeavyCopyMove && rhs) : str(std::move(rhs.str))
  {
    std::cout << "HeavyCopyMove start move " << rhs.str << std::endl;
    std::cout << "HeavyCopyMove moved" << std::endl;
  }
  HeavyCopyMove & operator=(HeavyCopyMove && rhs)
  {
    std::cout << "operator::HeavyCopyMove start move " << rhs.str << std::endl;
    std::cout << "operator::HeavyCopyMove moved" << std::endl;
    str = std::move(rhs.str);
    return *this;
  }

  std::string get() { return str; }
};
