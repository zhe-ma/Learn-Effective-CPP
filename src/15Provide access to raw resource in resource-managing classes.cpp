// 条款15: 在资源管理类中提供对原始资源的访问
// 这一条款的作用在于，前面的条款提到使用RAII管理类来管理对象，
// 使得对象自动的释放内存，或者自动的加锁解锁等自动操作行为。
// 但是原来的对象都被封装到管理类了，所以管理类需要提供对原始资源的访问渠道。

// 1.APIs往往要求访问原始资源，所以每一个RAII class应该提供一个“取得其所管理之资源”的办法。
// 2.对原始资源的访问可能经由显示转换或隐式转换。一般而言显示转换比较安全，但隐式转换对客户比较方便。

#include <iostream>
#include <memory>

class Investment15 {
public:
	Investment15() {
	}
	bool IsTaxFree() const;
};

Investment15* CreateInvestment() {
	return new Investment15;
}

int DaysHeld(const Investment15* pi);

void TestSmartPointer() {
	std::tr1::shared_ptr<Investment15> pinv(CreateInvestment());
	//DaysHeld(pinv);  // 错误
	// 3.tr1::shared_ptr和auto_ptr都提供了一个get成员函数，用来执行显示转换，也就是它会返回智能指针内部的原始指针。
	DaysHeld(pinv.get());

	// 4.tr1::shared_ptr和auto_ptr也重载了指针取值操作符(->和*)，它们在使用的时候将RAII对象指针转换到底部原始指针。
	bool b1 = pinv->IsTaxFree();
	bool b2 = (*pinv).IsTaxFree();
}

// 5.对于一个自己设计的RAII类，则需要自己设计显示和隐式转换函数。
// 例如Font对象，需要获取并在不用时自动释放。
class FontHandle {
};

// 获取字体句柄
FontHandle getFont();

// 释放字体句柄
void ReleaseFont(FontHandle fh);

int ChangeFontSize(FontHandle f, int new_size);

// 创建一个RAII类来管理字体
class Font {
public:
	explicit Font(FontHandle fh) 
		: fh_(fh) {
	}
	~Font() {
		ReleaseFont(fh_);
	}
	FontHandle get() const {  // 显示转换函数
		return fh_;
	}
	operator FontHandle() const {  // 隐式转换函数
		return fh_;
	}

private:
	FontHandle fh_;  // 原始字体资源
};

int main15() {
	Font f(getFont());
	ChangeFontSize(f, 5);
	ChangeFontSize(f.get(), 5);
	return 0;
}
