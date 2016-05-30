// 条款13: 以对象管理资源
// 1.为防止资源泄漏，请使用RAII对象，它们在构造函数中获得资源并在析构函数中释放资源.
// 2.两个常被使用的RAII classes 分别是tr1::shared_ptr和auto_ptr。前者通常是较佳选择，
// 因为其copy行为比较直观。若选择auto_ptr，复制动作会使它（被复制物）指向null.

// 3.TR1(Technical Report1)是针对c++标准库的第一次扩展.

// 4.以对象管理资源的两个关键想法：
// (A)RAII(Resource Acquisition Is Initialization)即资源取得时机便是初始化时机。
// 获得资源后立刻放入管理对象内。例如使用auto_ptr等智能指针对象，来进行管理对象。
// (B)管理对象运用析构函数确保资源被释放。也就是对象一旦被销毁，就调用析构函数释放资源。

#include <iostream>
#include <memory>

class Investment {
public:
	Investment() {
		std::cout << "construct" << std::endl;
	}
	~Investment() {
		std::cout << "destruct" << std::endl;
	}
};

// 创造工厂函数来创建对象，返回指针，指向Investment,继承体系内的动态分配对象，调用者有责任删除它。
Investment* CreateInvestment() {
	return new Investment;
}
                                
// f1这种做法的弊端是很多情况下，程序根本不会运行delete，导致pinv指向的对象,
// 及对象里面保存的资源没有被释放。
void f1() {
	Investment* pinv = CreateInvestment();
	{
		// 其他代码
	}
	delete pinv;  // 释放pinv所指对象
}

// f2通过将对象交给管理兑现auto_ptr来管理，使得它们在离开控制流或某个代码区块
// 或函数时被释放。
void f2() {
	// 经由auto_ptr的析构函数自动删除pinv
	std::auto_ptr<Investment> pinv(CreateInvestment());
	// auto_ptr为了防止一个对象被删除一次以上，有个不寻常的性质：若是通过copy构造函数
	// 或copy assignment操作符复制它们，它们就会变成null，而复制所得的指针将取得资源的
  // 唯一拥有权。它的底层条件就是：受auto_ptr管理的资源必须绝对没有一个以上的auto_ptr
	// 同时指向它。
	std::auto_ptr<Investment> pinv2(pinv);  // 现在pinv2指向对象，pinv为null
	pinv = pinv2;  // 现在pinv指向对象，pinv2被设为null。
}

// f3使用shared_ptr来管理对象。
// RCSP(reference-counting smart pointer)即引用计数型智慧指针，持续追踪共有多少对象指向
// 某笔资源，并在无人指向它时自动删除该资源，该行为类似于垃圾回收。但是其无法打破
// 环状引用（cycles of refrence，例如两个其实已经没被使用的对象互指，因此好像还在
// “被使用”状态）。
void f3() {
	// 经由shared_ptr析构函数自动删除pinv。
	std::tr1::shared_ptr<Investment> pinv(CreateInvestment());
	std::tr1::shared_ptr<Investment> pinv2(pinv);  // pinv和pinv2指向同一个对象
	pinv = pinv2;  // 同上，无任何改变
}

// 注： auto_ptr和shared_ptr两者在其析构函数内做delete而不是delete[],也就是说在动态
// 分配而得的array身上使用这两个是无用的。

int main13() {
	f1();  // 输出：construct destruct
	f2();  // 输出：construct destruct
	f3();  // 输出：construct destruct
	return 0;
}
