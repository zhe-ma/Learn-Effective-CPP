// 条款05： 了解C++默默编写并调用哪些函数
// 编译器可以暗自为class创建default构造函数，copy构造函数，copy assignment操作符，以及析构函数
// 唯有当这些函数被调用，它们才会被编译器创建出来

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

// 1.编译器产出的析构函数是non-virtual的
// 2.如果class中声明了一个构造函数，那么编译器就不会在为其创建default构造函数
// 3.如果class中声明了有参或者无参构造函数，那么编译器可能会为其创建copy构造函数
// 4.编译器自动产生的copy assignment行为基本和自动产生的copy构造函数一样
// 5.编译器自动生成copy assignment操作符后，可能拒绝编译某个赋值动作，比如这个类
// 内含reference成员，因为C++不允许reference指向不同的对象，只能初始化时指定一个；
// 或者内含const成员，因为更改const成员是不合法的。
// 还有一种情况父类将copy assignment声明为private，编译器拒绝为其子类生成一个copy assignment，
// 因为子类不能处理父类的class成分。

class Student05 {
public:
	Student05(char* name, const string& address, int age)
		: m_age(age), m_pname(name), m_address(address) {
	}

public:
	int m_age;
	char* m_pname;
	string m_address;
};

class Student052 {
public:
	Student052(const char* name, string& address)
		: m_pname(name), m_address(address) {
	}

public:
	const char* m_pname;
	string& m_address;
};

class Empty {
};

int main() {
	Empty e1;  //  编译器创建了default构造函数: Empty(){}
	Empty e2(e1);  // 编译器创建了copy构造函数: Empty(const Empty& e){}
	e2 = e1;  // 编译器创建了copy assignment操作符： Empty& operator=(const Empty& e) {}
						// 编译器创建了析构函数：~Empty(){}

	char* test_str = "aaaa";
	//test_str[1] = '1';  // 崩溃，因为"aaaa"为字符串常量，放在了常量存储区，只使用指针test_str来指向它了
	char test_arr[] = "abcde";
	test_arr[1] = 'a';  // 正确

	{
	  char name[] = "Jack.";
		//Student05 s1("Jack.", "road1.", 18);  // 这个“Jack”放在了常量存储区，所以对其任何修改都是错误的。
		Student05 s1(name, "road1.", 18);
		Student05 s2(s1);  // 编译器为其生成copy构造函数
		cout << "s1: " << s1.m_pname << s1.m_address << s1.m_age << endl;  
		cout << "s2: " << s2.m_pname << s2.m_address << s2.m_age << endl;   
		(s2.m_pname)[0] = 'P';
		s2.m_address = "road2.";
		s2.m_age = 81;
		cout << "after:" << endl;
		cout << "s1: " << s1.m_pname << s1.m_address << s1.m_age << endl;   
		cout << "s2: " << s2.m_pname << s2.m_address << s2.m_age << endl;
		// 上述输出为：
		//  s1: Jack.road1.18
		//  s2: Jack.road1.18
		//  after :
		//  s1 : Pack.road1.18
		//  s2 : Pack.road2.81
		// 得到结论，编译器创建出的copy构造函数，如果是内置类型，就简单的赋值，例如age，name，都是直接复制，
		// s1的name的值之所以会跟着s2的变化，因为s1和s2同时指向了一块内存。如果是class类型，那么就会调用
		// 该类的copy构造函数，例如s2的address是调用string类型的copy构造函数，将s1的address作为实参来进行
		// 初始化的。
	}

	{
		char name[] = "Jack.";
		Student05 s1(name, "road1.", 18);
		Student05 s2(name, "road2.", 19);
		s2 = s1;  // 编译器为其生成copy assignment操作符
		cout << "s1: " << s1.m_pname << s1.m_address << s1.m_age << endl;
		cout << "s2: " << s2.m_pname << s2.m_address << s2.m_age << endl;
		(s2.m_pname)[0] = 'P';
		s2.m_address = "road2.";
		s2.m_age = 81;
		cout << "after:" << endl;
		cout << "s1: " << s1.m_pname << s1.m_address << s1.m_age << endl;
		cout << "s2: " << s2.m_pname << s2.m_address << s2.m_age << endl;
		// 上述输出为：
		//  s1: Jack.road1.18
		//  s2: Jack.road1.18
		//  after :
		//  s1 : Pack.road1.18
		//  s2 : Pack.road2.81
	}

	{
		string address = "road";
		const char name[] = "Jack";
		Student052 s1(name, address);
		Student052 s2(name, address);
		// s1 = s2;  // 错误编译器拒绝编译这一行
		// note: compiler has generated 'Student052::operator =' here
		// error C2280: 'Student052 &Student052::operator =(const Student052 &)':
		// attempting to reference a deleted function
	}

	return 0;
}