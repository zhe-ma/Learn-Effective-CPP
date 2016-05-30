// 条款10： 令operator=返回一个reference to *this。
// Have assignment operators return a reference to *this.

// 这只是个良好的建议，这个建议被所有的内置类型和标准程序库提供的类型共同遵守。
// 这个建议同样适用于+=，-=，*=等等。
// 例如：
class Widgets {
public:
	Widgets& operator+=(const Widgets& widgets) {
		// 实现
		return *this;
	}
	Widgets& operator=(int rhs) {  // 此函数同样适用，即时此操作符了参数类型不符协定。
		// 实现
		return *this;
	}

};
