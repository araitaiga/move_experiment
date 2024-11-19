#pragma once
#include <chrono>
#include <iostream>
#include <map>
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
  HeavyCopyMove(HeavyCopyMove && rhs) noexcept : str(std::move(rhs.str))
  {
    std::cout << "HeavyCopyMove start move " << rhs.str << std::endl;
    std::cout << "HeavyCopyMove moved" << std::endl;
  }
  HeavyCopyMove & operator=(HeavyCopyMove && rhs) noexcept
  {
    std::cout << "operator::HeavyCopyMove start move " << rhs.str << std::endl;
    std::cout << "operator::HeavyCopyMove moved" << std::endl;
    str = std::move(rhs.str);
    return *this;
  }

  std::string get() { return str; }
};

class MoveConstructorDeleted
{
  // 自動生成されるコピーコンストラクタが削除されたクラス
private:
  std::map<std::string, HeavyCopyMove> map;

public:
  MoveConstructorDeleted() { std::cout << "CopyConstructorDeleted Constructor" << std::endl; };

  // 1. デストラクタ, コピーコンストラクタ, コピー代入演算子は三位一体で定義すべき
  // 2. 上記のうち一つでも定義された場合, ムーブコンストラクタ, ムーブ代入演算子は自動生成されない
  // デストラクタを定義した場合, 過去の経緯からコピーコンストラクタ, コピー代入演算子は自動生成されるため,
  // このクラスをムーブしようとしてもコンパイルエラーにならずコピーされる
  ~MoveConstructorDeleted() { std::cout << "CopyConstructorDeleted Destructor" << std::endl; };

  // コピー演算は自動生成される
  // MoveConstructorDeleted(const MoveConstructorDeleted & rhs) = delete;
  // MoveConstructorDeleted & operator=(const MoveConstructorDeleted & rhs) = delete;

  // ムーブ演算は自動生成されない
  // MoveConstructorDeleted(MoveConstructorDeleted && rhs) = default;
  // MoveConstructorDeleted & operator=(MoveConstructorDeleted && rhs) = default;

  void addMap(std::string key, HeavyCopyMove && value) { map[key] = std::move(value); }
};
