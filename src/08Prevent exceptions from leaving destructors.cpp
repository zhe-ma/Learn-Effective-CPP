// 条款08: 别让异常逃离析构函数
// 这个条款有歧义，并不是一定要在析构函数里面放异常,
// 而是析构里面发生的异常不要抛出到析构外面去处理。

#include <iostream>
#include <string>

using namespace std;

// 1.析构函数吐出异常，程序可能过早结束或出现不明确行为。

// 数据库连接类
class DBConnection {
public:
	static DBConnection create() {
		return DBConnection();
	}
	void close() {  // 关闭联机，失败则抛出异常。
	}
};

// 为了避免客户忘记调用close，创建一个管理DBConnection的类，并在析构函数中调用close。
class DBConn {
public:
	DBConn(DBConnection dbc) {
		db = dbc;
	}
	~DBConn() {  // 确保数据库总是被关闭
		db.close();
	}

private:
	DBConnection db;
};

// 客户可以这样调用它们
void TestDBConnection() {
	DBConn dbc(DBConnection::create());
}
// 当对象dbc销毁的时候，自动调用close函数。但是如果关闭不成功，DBConn析构函数就会
// 抛出该异常，也就是允许该异常离开析构函数。这就会造成难以驾驭的麻烦。

// 2.为了避免上述问题，有三种办法来解决。一个就是即时终止程序，另一种就是析构函数内部吞掉。
// 方法一： 终止程序
// ~DBConn() {
//  	try {
//		db.close();
//		}
//		catch (...) {
//		// 打log，记下close的调用失败。
//			abort();  // 终止程序，避免将异常从析构函数中抛出，那会导致不明确行为。
//  	}
//  }
// 方法二： 内部吞掉
// ~DBConn() {
//  	try {
//		db.close();
//		}
//		catch (...) {
//		// 打log，记下close的调用失败。
//  	}
//  }
// 一般而言，将异常吞掉会压制“某些失败动作”的重要信息。但是比草率的结束程序好。

// 方法三：重新设计DBConn接口，时刻会有机会去处理该异常。
class DBConn2 {
public:
	~DBConn2() {
		if (!closed) {
			try {  // 如果客户不去关闭的话，自己关闭连接
				db.close();
			}
			catch (...) {  // 如果关闭失败，只能使用使用方法一和二。
				//吞下程序或者结束程序
			}
		}
	}
	void close() {  // 供客户使用的新函数
		db.close();
		closed = true;
	}

private:
	bool closed;
	DBConnection db;
};
// 为什么要使用DBConn2这个类。这个类是用来管理DBConnection的。为了避免客户忘记关闭
// 而在析构函数里面来关闭，这种情况是客户忘了关闭或者客户坚信关闭不会发生异常。所以用
// 这个类来管理很方便。但是如果客户很在乎关闭异常怎么办，那么可以调用DBConn2里面的close
// 来自己处理异常。

// 2.如果客户需要对某个操作函数运行期间抛出的异常做出反应，那么class应该提供一个普通函数
// 而不是在析构函数里面去执行该操作。
