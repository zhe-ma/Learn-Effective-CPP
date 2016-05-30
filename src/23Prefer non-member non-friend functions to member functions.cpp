// 条款23： 宁以non-member、non-friend替换member函数。
// 1.宁可拿non-member non-friend函数替换member函数。这样做可以增加封装性、
// 包裹弹性和机能扩充性。

class WebBrowser23 {
public:
	void ClearCache();
	void ClearHistory();
	void RemoveCookies();

public:
	void ClearEverthing() {
		ClearCache();
		ClearHistory;
		RemoveCookies();
	}
};

void ClearBrower(WebBrowser23& wb) {
	wb.ClearCache();
	wb.ClearHistory();
	wb.RemoveCookies();
}

// 提供便利的函数该放到哪里。

// 考虑：一次性清除浏览器的member函数ClearEverything好还是non-member
// non-friend函数ClearBrowser好！答案是ClearBrowser更好，对类的封装性更好。

// 2.封装：如果某些东西被封装，它就不再可见。越多的东西被封装，越少的人可以看到它。
// 而越少人看到它，我们就有越大的弹性去改变它，因为我们的改变仅仅直接影响看到改变
// 的那些事物。封装使我们能改变事物而只影响有限客户。

// 3.考虑对象内的数据，越少的代码可以访问数据，那么越多的数据就可以被封装，而我们
// 也就越能自由地改变对象数据。如何测量有多少代码可以看到某块数据呢？我们可以计算能够
// 访问该数据的函数数量，来作为一种粗糙的测量。越多的函数访问它，数据的封装性也就越低。
// 就像条款22所说成员变量应该是private，如果不是，就会有无限量的函数可以访问它们，
// 它们也就毫无封装性。外部的一个函数想要访问private成员变量，只能使用成员函数或者加上
// friend.但是这样做直接访问了private成员变量，增加了访问它的函数数量。这也就解释了
// non-member non-friend函数比member函数更瘦欢迎的原因：non-member non-friend使类
// 有较大的封装性。

// C++更为自然的做法是让ClearBrower成为一个non-member函数并且和WebBrowser位于同一个
// namespace里面。namespace可以跨越多个源码文件，而class不行。
// 将所有的便利函数放在多个头文件内但隶属同一个命名空间，意味着客户可以轻松扩展
// 这一组便利函数。这种分割方法使得客户可以只对它们所用的那一小部分系统形成编译相依.
// 例如C++标准库，在std命名空间里面有好多头文件，如果你用vector的相关性能只需#include
// vector就可以了，不用去include其他的无关内容。

// 在"web_browser.h"中定义WebBrower这个类自身，以及其核心功能。
namespace WebBrowserStuff {
	class WebBrowser23 {
	public :
		void Func1();
		void Func2();
		void Func3();
	};
	
	// non-member函数，提供几乎所有客户都需要的核心机能
	void CoreFunc(WebBrowser23& wb) {
		wb.Func1();
	}
}

// 在头文件"webbrower_bookmarks.h"中提供与书签相关的便利函数
namespace WebBrowserStuff {
	void ClearBookmarks(WebBrowser23& wb) {
		wb.Func2();
		wb.Func3();
	}
}

// 在头文件"webbrower_cookies.h"中提供与Cookies相关的便利函数
namespace WebBrowserStuff {
	void ClearCookies(WebBrowser23& wb) {
		wb.Func1();
		wb.Func2();
		wb.Func3();
	}
}
