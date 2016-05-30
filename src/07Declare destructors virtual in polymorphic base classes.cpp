// 条款07： 为多态基类声明virtual析构函数

// 1.C++明确指出，当子类对象经由一个基类指针删除，而该基类带着一个non-virtual析构函数，
// 其结果未有定义。实际执行时通常发生的是对象的derived成分没有被销毁。也就是不光子类
// 里面的成员变量可能没被销毁，而子类的析构函数也未能执行起来。而基类成分通常会被销毁，
// 造成了一个诡异的“局部销毁”对象。

// 2.析构函数的运作方式是，最深层派生的那个类最先析构，然后其每一个base class被析构。

// 3.polymorphic(带多态性质的)base classes应该声明一个virtual析构函数。如果class
// 带有任何virtual函数，它就应该拥有一个virtual析构函数。这样的基类的设计目的是为了
// 用来通过基类接口处理derived class对象。

// 4.一个类的设计目的如果不是作为基类使用，或不是为了具备多态性，就不应该声明为virtual
// 析构函数。

// 5.声明虚析构函数的另一个用途是，当定义一个抽象类的时候，没有想要定义为virtual的函数，
// 可以将析构函数进行virtual

#include <iostream>
#include <string>

using namespace std;

class AbstractClass {
	AbstractClass();
	virtual ~AbstractClass() = 0;
};

class Base06 {
public:
	Base06() {
		cout << "Base06 construct" << endl;
	}

	virtual ~Base06() {
		cout << "Base06 destruct" << endl;
	}

	void IntroduceNoVirtual() {
		cout << "I am Base06" << endl;
	}

	virtual void IntroduceWithVirtual() {
		cout << "I am Base06" << endl;
	}
};

class Derived061 : public Base06 {
public:
	Derived061() {
		cout << "Derived061 construct" << endl;
	}

	~Derived061() {
		cout << "Derived061 destruct" << endl;
	}

	void IntroduceNoVirtual() {
		cout << "I am Derived061" << endl;
	}

	virtual void IntroduceWithVirtual() {
		cout << "I am Derived061" << endl;
	}
};

class Derived062 : public Derived061 {
public:
	Derived062() {
		cout << "Derived062 construct" << endl;
	}

	~Derived062() {
		cout << "Derived062 destruct" << endl;
	}

	void IntroduceNoVirtual() {
		cout << "I am Derived062" << endl;
	}

	virtual void IntroduceWithVirtual() {
		cout << "I am Derived062" << endl;
	}
};

int main07() {
	Base06* b1 = new Derived062();
	b1->IntroduceNoVirtual();  // 输出：I am Base06
	b1->IntroduceWithVirtual();  // 输出：I am Derived062
	delete b1;
	//Base06不用虚析构函数时，输出结果为：
	//Base06 construct
	//Derived061 construct
	//Derived062 construct
	//Base06 destruct
 
	//Base06使用虚析构函数时，输出结果为：
	//Base06 construct
	//Derived061 construct
	//Derived062 construct
	//Derived062 destruct
	//Derived061 destruct
	//Base06 destruct
	//构造时顺序为：没有老子哪有儿子。析构顺序为白发人送黑发人。
	
	return 0;
}
