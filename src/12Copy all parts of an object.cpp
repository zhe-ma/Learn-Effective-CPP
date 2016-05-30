// 条款12： 复制对象时勿忘记其每一个成分

// 这里将copy构造函数和copy assignment操作符统称为copying函数。
// 1.Copying函数应该确保复制“对象内的所有成员变量”及“所有base class成分”。
// 2.不要尝试以某个copying函数实现另一个copying函数。应该将共同机能放进第三个函数中，
// 并由两个copying函数共同调用。copy构造函数和copy assignment操作符两个之间的互相调用
// 是没有任何意义的，一个用来构造新的对象，另一个用来给对象赋值。

#include <iostream>
#include <string>

class Customer {
public:
	Customer() {
	}
	Customer(const Customer& rhs) : name_(rhs.name_) {
		// name_ = rhs.name_;  // 不要这样写，效率问题，构造问题，写到初始化列表
	}
	Customer& operator=(const Customer& rhs) {
		name_ = rhs.name_;
		return *this;
	}

private:
	std::string name_;
};

class PriorityCustomer : public Customer {
public:
	PriorityCustomer() {
	}
	PriorityCustomer(const PriorityCustomer& rhs)
		: Customer(rhs), priority_(rhs.priority_) {  // 调用base class的copy构造函数
	}
	PriorityCustomer& operator=(const PriorityCustomer& rhs) {
		Customer::operator=(rhs);  // 对base class成分进行赋值
		priority_ = rhs.priority_;
		return *this;
	}

private:
	int priority_;
};

int main12() {
	PriorityCustomer priority_customer;
	PriorityCustomer priority_customer2(priority_customer);
	priority_customer = priority_customer2;
	return 0;
}