// 条款14： 在资源管理类中小心copy行为
// 1.复制RAII对象必须一并复制它所管理的资源，所以资源的copying行为决定RAII对象的copying行为。
// 2.普遍而常见的RAII class copying行为是：抑制copying、施行引用计数法(reference counting)。
// 不过其他行为也都可能被实现。

#include <iostream>
#include <memory>

class UnCopyable {
public:
	UnCopyable() {
	}
	~UnCopyable() {
	}
	
private:
	UnCopyable(UnCopyable& );
	UnCopyable& operator=(UnCopyable&);
};

// 互斥器对象
class Mutex {
};

// 锁定pm所指的互斥器
void lock(Mutex* pm);

// 将互斥器解除锁定，并不是将互斥器销毁，只是解除锁定
void unlock(Mutex* pm);

// 建立一个类来管理锁，采用RAII守则，资源获取时就是初始化时机，也就是
// “资源在构造期获得，在析构期释放”。在这里是互斥器对象在管理类构造期加锁，
// 在管理类析构期解锁。
// 这样做的目的是，建立一个管理类来管理某个对象是其自动做某些行为，而不单单是
// 销毁资源释放内存等。所以可以用指针来管理这类，管理这个对象的行为。
class Lock {
public:
	explicit Lock(Mutex* pm) 
		: mutex_ptr_(pm) {
		lock(mutex_ptr_);  // 在构造器获取资源也就是加锁，在这里获取资源就是加锁
	}
	~Lock() {
		unlock(mutex_ptr_);  // 在析构期释放资源也就是解锁
	}

private:
	Mutex* mutex_ptr_;
};

// 3.一般当一个RAII对象被复制，大多数时候采用以下两种选择：
// (A)禁止复制，因为许多时候允许RAII对象被复制并不合理。
class Lock1 : public UnCopyable {  // 使用这种方法来禁止复制
};

// (B)对底层资源祭出“引用计数法(refrence-count)”。这样做的目的是，希望保有资源，
// 直到它的最后一个使用者被销毁时才释放该资源。
// 我们的实现方法可以使用tr1::shared_ptr成员变量。但是tr1::shared_ptr的缺省行为是
// “当引用次数为0的时候删除某物”而我们要的行为是unlock某物。幸运的是，tr1::shared_ptr
// 允许指定“删除器”,y也就是指定删除行为，将其删除行为变为我们想要的行为。
class Lock2 {
public:
	explicit Lock2(Mutex* pm)
		: mutex_ptr_(pm, unlock) {  // 给shared_ptr设置unlock为删除器
		lock(mutex_ptr_.get());
	}
	// 不必定义析构函数，因为class析构函数（不论是编译器生成，还是用户自定义的）
	// 总是会自动调用其non-static成员变量的析构函数。所以mutex_ptr_的析构函数
	// 会在互斥器的引用次数为0时自动调用tr1::shared_ptr的删除器
private:
	std::tr1::shared_ptr<Mutex> mutex_ptr_;  // 使用shared_ptr来替换raw_pointer。
};

// 4.以下两种解决方法也可能出现
// (A)复制底部资源，也就是深拷贝。
// (B)转移底部资源的拥有权，就像auto_ptr一样，谁复制了，这个资源就归谁。

int main14() {
	Mutex m;
	// 建立一个区块，在区块末尾自动解除互斥器锁定
	{
		Lock m1(&m);
	}
	// 但是如果发生如下行为会怎么样？该对象会被unlock两次，因为这样的浅拷贝，会直接指针赋值。
	{
		Lock ml1(&m);
		Lock ml2(&m);
	}

	return 0;
}
