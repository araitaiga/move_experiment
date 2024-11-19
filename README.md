# ムーブ

## の前にコピー

main関数ではHeavyCopyのコピーコンストラクタは何回呼ばれる?

```cpp
class HeavyCopy
{
  HeavyCopy(std::string str);
...
};

int main()
{
  std::vector<HeavyCopy> hoge{HeavyCopy("1"), HeavyCopy("2"), HeavyCopy("3")}; 
  std::vector<HeavyCopy> fuga(hoge);
  return 0;
}
```

<details>
<summary>正解</summary>
6回. hogeの初期化で3回, fugaへのコピーで3回.
</details>

</br>

以下のコードでは?

```cpp
int main()
{
  std::vector<HeavyCopy> hoge;
  hoge.reserve(3);
  hoge.emplace_back(1);
  hoge.emplace_back(2);
  hoge.emplace_back(3);
  std::vector<HeavyCopy> fuga(std::move(hoge));
  return 0;
}
```

<details>
<summary>正解</summary>
0回. move!
</details>

</br>

## コピーコンストラクタ

引数に同じクラスのconst左辺値参照を取り, メンバを"コピー"する  
引数のインスタンスは不変. (const &)  

```cpp
class HeavyCopy{
  char * ptr;
  HeavyCopy(const HeavyCopy & rhs)
  {
    ptr = new char[1000] ;
    std::copy(rhs.ptr, rhs.ptr + 1000, ptr);
  }
};
```

## ムーブコンストラクタ

引数に同じクラスの右辺値参照を取り, 引数のメンバの所有権を奪う  
引数のインスタンスは可変.  

```cpp
class HeavyCopyMove{
  char * ptr;
public:
  HeavyCopyMove(HeavyCopyMove && rhs)
  {
    // ポインタだけコピーする
    ptr = rhs.ptr;
    rhs.ptr = nullptr;
    return *this;
  }
};

```

以下すべての条件を満たす場合、ムーブ代入演算子と共に自動生成される  

- クラスがコピー演算を宣言していない  
- クラスがムーブ演算を宣言していない  
- クラスがデストラクタを宣言していない  

<https://cpprefjp.github.io/lang/cpp11/rvalue_ref_and_move_semantics.html>  
<https://theolizer.com/cpp-school1/cpp-school1-36/>

## 右辺値と左辺値

<https://cpprefjp.github.io/lang/cpp11/rvalue_ref_and_move_semantics.html>

```cpp

int main()
{
  int i = 0;

  i;      // 名前付きオブジェクトは左辺値
  0;      // リテラル値は右辺値

  Foo x ;

  x;      // 名前付きオブジェクトは左辺値
  Foo();  // コンストラクタの戻り値は右辺値

  f();    // 関数の戻り値は右辺値
}
```

## 右辺値参照

```T&&```. 右辺値のみを束縛する参照.  一方```T&```は左辺値参照.  
**関数やコンストラクタが, 受け取った引数が右辺値か左辺値かを区別するためのフラグとして用いられる**

```cpp
void operateHeavyCopyMove(HeavyCopyMove && obj)
{
  //  引数に右辺値が渡された場合, こちらが呼び出される
  std::cout << "operate with rvalue reference\n";
}
void operateHeavyCopyMove(const HeavyCopyMove & obj)
{
  //  引数に左辺値が渡された場合, こちらが呼び出される
  std::cout << "operate with lvalue reference\n";
}

int main()
{
  HeavyCopyMove base_obj("1");
  std::cout << "=====Operate with LValue Reference===== \n";
  operateHeavyCopyMove(base_obj);

  std::cout << "=====Operate with RValue Reference===== \n";
  operateHeavyCopyMove(HeavyCopyMove("2"));
  operateHeavyCopyMove(std::move(base_obj));

  return 0;
}

```

右辺値は一時オブジェクトのため自由に破棄して良く, 左辺値はそうではない.  
--> 関数やコンストラクタ側がムーブにより所有権を奪うためには右辺値が必要.  

## ムーブ

std::move(X)は実際には**ムーブしない**. 左辺値Xを右辺値にキャストするだけ.  
実際のムーブはムーブコンストラクタ, ムーブ代入演算子により行われる.  

```cpp
int main()
{
  std::string x = "Hello, world!";
  // 何も起こらない
  std::move(x);

  // 実際にxからyへ文字列がムーブされる
  std::string y = std::move(x);
}
```

## コピー or ムーブ ?

以下のコードで行われるのはコピー or ムーブ?

```cpp

class HeavyCopy
{
  // Copyに時間のかかるクラス
private:
  std::string str{0};

public:
  HeavyCopy() = default;
  explicit HeavyCopy(const std::string str) : str(str)
  {
    std::cout << "HeavyCopy created " << str << std::endl;
  };
  HeavyCopy(const HeavyCopy & rhs) : str(rhs.str)
  {
    std::cout << "HeavyCopy start copy " << rhs.str << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "HeavyCopy copied" << std::endl;
  }
};
int main(){
  std::array<HeavyCopy, 3> hoge_array;
  HeavyCopy val1(1);
  HeavyCopy val2(2);
  HeavyCopy val3(3);
  hoge_array.at(0) = std::move(val1);
  hoge_array.at(1) = std::move(val2);
  hoge_array.at(2) = std::move(val3);

  return 0;
}
```

<details>
<summary>正解</summary>
copy. ムーブコンストラクタが定義されていないので, 右辺値を代入していてもムーブ代入演算子は呼ばれない
</details>

</br>

## ムーブのつかいどころ

- <https://theolizer.com/cpp-school1/cpp-school1-36/>  
  - > 通常通りクラス型の変数を定義する際に、例えば他の関数の戻り値と同じものをコンストラクトする時が典型的な使い方と思います。関数の戻り値ですから一時オブジェクト（右辺値）となるため、その文が終わると解放されます。ならば、折角、関数で作られたデータをなるべく有効活用するため、ムーブしたいです。  
  - memo: RVOと矛盾...?  

- <https://theolizer.com/cpp-school1/cpp-school1-37/>  
  - 要約
  - > RAIIパターンのクラス(リソースの確保と値の初期化を同時に行うもの. unique_ptrなど) はコピー不可能. ムーブを使えばstd::vectorで管理できるようになる  
  - > 所有権が一つのオブジェクトにのみ割り当てられるようなクラス（ファイルをオープンしたときのハンドラ, unique_ptrなど）をムーブ対応しておく（ムーブコンストラクタを書いておく）と、必要なときに所有権を移動できて便利

ムーブ演算を持つクラスはstd::vectorで管理してメモリの再確保が行われた際にコピーではなくムーブされるのでお得

```cpp

int main()
{
  // HeavyCopyのコピーコンストラクタが3回呼ばれる
  std::vector<HeavyCopy> hoge_copy1{HeavyCopy("1"), HeavyCopy("2"), HeavyCopy("3")};

  // HeavyCopyはムーブコンストラクタを持たない
  // メモリの再確保が発生するため, コピーコンストラクタが呼ばれる
  std::vector<HeavyCopy> hoge_copy2;
  hoge_copy2.emplace_back("1");
  hoge_copy2.emplace_back("2");
  hoge_copy2.emplace_back("3");

  // HeavyCopyMoveはnoexceptなムーブコンストラクタを持つ
  // メモリの再確保の際にムーブコンストラクタが呼ばれる
  // https://qiita.com/kei10in/items/00f65e68a3d08093aceb
  std::vector<HeavyCopyMove> hoge_copy3;
  hoge_copy3.emplace_back("1");
  hoge_copy3.emplace_back("2");
  hoge_copy3.emplace_back("3");

  return 0;
}
```

std::vectorのような, リソースを受け取って管理するクラスを自作する際にムーブが必要  
<https://cpprefjp.github.io/reference/vector/vector/push_back.html>  

std::vector::push_backでも右辺値参照引数でオーバーロードされている  
```cpp
void push_back(const T& x);           // (1) C++03
void push_back(T&& x);                // (2) C++11
```

```cpp
HeavyCopyMove heavy_copy_move("4");
std::vector<HeavyCopyMove> hoge_copy4;
// push_backでもmoveコンストラクタが呼ばれる
hoge_copy4.push_back(std::move(heavy_copy_move));
```

---
## 自動生成される特殊メンバ関数
コンストラクタ, デストラクタ, コピーコンストラクタ/コピー代入演算子, ムーブコンストラクタ/ムーブ代入演算子と共に自動生成される


## RVO

## std::forwardと完全転送
