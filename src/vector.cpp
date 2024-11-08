#include <iostream>
#include <vector>

#include "heavy_copy.h"
#include "stop_watch.h"

int main()
{
  std::cout << "=====Copy Constructor1===== \n";
  // HeavyCopyのコピーコンストラクタが3回呼ばれる
  std::vector<HeavyCopy> hoge_copy1{HeavyCopy("1"), HeavyCopy("2"), HeavyCopy("3")};

  std::cout << "=====Copy Constructor2===== \n";
  // HeavyCopyはムーブコンストラクタを持たない
  // メモリの再確保が発生するため, コピーコンストラクタが呼ばれる
  std::vector<HeavyCopy> hoge_copy2;
  hoge_copy2.emplace_back("1");
  hoge_copy2.emplace_back("2");
  hoge_copy2.emplace_back("3");

  std::cout << "=====Move Constructor===== \n";
  // HeavyCopyMoveはnoexceptなムーブコンストラクタを持つ
  // メモリの再確保の際にムーブコンストラクタが呼ばれる
  // https://qiita.com/kei10in/items/00f65e68a3d08093aceb
  std::vector<HeavyCopyMove> hoge_copy3;
  hoge_copy3.emplace_back("1");
  hoge_copy3.emplace_back("2");
  hoge_copy3.emplace_back("3");

  HeavyCopyMove heavy_copy_move("4");
  std::vector<HeavyCopyMove> hoge_copy4;
  // push_backでもmoveコンストラクタが呼ばれる
  // https://cpprefjp.github.io/reference/vector/vector/push_back.html
  hoge_copy4.push_back(std::move(heavy_copy_move));

  std::cout << "=====Create Base Object===== \n";
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
