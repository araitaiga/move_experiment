#include <array>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "heavy_copy.h"
#include "stop_watch.h"

class HeavyCopyMoveContainer
{
public:
  using HeavyCopyMoveArray = std::array<HeavyCopyMove, 10>;
  HeavyCopyMoveArray heavys_;

  explicit HeavyCopyMoveContainer(HeavyCopyMoveArray heavys) : heavys_(std::move(heavys))
  {
    std::cout << "HeavyCopyMoveContainer created\n";
  }
};

int main()
{
  // 外部で何らかの処理を行い, それをHeavyCopyMoveContainerに渡す
  StopWatch stop_watch;
  std::cout << "=====Create HeavyCopyMoveContainer===== \n";
  HeavyCopyMoveContainer::HeavyCopyMoveArray base_heavy_copy_moves;
  for (size_t i = 0; i < 10; i++) {
    base_heavy_copy_moves.at(i) = HeavyCopyMove(std::to_string(i));
  }

  std::cout << "=====Copy HeavyCopyMoveContainer===== \n";
  stop_watch.reset();
  // 左辺値を渡す場合, HeavyCopyMoveContainer内でコピーが発生
  HeavyCopyMoveContainer container1(base_heavy_copy_moves);
  std::cout << "Copy: " << stop_watch.getDuration().count() << "ms\n";

  std::cout << "=====Move HeavyCopyMoveContainer===== \n";
  stop_watch.reset();
  HeavyCopyMoveContainer container2(std::move(base_heavy_copy_moves));
  std::cout << "Move: " << stop_watch.getDuration().count() << "ms\n";

  std::cout << "=====Cannot Access Moved Object===== \n";
  for (size_t i = 0; i < 10; i++) {
    std::cout << base_heavy_copy_moves.at(i).get() << std::endl;
  }

  std::cout << "=====Can Access HeavyCopyMoveContainer===== \n";
  for (size_t i = 0; i < 10; i++) {
    std::cout << container2.heavys_.at(i).get() << std::endl;
  }

  return 0;
}
