#include <iostream>
#include <vector>

#include "heavy_copy.h"
#include "stop_watch.h"

class ForwardSample
{
private:
  std::vector<HeavyCopyMove> vec;
  std::vector<HeavyCopyMove> vec2;

public:
  ForwardSample()
  {
    std::cout << "ForwardSample Constructor" << std::endl;
    vec.reserve(100);
    vec2.reserve(100);
  };

  // テンプレートとforwardを使うと一つの関数定義でOK
  // 引数は(右,右), (左,右), (右,左), (左,左)の4パターンあるが, 適切にムーブ/コピーされる
  template <typename T, typename U>
  void add(T && value, U && value2)
  {
    vec.push_back(std::forward<T>(value));
    vec2.push_back(std::forward<U>(value2));
  }

  // // forwardを使わないと4種類のオーバーロードが必要
  // void add(HeavyCopyMove && value, HeavyCopyMove && value2)
  // {
  //   std::cout << "add(HeavyCopyMove && value, HeavyCopyMove && value2)\n";
  //   vec.push_back(std::move(value));
  //   vec2.push_back(std::move(value2));
  // }
  // void add(const HeavyCopyMove & value, const HeavyCopyMove & value2)
  // {
  //   std::cout << "add(const HeavyCopyMove & value, const HeavyCopyMove & value2)\n";
  //   vec.push_back(value);
  //   vec2.push_back(value2);
  // }
  // // 以下２つは無くてもコンパイルエラーにならないが, 上のconst LValueの定義が呼ばれるため適切にムーブされずコピーになる
  // void add(HeavyCopyMove && value, const HeavyCopyMove & value2)
  // {
  //   std::cout << "add(HeavyCopyMove && value, const HeavyCopyMove & value2)\n";
  //   vec.push_back(std::move(value));
  //   vec2.push_back(value2);
  // }
  // void add(const HeavyCopyMove & value, HeavyCopyMove && value2)
  // {
  //   std::cout << "add(const HeavyCopyMove & value, HeavyCopyMove && value2)\n";
  //   vec.push_back(value);
  //   vec2.push_back(std::move(value2));
  // }
};

int main()
{
  std::cout << "=====ForwardSample===== \n";

  ForwardSample dont_use_forward_sample;
  std::cout << "=====Rvalue, Rvalue===== \n";
  dont_use_forward_sample.add(HeavyCopyMove("1"), HeavyCopyMove("2"));
  const auto dont_use_forward_sample_value = HeavyCopyMove("3");
  std::cout << "=====Lvalue, Rvalue===== \n";
  dont_use_forward_sample.add(dont_use_forward_sample_value, HeavyCopyMove("4"));
  std::cout << "=====Rvalue, Lvalue===== \n";
  dont_use_forward_sample.add(HeavyCopyMove("5"), dont_use_forward_sample_value);
  const auto dont_use_forward_sample_value2 = HeavyCopyMove("6");
  std::cout << "=====Lvalue, Lvalue===== \n";
  dont_use_forward_sample.add(dont_use_forward_sample_value, dont_use_forward_sample_value2);

  return 0;
}
