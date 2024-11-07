#include <iostream>
#include <vector>

#include "heavy_copy.h"
#include "stop_watch.h"

int main()
{
  // std::vector<HeavyCopy> hoge{HeavyCopy("1"), HeavyCopy("2"), HeavyCopy("3")}; // HeavyCopyのコピーコンストラクタが3回呼ばれる
  std::vector<HeavyCopy> hoge;
  hoge.reserve(3);
  hoge.emplace_back("1");
  hoge.emplace_back("2");
  hoge.emplace_back("3");


  StopWatch stop_watch;
  std::cout << "=====Copy===== \n";
  stop_watch.reset();

  // コピーコンストラクタが呼ばれる
  std::vector<HeavyCopy> copied_hoge(hoge);
  std::cout << "Copy Vector: " << stop_watch.getDuration().count() << "ms\n";

  std::cout << "=====Move===== \n";
  stop_watch.reset();
  // vectorのムーブコンストラクタはvectorが管理する要素へのポインタをコピーするだけなので,
  // HeavyCopyのコピーコンストラクタは呼ばれない
  std::vector<HeavyCopy> moved_hoge(std::move(hoge));
  std::cout << "Move Vector: " << stop_watch.getDuration().count() << "ms\n";

  return 0;
}
