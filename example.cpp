/*******************************************************************************

 File Name: example.cpp
 Author: Shaopeng Li
 mail: lsp001@mail.ustc.edu.cn
 Created Time: 2016年02月25日 星期四 21时31分31秒

*******************************************************************************/

#include <iostream>

#include "coroutine.h"

using namespace std;
using namespace uc;

void func(coroutine_ptr co)
{
	for (int i = 0; i < 10; ++i) {
		int mul = co->yield<int, int>(i);
		cout << mul << endl;
	}
}

int main()
{
	uc::coroutine_ptr co = uc::coroutine::create(func);
	int n = co->resume<int>();
	
	while (!co->stopped()) {
		int mul = n * n;
		cout << n << " * " << n << " = ";
		n = co->resume<int, int>(mul);
	}

	return 0;
}
