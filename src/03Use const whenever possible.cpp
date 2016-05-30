// 条款03: 尽可能使用const

#include <iostream>
#include <vector>
#include <string>

// 1.用const 修饰函数的参数
//  如果输入参数采用“指针传递”，那么加const修饰可以防止意外地改动该指针，起到保护作用。
//  例如void StringCopy(char*strDestination, const char *strSource);
//  如果输入参数采用“值传递”，由于函数将自动产生临时变量用于复制该参数，该输入参数本来就无需保护，
//  所以不要加const修饰。例如不要将函数void Func1(int x) 写成voidFunc1(const int x)。
//  同理不要将函数void Func2(A a) 写成void Func2(const Aa)。其中A 为用户自定义的数据类型。
//  对于非内部数据类型的参数而言，像void Func(A a)这样声明的函数注定效率比较底。
//  因为函数体内将产生A类型的临时对象用于复制参数a，而临时对象的构造、复制、析构过程都将消耗时间。
//  为了提高效率，可以将函数声明改为void Func(A&a)，因为“引用传递”仅借用一下参数的别名而已，不需要产生临时对象。
//  但是函数void Func(A&a) 存在一个缺点：“引用传递”有可能改变参数a，这是我们不期望的。解决这个问题很容易，
//  加const修饰即可，因此函数最终成为voidFunc(const A &a)。以此类推，
//  是否应将void Func(int x)改写为void Func(const int&x)，以便提高效率？完全没有必要，
//  因为内部数据类型的参数不存在构造、析构的过程，而复制也非常快，“值传递”和“引用传递”的效率几乎相当。
//  结论:A.对于非内部数据类型的输入参数，应该将“值传递”的方式改为“const 引用传递”，目的是提高效率。
//  结论:B.对于内部数据类型的输入参数，不要将“值传递”的方式改为“const引用传递”。否则既达不到提高效率的目的，又降低了函数的可理解性。

// 2.用const修饰函数的返回值
// 如果给以“指针传递”方式的函数返回值加const修饰，那么函数返回值（即指针）的内容不能被修改，
// 该返回值只能被赋给加const 修饰的同类型指针。
// 例如函数const char *GetString(void); 如下语句将出现编译错误：
// char *str = GetString(); 正确的用法是constchar *str = GetString(); 
// 如果函数返回值采用“值传递方式”，由于函数会把返回值复制到外部临时的存储单元中，加const修饰没有任何价值。
// 例如不要把函数int GetInt(void) 写成const int GetInt(void)。
// 同理不要把函数AGetA(void) 写成const A GetA(void)，其中A 为用户自定义的数据类型。
// 如果返回值不是内部数据类型，将函数AGetA(void) 改写为const A &GetA(void)的确能提高效率。
// 但此时千万千万要小心，一定要搞清楚函数究竟是想返回一个对象的“拷贝”还是仅返回“别名”就可以了，否则程序会出错。
// 函数返回值采用“引用传递”的场合并不多，这种方式一般只出现在类的赋值函数中，目的是为了实现链式表达。
// 例如：class A{ A &operate = (const A &other); // 赋值函数};
//  A a, b, c; //a, b, c 为A 的对象
//  a = b = c; // 正常的链式赋值
//  (a = b) =c; // 不正常的链式赋值，但合法如果将赋值函数的返回值加const 修饰，那么该返回值的内容不允许被改动。
//  上例中，语句 a = b= c 仍然正确，但是语句 (a = b) = c 则是非法的。

// 3.const成员函数const的作用
// 说明其不会修改数据成员,任何不会修改数据成员的函数都应该声明为const 类型。
// 如果在编写const成员函数时，不慎修改了数据成员，或者调用了其它非const成员函数，编译器将指出错误，
// 这无疑会提高程序的健壮性。以下程序中，类stack 的成员函数GetCount仅用于计数，从逻辑上讲GetCount 
// 应当为const 函数。编译器将指出GetCount 函数中的错误。
class Stack {
public:
	void Push(int elem);
	int Pop(void);
	int GetCount(void) const;  // const成员函数
  private:
	  int m_num; 
		int m_data[100];
  };
int Stack::GetCount(void)const
{
	//++m_num; //编译错误，企图修改数据成员m_num
	//Pop(); // 编译错误，企图调用非const函数
	return m_num;
  }
// const成员函数的声明看起来怪怪的：const关键字只能放在函数声明的尾部，大概是因为其它地方都已经被占用了。

// 4.关于Const函数的几点规则：
//  a.const对象只能访问const成员函数, 而非const对象可以访问任意的成员函数, 包括const成员函数.
//  b.const对象的成员是不可修改的, 然而const对象通过指针维护的对象却是可以修改的.
//  c.const成员函数不可以修改对象的数据.它在编译时, 以是否修改成员数据为依据, 进行检查.
//  d.然而加上mutable修饰符的数据成员, 对于任何情况下通过任何手段都可修改, 自然此时的const成员函数是可以修改它的
//  e.const 函数只能调用 const函数，即使某个函数本质上没有修改任何数据，但没有声明为const，也是不能被const函数调用的。

class A {
public:
	int a;
	int b;
	mutable int c;
	void Test() const {
		//a++;  // 错误
		c++;  // 正确
	}
	void Test1() {
		a++;
	}
};

// 5.const 修饰指针
const char* p1 = "hello";  // const data, non-const pointer
char* const p2 = "hello";  // const pointer, non-const data

// 6.const修饰迭代器
// STL迭代器是以指针为根据塑造出来的,所以迭代器的作用就像个T*指针
void ConstIterator() {
	std::vector<int> vec;
	vec.push_back(1);
	vec.push_back(2);
	const std::vector<int>::iterator iter = vec.begin();  // iter的作用就像一个T* const.
	*iter = 10;  // 正确
	//iter++;  // 错误

	std::vector<int>::const_iterator citer = vec.begin();  // citer的作用就像const T*.
	//*citer = 10;  // 错误 
	++citer;  // 正确

}

// 7.两个成员函数如果只是常量性不同,可以被重载
class TextBlock {
public:
	TextBlock() {
		str = "abcdefg";
	}

	char& operator[] (std::string::size_type position) {
		return str[position];
	}

	const char& operator[] (std::string::size_type position) const { 
		// 注意: 只有成员函数用const修饰,才为重载
		// 返回值必须用const修饰,因为函数用const修饰,那么里面的成员则也为const属性,
		// 'return' : cannot convert from 'const char' to 'char &
		return str[position + 1];
	}

	void Test() {
	}

	void TestConst() const {
	}

	void Print(TextBlock& text) {
		std::cout << text[0] << std::endl;
	}

	void Print(const TextBlock& text) {  
		// 参数用const修饰也变为了重载
		std::cout << text[0] << std::endl;
	}

private:
	std::string str;
};

// TextBlock实现代码复用的方法:
// 上面的TextBlock两个=运算符的重载,里面实现了同样的代码,这样做的坏处是,如果代码很长那么,就导致了代码累赘.
// 解决方法就是non-const函数调用const函数,然后返回值强转为non-const.
// static_cast < type-id > ( expression )
// 该运算符把expression转换为type-id类型，但没有运行时类型检查来保证转换的安全性。
// const_cast<type_id> (expression)
// 该运算符用来修改类型的const或volatile属性。除了const 或volatile修饰之外， type_id和expression的类型是一样的。
class TextBlock2 {
private:
	std::string str;

public:
	TextBlock2(std::string str) {
		this->str = str;
	}

	const char& operator[] (std::string::size_type position) const {
		std::string other_code = "many other codes";
		std::cout << other_code << std::endl;
		return str[position];
	}

	char& operator[] (std::string::size_type position) {
		return const_cast<char&>(static_cast<const TextBlock2&>(*this)[position]);  
		//将*this本对象强制转为const TextBlock2&类型,然后调用[]const函数得到const char的值,最后将其的const属性去掉
	}
};

int main03() {
	A* a = new A();
	a->Test();  // 正确
	const A* a1 = new A();
	a->Test1();  // 正确
	a->Test();  // 正确

	TextBlock text1, text2;
	const TextBlock ctext;
	text2.Print(text1);  // a const和non-const参数导致不同的处理
	text2.Print(ctext);  // b
	ctext.TestConst();  // 正确
	//ctext.Test();  // cannot convert 'this' pointer from 'const TextBlock' to 'TextBlock &'
	// const对象只能调用const成员函数
	// 原因: C++ Primer中说了，在一个对象调用其成员函数时，它隐含的一个形参this指针。
  // 例如，我们定义了一个函数CTest::t(); 实际上在编译器中该函数的定义就是CTest::t(CTest *const this)，
	// 该this指针所指向的内容可以改变，但是该this指针不可以被改变。
	// 当我们用CTest的一个对象test1调用t函数时即test1.t()时，编译器就会将它解释为t(&test1)，
	// 所以我们在t中使用this指针就可以改变对象test1的成员变量了。
  // 但是，当我们的对象是const对象时，即const CTest t;t的内容是不可以改变的，
	// 当我们把&t作为一个参数传到形参this时，矛盾出现了：t是一个常量，其成员不可以被改变；
	// this指针的成员变量是可以改变的。如果我能正确的将t的地址传给this，
	// 那么t这个常量的值不是可以在this中被改变了吗，所以，编译器是不允许这种情况出现的，就提示错误了。
	// 故，const对象不能访问非const成员函数。
  // 同理，当我们的成员函数是const成员函数时，例：CTest::t() const, 
	// 在编译器解释时会将该函数解释为CTest::t(const CTest * const this), this指针及其所指向的内容都不可以被修改，
	// 前面提到的矛盾也就不存在了，所以const对象可以访问const成员变量。

	TextBlock t;
	char& tem = t[0];
	std::cout << tem << std::endl;  // a
	const char& tem2 = t[0];
	std::cout << tem2 << std::endl; //a
	const TextBlock t1;  // const对象调用const成员函数
	const char& tem3 = t1[0];  // 返回const,所以必须用const修饰

	TextBlock2 t3("abcdef");
	char& tem4 = t3[0];
	std::cout << tem3 << std::endl;
	tem4 = 'b';  // 正确
	return 0;
}

//  参考 : http://blog.csdn.net/zheng19880607/article/details/23883437
