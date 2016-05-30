// 条款04： 确定对象被使用前已被初始化

#include <iostream>
#include <string>
#include <list>

using std::string;
using std::cout;
using std::endl;
using std::list;

class Point04 {
public: 
	int a;
	int b;
};

// 1.不要混淆了赋值(assignment)和初始化(initialization)
// C++规定，对象成员变量的初始化动作发生在进入构造函数本体之前。
// 编译器会为用户自定义类型的成员变量自动调用default构造函数，
// 也就是说在进入ABEntry构造函数本体时间之前，这些自定义类型的成员得default构造函数
// 已经被调用。但是对于内置类型，不能保证一定在赋值动作之前获得初值。
class PhoneNumber {
};

class ABEntry {
public:
	ABEntry(string& name, string& address, list<PhoneNumber> phones);

private:
	string m_name;
	string m_address;
	list<PhoneNumber> m_phones;
	int m_num_consulted; 
};

// 赋值
// 先调用default构造函数，然后调用copy assignment操作符
// ABEntry::ABEntry(string& name, string& address, list<PhoneNumber> phones) {
//   m_name = name;  // 这些都是赋值，不是对成员变量的初始化！！
//   m_address = address;  // 这样的做法导致首先调用default构造函数为m_name,m_address,m_phones
//   m_phones = phones;  // 设初值，然后立即进入函数体再对它们赋予新值。
// }

// 初始化（用成员列表初始化）
// 只用了一次copy构造函数
// 这个做法效率更高，因为初始列中针对各个成员变量设的实参,
// 被拿去作为各成员变量的构造函数的实参,调用构造函数时已经对各个变量进行了copy初始化
// 对于内置类型例如本例中的int类型，赋值和初始化成本相同，为了一致性，也在成员列表初始化
// 但是对于内置类型，如果其为const或者reference，那么它们就不能被赋值，需要初值，所以要用初始化列表
ABEntry::ABEntry(string& name, string& address, list<PhoneNumber> phones)
		: m_name(name),  // 这些都是初始化，构造函数本体不必有任何动作
			m_address(address),  // m_name以name为初值进行拷贝构造，等等
			m_phones(phones),
			m_num_consulted(0) {  
}                        

// 2.总是使用成员列表进行初始化，这样做有时候绝对必要，且往往比赋值更高效

// 3.C++有着十分固定的成员初始化次序，基类更早的初始化，成员变量以其声明的次序被初始化。

// 4.为内置类型对象进行手工初始化，因为C++不保证初始化它们。

// 5.为免除“跨编译单元只初始化次序”问题，请以local static对象替换non-local

int main04() {
	// Point04 p1;
	// cout << p1.a << "&" << endl;  // 错误 ：uninitialized local variable 'p1' used
 
	return 0;
}