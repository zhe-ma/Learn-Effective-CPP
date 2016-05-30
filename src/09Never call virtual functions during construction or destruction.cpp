// 条款09： 绝不在构造和析构过程中调用virtual函数。

#include <iostream>
#include <string>

using namespace std;

// 1.例如一个对象在创建和销毁的时候必须要执行另一个行为。
// 这样做的目的是base class 中的LogTransaction作为Virtual，子类中只需要重写这个函数，那么就能
// 在每个子类创建对象的时候都能调用这个函数，因为基类的构造和析构函数里面调用了这个函数。
// 充分运用了多态的原理。
// 这是这样的做法并不可行。
// 错误的原因：
// 从逻辑上分析：base class得构造函数在derived class之前执行。base class构造函数
// 执行时derived class的成员变量还没有初始化，如果此时virtual函数调用的是derived 
// class里面的，那么就十分危险，因为该函数里面很可能要用到derived class里面的成员变量。
// C++编译器是不会这样做危险事情的。
// 根本原因是：derived class对象在base class构造期间，对象的类型时base class而不是derived
// class。virtual函数和运行期类型信息都会被编译器解析为base class。这样做的思想是：
// 对于derived class对象中的专属成分尚未被初始化，面对它们最安全的做法就是视它们为
// 不存在。derived class构造函数开始执行前不会成为一个derived class对象。
// 道理同样适用于析构函数。进入base class析构函数后对象就成为一个base class对象。
class Transaction {
public:
	Transaction() {
		LogTransaction();
	}
	~Transaction() {
		LogTransaction();
	}
	virtual void LogTransaction() {
		cout << "I am base class" << endl;
	}
};

class BuyTransaction : public Transaction {
public:
	BuyTransaction() {
	}
	~BuyTransaction() {
	}
	virtual void LogTransaction() override {
		cout << "I am derived class" << endl;
	}
};

// 上述问题的解决方案，其实可以用给基类传递参数的方法来解决。
class Transaction2 {
public:
	Transaction2(const string log_info) {
		LogTransaction(log_info);  // non-virtual不使用多态
	}
	void LogTransaction(const string log_info) {  // non-virtual 函数
		cout << log_info << endl;
	}
};

class BuyTransaction2 : public Transaction2 {
public:
	BuyTransaction2(string info) : Transaction2(CreateLogInfo(info)) {  // 将log信息传给基类
	}
	~BuyTransaction2() {
	}
private:
	// 构造了一个辅助函数来帮助成员列表初始化基类，这样做可读性更好，并且更加灵活，可以进行其它行为。
	// 声明为static则避免了该函数调用时使用了子类还没有初始化好的对象。因为static是和类一个级别的。
	static string CreateLogInfo(string info) {
		return info;
	}
};


int main09() {
	{
		BuyTransaction b;
	}
	// 输出为：
	// I am base class
	// I am base class
	// 理想中的输出为：
	// I am derived clss
	// I am derived clss
	// 因为运用了多态，所以认为应该这样输出。

	{
		BuyTransaction2 b2("I am derived");
	}

	return 0;
}
