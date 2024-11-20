#include <iostream>
#include <vector>

#include "heavy_copy.h"
#include "stop_watch.h"

class MoveConstructorDeleted
{
  // 自動生成されるコピーコンストラクタが削除されたクラス
private:
  std::map<std::string, HeavyCopyMove> map;

public:
  // 1. デストラクタ, コピーコンストラクタ, コピー代入演算子は三位一体で定義すべき
  // 2. 上記のうち一つでも定義された場合, ムーブコンストラクタ, ムーブ代入演算子は自動生成されない
  // デストラクタを定義した場合, 過去の経緯からコピーコンストラクタ, コピー代入演算子は自動生成されるため,
  // このクラスをムーブしようとしてもコンパイルエラーにならずコピーされる
  // ~MoveConstructorDeleted() { std::cout << "CopyConstructorDeleted Destructor" << std::endl; };
  ~MoveConstructorDeleted() = default;  // defaultを指定してもムーブ演算は自動生成されない

  // コピー演算は自動生成される

  // ムーブ演算は自動生成されない
  // MoveConstructorDeleted(MoveConstructorDeleted && rhs) = default;
  // MoveConstructorDeleted & operator=(MoveConstructorDeleted && rhs) = default;

  void addMap(std::string key, HeavyCopyMove && value) { map[key] = std::move(value); }
};

int main()
{
  std::cout << "=====MoveConstructor is Deleted===== \n";
  MoveConstructorDeleted move_deleted_container;
  move_deleted_container.addMap("key1", HeavyCopyMove("1"));
  move_deleted_container.addMap("key2", HeavyCopyMove("2"));
  move_deleted_container.addMap("key3", HeavyCopyMove("3"));

  std::cout << "=====Copy===== \n";
  // コピーされる
  MoveConstructorDeleted move_deleted_container2 = std::move(move_deleted_container);

  return 0;
}
