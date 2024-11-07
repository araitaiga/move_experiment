#include <array>
#include <iostream>
#include <vector>

#include "heavy_copy.h"
#include "stop_watch.h"

int main()
{
  std::cout << "=====Create HeavyCopy Array===== \n";
  std::array<HeavyCopy, 3> hoge_array;
  // HeavyCopyはムーブコンストラクタを持たないためムーブ不可
  // HeavyCopy val1(1);
  // HeavyCopy val2(2);
  // HeavyCopy val3(3);
  // hoge_array.at(0) = std::move(val1);
  // hoge_array.at(1) = std::move(val2);
  // hoge_array.at(2) = std::move(val3);

  // コピーのみ可能
  hoge_array.at(0) = HeavyCopy("1");
  hoge_array.at(1) = HeavyCopy("2");
  hoge_array.at(2) = HeavyCopy("3");

  StopWatch stop_watch;
  std::cout << "=====Copy HeavyCopy===== \n";
  stop_watch.reset();

  std::array<HeavyCopy, 3> copied_hoge(hoge_array);
  std::cout << "Copy Array: " << stop_watch.getDuration().count() << "ms\n";

  std::cout << "=====Try To Move HeavyCopy===== \n";
  stop_watch.reset();
  // arrayのムーブコンストラクタはarrayが管理する要素を１つずつムーブしようとするが,
  // 要素がムーブできない場合はコピーコンストラクタが呼ばれる
  std::array<HeavyCopy, 3> moved_hoge(std::move(hoge_array));
  std::cout << "Move Vector: " << stop_watch.getDuration().count() << "ms\n";

  std::cout << std::endl;
  std::cout << "=====Create HeavyCopyMove Array===== \n";
  // HeavyCopyMoveはムーブコンストラクタを持つためムーブ可能
  std::array<HeavyCopyMove, 3> hoge_move_array;
  // HeavyCopyMove(1)は右辺値なのでムーブされる
  hoge_move_array.at(0) = HeavyCopyMove("1");
  hoge_move_array.at(1) = HeavyCopyMove("2");
  hoge_move_array.at(2) = HeavyCopyMove("3");

  std::cout << "=====Move HeavyCopyMove===== \n";
  stop_watch.reset();
  // ここでも正しくムーブされる
  std::array<HeavyCopyMove, 3> moved_hoge_move(std::move(hoge_move_array));
  std::cout << "Move Vector: " << stop_watch.getDuration().count() << "ms\n";

  return 0;
}
