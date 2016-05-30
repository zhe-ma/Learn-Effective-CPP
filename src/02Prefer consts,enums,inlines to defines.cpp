// 条款02： 尽量以const，enum，inline 替换#define
// 尽可能的使用编译器而不是预处理器

#include <iostream>

// 1. 定义class专属常量（用const代替define）
//   A. 不能用#define，因为define不重视作用域（scope）
//   B. 用const，static修饰。用const因为不能被改变，用static因为所有的对象共享这一个变量
//   C. 在class中const，static修饰的int类型需特殊处理，可以在类中赋值，但是这只是声明而不是定义，
//      可以取值使用，在取地址时，最好还是要外部定义。
//   D. 定义类中常量的方法是，类外面定义，类里面声明。

// 2. 用enum代替define
//   理论基础： 一个枚举类型的数值可以充当int类型被使用

// 3. 对于形似函数的宏，用inline函数来代替#define

class Circle {
private:
	enum {
		Num = 5  // 枚举类型被默认为int类型
	};

public:
	static const int A = 5;  // 正确，这只是常量声明式，只用取地址时c++编译器才会给它开辟内存
  double a = 5;  // 正确，说明c++ 中成员变量可以直接赋值
	//static const double Pi = 3.14; //错误 
	//错误:: a static data member with an in-class initializer must have non-volatile const integral type
	static const double Pi;  // 声明常量
	int array[Num];  // 声明数组
};

const double Circle::Pi = 3.14;  // 定义常量

int main02() {
	Circle *c1 = new Circle();
	std::cout << Circle::A << c1->A << std::endl;  // 正确，可以访问
	std::cout << &Circle::A << std::endl;  // 正确
	std::cout << c1->a << std::endl;  // 正确
	std::cout << c1->Pi << std::endl;  //正确
	std::cout << Circle::Pi << std::endl;  // 正确
	//c1->Pi = c1->Pi + 1;  // 错误
	return 0;
}