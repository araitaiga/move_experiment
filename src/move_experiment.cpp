#include <chrono>
#include <iostream>
#include <thread>
#include <utility>
#include <vector>

class Fuga
{
public:
  Fuga(const size_t i) { std::cout << "Fuga created " << i << std::endl; };
  Fuga(const Fuga &)
  {
    std::cout << "Fuga start copy\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Fuga copied\n";
  }
};

// テンプレートで転送参照を利用したcalcFugasの定義
template <typename T>
std::vector<Fuga> calcFugasForward(T && fugas)
{
  fugas.emplace_back(fugas.size());
  std::cout << "calcFugasForward: modified fugas\n";
  return std::forward<T>(fugas);  // ムーブまたはコピーで返す
}

std::vector<Fuga> calcFugasLValueRef(const std::vector<Fuga> & fugas)
{
  std::vector<Fuga> result = fugas;
  result.emplace_back(fugas.size());
  std::cout << "calcFugasForward: modified fugas\n";
  return result;
}
std::vector<Fuga> calcFugasMove(std::vector<Fuga> && fugas)
{
  fugas.emplace_back(fugas.size());
  std::cout << "calcFugasForward: modified fugas\n";
  return std::move(fugas);
}

// RVO
// https://cpprefjp.github.io/lang/cpp17/guaranteed_copy_elision.html
std::vector<Fuga> calcFugasMoveRVO(std::vector<Fuga> && fugas)
{
  fugas.emplace_back(fugas.size());
  std::cout << "calcFugasForward: modified fugas\n";
  return fugas;
}

std::vector<Fuga> createFugas()
{
  std::vector<Fuga> fugas;
  // reserveしておかないとvectorの容量不足の際に要素の再確保が行われ、コピーが発生する
  // https://qiita.com/buyoh/items/be49053aebb06b14fe76
  fugas.reserve(4);
  fugas.emplace_back(fugas.size());
  fugas.emplace_back(fugas.size());
  fugas.emplace_back(fugas.size());
  std::cout << "[createFugas]\n";
  return fugas;
}

int main()
{
  std::cout << "=====Copy Forward===== \n";
  auto fugas1 = createFugas();
  // calcFugasの実行時間を計測
  auto start = std::chrono::system_clock::now();
  // 左辺値を渡す場合
  std::vector<Fuga> result1 = calcFugasForward(fugas1);
  auto end = std::chrono::system_clock::now();
  std::cout << "calcFugasForward: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";

  std::cout << "=====Move Forward===== \n";
  auto fugas2 = createFugas();
  start = std::chrono::system_clock::now();
  // 右辺値を渡す場合
  std::vector<Fuga> result2 = calcFugasForward(std::move(fugas2));
  end = std::chrono::system_clock::now();
  std::cout << "calcFugasForward: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";

  std::cout << "=====Copy LValueRef===== \n";
  auto fugas3 = createFugas();
  start = std::chrono::system_clock::now();
  // 左辺値を渡す場合
  std::vector<Fuga> result3 = calcFugasLValueRef(fugas3);
  end = std::chrono::system_clock::now();
  std::cout << "calcFugasLValueRef: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";

  std::cout << "=====Move LValueRef===== \n";
  auto fugas4 = createFugas();
  start = std::chrono::system_clock::now();
  // 右辺値を渡す場合
  std::vector<Fuga> result4 = calcFugasLValueRef(std::move(fugas4));
  end = std::chrono::system_clock::now();
  std::cout << "calcFugasLValueRef: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";

  std::cout << "=====Move RValueRef===== \n";
  auto fugas5 = createFugas();
  start = std::chrono::system_clock::now();
  // 右辺値を渡す場合
  std::vector<Fuga> result5 = calcFugasMove(std::move(fugas5));
  end = std::chrono::system_clock::now();
  std::cout << "calcFugasLValueRef: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";

  std::cout << "=====Move RValueRef RVO===== \n";
  auto fugas6 = createFugas();
  start = std::chrono::system_clock::now();
  // 右辺値を渡す場合
  std::vector<Fuga> result6 = calcFugasMoveRVO(std::move(fugas6));
  end = std::chrono::system_clock::now();
  std::cout << "calcFugasLValueRef: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";

  std::cout << "=====Result===== \n";
  std::cout << "main: result1 size = " << result1.size() << "\n";
  std::cout << "main: result2 size = " << result2.size() << "\n";
  std::cout << "main: result3 size = " << result3.size() << "\n";
  std::cout << "main: result4 size = " << result4.size() << "\n";

  return 0;
}
