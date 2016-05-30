// 条款21： 必须返回对象时，别妄想返回其reference
// 绝不要返回pointer或reference指向一个local stack对象，会返回reference指向
// 一个heap-allocated对象，或返回pointer或reference指向一个local static对象
// 而有可能同时需要多个这样的对象。条款4已经为“在单线程环境中合理返回reference
// 指向一个local static对象”提供了一份设计实例。

#include <iostream>

// 由于条款20的建议pass by reference效率高，避免了构造函数导致的效率问题。
// 但进行函数返回时，也用pass by reference来提高效率，就很可能出现问题。

class Rational21 {
public:
	int a;
};

// 返回对象糟糕代码一： 返回对象时用指针或者引用指向local变量
// 这种做法不仅没有避免构造函数，而且问题严重的是result是一个local变量，在函数退出
// 前就被销毁了。返回的是一个残骸
// 事实的真相就是：任何函数如果返回一个reference指向local对象，都将失败。如果函数返回
// 指针指向一个local对象，后果也是一样。
const Rational21& operator* (const Rational21& r1, const Rational21& r2) {
	Rational21 result;
	return result;
}

// 返回对象糟糕代码二：返回对象时用指针或者引用指向heap变量
// 付出了构造函数调用的代价，而且new 出来的对象谁来delete？
// 如果出现x + y + z；这样的代码，这条语句调用了两次operator+则new出了两个对象，
// 但是没有什么合理的办法来获得reference背后隐藏的那个指针。
const Rational21& operator+ (const Rational21& r1, const Rational21& r2) {
	Rational21 *r = new Rational21();
	return *r;
}

// 返回对象糟糕代码三： 返回对象时用指针或者引用指向static变量
// 同样付出了构造函数的代价，而且static是共享变量，在函数中也是这样，
// 多次调用这个函数，操作的都是同一个static变量，多线程的安全性令人疑虑。
const Rational21& operator-(const Rational21& r1, const Rational21& r2) {
	static Rational21 result;
	// 对r2的一些操作
	return result;
}
// 可能导致如下严重问题的代码
bool operator== (const Rational21& r1, const Rational21& r2) {
	if (r1.a == r2.a) {
		return true;
	}
	return false;
}
// 这个函数永远返回true，因为调用了两侧operator-，而这两次返回的是同一个变量，
// 返回的都是static创建出来的变量，前一次计算出一个结果会被后一次调用把结果冲掉，
// 又因为返回的是引用所以==比较的就是引用指向的同一个对象，如果是返回的是直接的
// 对象，那么结果会是对的。
bool Terrible() {
	Rational21 a, b, c, d;
	if ((a - b) == (c - d)) {
		return true;
	}
	return false;
}

// 如下形式的代码最好,当然要付出构造成本和析构成本
inline const Rational21 operator/ (const Rational21& r1, const Rational21& r2) {
	return Rational21();
}
