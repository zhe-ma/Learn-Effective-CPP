// 条款16： 成对使用new和delete时要采用相同的形式
// 1.如果在new表达式中使用[],必须在相应的delete表达式中使用[]。如果
// 在new表达式中不使用[]，一定不要再相应的delete表达式中使用[]。
// 2.new一个对象会有两个行为，第一个是内存被分配出来，第二是针对此内存会有一个
// 或多个构造函数被调用。
// 3.delete一个对象也会有两个行为，第一个是针对此内存会有一个或多个析构函数被调用，
// 第二个是内存被释放。

#include <iostream>
#include <string>

class MyString16 {
public:
	MyString16() {
		count_++;
		std::cout << "construct" << count_ << std::endl;
	}
	~MyString16() {
		std::cout << "destruct" << count_ << std::endl;
		count_--;
	}

public:
	static int count_;
};

int MyString16::count_ = 0;

int main16() {
	MyString16* ptr1 = new MyString16;
	MyString16* ptr2 = new MyString16[10];
	delete ptr1;  // 删除一个对象
	delete [] ptr2;  // 删除一个对象数组

	// 错误用法，未知结果，导致系统崩溃
	MyString16* ptr3 = new MyString16;
	MyString16* ptr4 = new MyString16[10];
	delete ptr4;
	delete [] ptr3;

	return 0;
}