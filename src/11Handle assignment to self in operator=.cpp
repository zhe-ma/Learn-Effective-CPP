// 条款11： 在operator=中处理“自我赋值”
// 1.确定任何函数如果操作一个以上的对象，而其中多个对象是同一个对象时，其行为仍然正确。
// 2.确保当对象自我赋值时operator=有良好行为。其中技术包括比较“来源对象”和“目标对象”的地址、
// 精心安排的语句顺序、以及copy-and-swap。

// operator=自己给自己赋值的可能情景：
// class Widget{} Widget w；
// w = w。 ws[i] = ws[j]，*px = *py。

// 例如
class Bitmap {
};

class Widget {
public:
	// 弊端分析：如果这样做，当进行自我赋值时，delete pb时，将对象销毁，那么也就是
	// 销毁了rhs的对象，所以后面的动作全是错的。
	Widget& operator=(const Widget& rhs) {
		delete pb;  // 因为要赋值，所以首先删掉原来自己指向的内存，防止内存泄露。
		pb = new Bitmap(*rhs.pb);  // 指向rhs里面bitmap的副本。
		return *this;
	}

private:
	Bitmap* pb;  // 指针，指向一个从heap分配而得的对象。
};

// 改进方式一：证同测试（identity test）
// 弊端分析： 如果在new Bitmap() 这一句出现异常，那么pb就可能指向一块被删除的Bitmap.
class Widget1 {
public:
	Widget1& operator=(const Widget1& rhs) {
		 if (this == &rhs)  // 首先证同测试，如果是自我赋值，就不做任何事。
		 {
			 return *this;
		 }
		 delete pb;   
		 pb = new Bitmap(*rhs.pb);   
		 return *this;
	}
private:
	Bitmap* pb;  // 指针，指向一个从heap分配而得的对象。
};

// 改进方式二： 精心安排语句顺序
// 如果new Bitmap抛出异常，那么pb还是指向原来的bitmap。如果不抛出异常，那么pb首先指向新的bitmap,
// 然后删掉自己原来的bitmap。
// 弊端分析：如果是自我赋值，相当于对原来的bitmap做了一份复件，然后删除原来的bitmap，然后再指向这个bitmap。
// 这种办法显然有点损失效率。
class Widget2 {
public:
	Widget2& operator=(const Widget2& rhs) {
		Bitmap* ptemp = pb;  // 首先记住原来的pb，令pb指向*pb的一个副本，然后删除原先的pb。
		pb = new Bitmap(*rhs.pb);
		delete ptemp;
		return *this;
	}
private:
	Bitmap* pb;  // 指针，指向一个从heap分配而得的对象。
};

// 方式三：copy-and-swap
// 优点：不但保证“异常安全”，而且“自我赋值安全”。
class Widget3 {
public:
	void Swap(Widget3& rhs);  // 交换*this和rhs的数据。
	Widget3& operator=(const Widget3& rhs) {
		Widget3 temp(rhs);  // 为rhs数据制作一份副本，将*this数据和上述复件的数据交换。
		Swap(temp);
		return *this;
	}
private:
	Bitmap* pb;  // 指针，指向一个从heap分配而得的对象。
};