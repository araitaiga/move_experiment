#include <iostream>
#include <vector>

#include "heavy_copy.h"
#include "stop_watch.h"

int main()
{
  std::cout << "=====MoveConstructor is Deleted===== \n";
  MoveConstructorDeleted heavy_copy;
  heavy_copy.addMap("key1", HeavyCopyMove("1"));
  heavy_copy.addMap("key2", HeavyCopyMove("2"));
  heavy_copy.addMap("key3", HeavyCopyMove("3"));

  // コピーされる
  MoveConstructorDeleted heavy_copy2 = std::move(heavy_copy);

  return 0;
}
