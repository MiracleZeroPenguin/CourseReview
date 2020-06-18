#include <iostream>
#include <stack>
#include <map>
#include <string>

using namespace std;

int main() {
	stack<char> op;

	// 规定优先级
	map<char, int> mymap;
	mymap['+'] = 1;
	mymap['*'] = 2;

	string infix = "4*3+(6*3-12)";
	string postfix = "";

	for (int i = 0; i < infix.length(); i++) {
		// 操作数->直接输出
		if (infix[i] >= 'a' && infix[i] <= 'z') {
			postfix += infix[i];
		}
		else if (infix[i] == '(') {
			// '(' ->入栈
			op.push(infix[i]);
		}
		else if (infix[i] == ')') {
			// ')' ->将栈中在'('之后的操作符全部输出
			while (op.top() != '(') {
				postfix += op.top();
				op.pop();
			}
			// 将'('弹出
			op.pop();
		}
		else {
			// 操作符->将栈顶输出,直到栈顶优先级小于该操作符,最后把该操作符压入栈
			while (!op.empty() && mymap[op.top()] >= mymap[infix[i]]) {
				postfix += op.top();
				op.pop();
			}
			op.push(infix[i]);
		}
	}
	// 将栈中剩余的操作符输出
	while (!op.empty()) {
		postfix += op.top();
		op.pop();
	}

	cout << postfix << endl;

	return 0;
}
