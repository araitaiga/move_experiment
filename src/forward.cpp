#include <iostream>
#include <vector>

#include "heavy_copy.h"
#include "stop_watch.h"

class ForwardSample
{
  // 自動生成されるコピーコンストラクタが削除されたクラス
private:
  std::map<std::string, HeavyCopyMove> map;

public:
  ForwardSample() { std::cout << "CopyConstructorDeleted Constructor" << std::endl; };

  // void addMap(std::string key, HeavyCopyMove && value) { map[key] = std::move(value); }

  template <typename T>
  void addMap(std::string key, T && value)
  {
    map[key] = std::forward<T>(value);
  }
};

int main()
{
  std::cout << "=====MoveConstructor is Deleted===== \n";
  ForwardSample forward_sample;
  forward_sample.addMap("key1", HeavyCopyMove("1"));

  // // addMapの引数を右辺値参照とした場合,
  // // 以下はコンパイルエラー
  // const auto forward_sample_value = HeavyCopyMove("2");
  // forward_sample.addMap("key2", forward_sample_value);
  // forward_sample.addMap("key2", std::move(forward_sample_value));

  // // 以下はOK
  // auto forward_sample_value = HeavyCopyMove("2");
  // forward_sample.addMap("key2", std::move(forward_sample_value));

  // addMapの引数をテンプレートとしてstd::forwardを使用した場合,
  // 以下はOK
  const auto forward_sample_value = HeavyCopyMove("3");
  forward_sample.addMap("key3", forward_sample_value);
  forward_sample.addMap("key4", HeavyCopyMove("4"));

  return 0;
}
