// 条款06： 若不想使用编译器自动生成的函数，就该明确拒绝。

#include <iostream>

// 防止编译器自动生成copy构造函数和copy assignment操作符的两种方法：
// 1.将这两个函数声明为private，并且不实现它们。如果有人调用了它们在链接期会得到一个链接错误。
// 2.专门设计一个阻止copying动作的base class，然后继承它。这样在编译期间就能发现错误。

class HomeForSale1 {
public:
	HomeForSale1() {};
	~HomeForSale1() {};

private:
	HomeForSale1(const HomeForSale1&);
	HomeForSale1& operator=(const HomeForSale1&);
};

class Uncopyable {
protected:
	Uncopyable() {};  // 允许derive的对象构造和析构
	~Uncopyable() {};

private:
	Uncopyable(const Uncopyable&);
	Uncopyable& operator=(const Uncopyable&);
};

class HomeForSale2 : public Uncopyable {
	// 该class不在私有声明那两个函数
};

int main06() {
	HomeForSale1 h1;
	//HomeForSale1 h2(h1);  // 错误
	//HomeForSale1 h2 = h1; // 错误

	HomeForSale2 h3;
	//HomeForSale2 h4(h3);  // 报错cannot access private member declared in class 'Uncopyable'
  //HomeForSale2 h4 = h3;  // 报错
	return 0;
}