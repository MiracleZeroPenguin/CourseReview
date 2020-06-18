#include <iostream>
#include <stack>
#include <map>
#include <string>

using namespace std;

int main() {
	stack<char> op;

	// �涨���ȼ�
	map<char, int> mymap;
	mymap['+'] = 1;
	mymap['*'] = 2;

	string infix = "4*3+(6*3-12)";
	string postfix = "";

	for (int i = 0; i < infix.length(); i++) {
		// ������->ֱ�����
		if (infix[i] >= 'a' && infix[i] <= 'z') {
			postfix += infix[i];
		}
		else if (infix[i] == '(') {
			// '(' ->��ջ
			op.push(infix[i]);
		}
		else if (infix[i] == ')') {
			// ')' ->��ջ����'('֮��Ĳ�����ȫ�����
			while (op.top() != '(') {
				postfix += op.top();
				op.pop();
			}
			// ��'('����
			op.pop();
		}
		else {
			// ������->��ջ�����,ֱ��ջ�����ȼ�С�ڸò�����,���Ѹò�����ѹ��ջ
			while (!op.empty() && mymap[op.top()] >= mymap[infix[i]]) {
				postfix += op.top();
				op.pop();
			}
			op.push(infix[i]);
		}
	}
	// ��ջ��ʣ��Ĳ��������
	while (!op.empty()) {
		postfix += op.top();
		op.pop();
	}

	cout << postfix << endl;

	return 0;
}
