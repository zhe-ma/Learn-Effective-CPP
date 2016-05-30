// 条款17： 以独立的语句将newed对象置入智能指针。
// 1.以独立语句将newed对象存储于智能指针内。如果不这样做，一旦异常被抛出，
// 有可能导致难以察觉的资源泄漏。
// 2.智慧名言：以对象管理资源。所以用智能指针管理Widget

#include <iostream>
#include <memory>

int GetPriority() {
	return 1;
}

class Widget17 {
};

void ProcessWidget1(std::tr1::shared_ptr<Widget17> pw, int priority);

int main17() {
	// cannot convert argument 1 from 'Widget17 *' to 'std::shared_ptr<Widget17>'
	//ProcessWidget1(new Widget17, GetPriority());

	// 下面这种方式可以通过编译，但是会存在内存泄露的风险
	ProcessWidget1(std::tr1::shared_ptr<Widget17>(new Widget17), GetPriority());
	// 因为C++不同于Java，C#，这两种语言总是以特定的次序完成函数参数和核算。
	// 而C++是不确定的，可以肯定的是new Widget肯定发生在tr1::shared_ptr构造函数之前
	// 但是GetPriority发生的时间位置不能确定，如果是这样的顺序：new Widget-->Getpriority-->
	// tr1::shared_ptr的构造函数。如果在Getpriority的时候发生了异常，那么new出来的Widget还没有
	// 被放到tr1::shared_ptr中，那么就可能产生资源泄漏。

	// 解决方法将new一个对象和传给智能指针分开。
	std::tr1::shared_ptr<Widget17> pw(new Widget17);
	ProcessWidget1(pw, GetPriority());
	
	return 0;
}
