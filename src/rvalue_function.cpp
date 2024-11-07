#include <chrono>
#include <iostream>
#include <thread>
#include <utility>
#include <vector>

#include "heavy_copy.h"
#include "stop_watch.h"

void operateHeavyCopyMove(HeavyCopyMove && obj)
{
  //  引数に右辺値が渡された場合, こちらが呼び出される
  std::cout << "operate with rvalue reference\n";
}
void operateHeavyCopyMove(const HeavyCopyMove & obj)
{
  //  引数に左辺値が渡された場合, こちらが呼び出される
  std::cout << "operate with lvalue reference\n";
}

int main()
{
  HeavyCopyMove base_obj("1");
  std::cout << "=====Operate with LValue Reference===== \n";
  operateHeavyCopyMove(base_obj);

  std::cout << "=====Operate with RValue Reference===== \n";
  operateHeavyCopyMove(HeavyCopyMove("2"));
  operateHeavyCopyMove(std::move(base_obj));

  return 0;
}
