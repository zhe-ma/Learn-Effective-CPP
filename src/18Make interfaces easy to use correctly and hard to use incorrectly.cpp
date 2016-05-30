// 条款18： 让接口容易被正确使用，不易被误用
// 1.好的接口很容易被使用，不容易被误用。应该在所有的接口中努力达成这些性质。
// 2.“促进正确使用”的办法包括接口的一致性，以及与内置类型的行为兼容。
// 3.“阻止误用”的办法包括建立新类型、限制类型上的操作，数据对象值，以及消除客户的资源管理责任。
// 4.tr1::shared_ptr支持定制型删除器。这可防范DLL问题，可被用来自动解除互斥锁。

#include <iostream>
#include <memory>

struct Day {
	explicit Day(int d)
		: val(d) {
	}
	int val;
};

struct Year {
	explicit Year(int y)
		: val(y) {
	}
	int val;
};

class Month {
public:
	static Month Jan() {
		return Month(1);  // 函数返回有效月份
	}
	static Month Feb() {
		return Month(2);
	}

private:
	explicit Month(int m);  // 阻止生成新的月份
};

class Date {
public:
	Date(const Month& m, const Day& d, const Year& y) {

	}
};

int main18() {
	// 理想上，如果客户企图使用某个接口而却没有获得他预期的行为，这个代码就不该通过编译，
	// 如果代码通过了编译，它的作为就是该客户想要的。
	Date d(Month::Feb(), Day(12), Year(2012));
	// Date d(Month::Feb(), Day(12.1), Year(2012));  // 编译不通过，声明了explicit，不能隐式转换类型
	// Date d2(2, Day(2), Year(12));  // 编译不通过

	return 0;
}
