#include <iostream>
#include <utility>
#include <vector>

#include "heavy_copy.h"
#include "stop_watch.h"

// RVO
// https://cpprefjp.github.io/lang/cpp17/guaranteed_copy_elision.html
std::vector<HeavyCopyMove> testRVO()
{
  std::vector<HeavyCopyMove> heavy_array;
  // reserveしておかないとvectorの容量不足の際に要素の再確保が行われ、コピーが発生する
  // https://qiita.com/buyoh/items/be49053aebb06b14fe76
  heavy_array.reserve(3);
  heavy_array.emplace_back("1");
  heavy_array.emplace_back("2");
  heavy_array.emplace_back("3");
  std::cout << "testRVO\n";
  return heavy_array;
}

int main()
{
  std::cout << "=====RVO===== \n";
  StopWatch stop_watch;
  // resultの初期化時にコピーが省略される
  std::vector<HeavyCopyMove> result = testRVO();

  std::cout << "testRVO: " << stop_watch.getDuration().count() << "ms\n";

  return 0;
}
