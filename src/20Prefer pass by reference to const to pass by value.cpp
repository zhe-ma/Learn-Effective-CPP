// 条款20： 宁以pass-by-reference-to-const替换pass-by-value
// 1.尽量以pass-by-reference-to-const替换pass-by-value。前者通常比较高效，可避免切割问题。
// 2.以上规则并不适用于内置类型，以及STL的迭代器和函数对象。对它们而言，pass-by-value往往比较恰当。

#include <iostream>
#include <string>

// 缺省状况下C++以值方式传递对象时，传递的是对象的副本，系由copy构造函数产出，可能使得以值传递
// 成为费时操作。

class Face20 {
public:
	Face20(const Face20& f) {
		std::cout << "I am Face" << std::endl;
	}
	Face20() {
	}
};

class Person20 {
public: 
	Person20(const Person20& p)
		: face(p.face) { 
		std::cout << "I am Person" << std::endl;
	}
	Person20() {
	}
	virtual void Name() const {
		std::cout << "My name is Person20" << std::endl;
	}

private:
	Face20 face;
};

class Student20 : public Person20 {
public:
	Student20() {
	}
	Student20(const Student20& s) {
		std::cout << "I am Student" << std::endl;
	}
	virtual void Name() const {
		std::cout << "My name is Student20" << std::endl;
	}
};

// 测试以值传递的耗时性
// I am Student
// I am Face
// I am Person
// 以值传递参数的传递成本就是多次调用构造函数，最后调用析构函数
void TestStudent20(Student20 s) {
}

// 测试以值传递的切割性
// My name is Person20
// 以值传递时，参数p会被构造成一个Person20的对象，p原本的特化信息被切割。导致多态不能形成。
void TestStudent202(Person20 p) {
	p.Name();
}

// 测试引用传递的完整性，可以实现多态
// My name is Student20
void TestStudent203(const Person20& p) {
	p.Name();
}

int main21() {
	Student20 s;
	TestStudent20(s);
	TestStudent202(s);
	TestStudent203(s);
	//输出为：
	// I am Student
	// I am Face
	// I am Person
	// ------------
	// My name is Person20
	// ------------
	// My name is Student20

	return 0;
}
