/*******************************************************************************

 File Name: coroutine.cpp
 Author: Shaopeng Li
 mail: lsp001@mail.ustc.edu.cn
 Created Time: 2016年02月25日 星期四 10时05分33秒

*******************************************************************************/

#include <stdexcept>

#include "coroutine.h"

using namespace std;
using namespace uc;

coroutine::coroutine(task_t task, size_t ss)
	: _task(task), _stat(START)
{
	getcontext(&_ctx);
	_ctx.uc_stack.ss_sp = new char[ss];
	_ctx.uc_stack.ss_size = ss;
	_ctx.uc_link = &_back;
	makecontext(&_ctx, reinterpret_cast<void (*)()>(&coroutine::worker),
				1, this);
}

void coroutine::resume()
{
	if (_stat != STOP) {
		swapcontext(&_back, &_ctx);
	} else {
		throw std::runtime_error("coroutine already stopped");
	}
}

void coroutine::yield()
{
	if (_stat != STOP) {
		swapcontext(&_ctx, &_back);
	} else {
		throw std::runtime_error("coroutine already stopped");
	}
}

void coroutine::worker()
{
	_stat = RUNING;
	_task(shared_from_this());
	_stat = STOP;
}

coroutine::~coroutine()
{
	delete [] (char*)_ctx.uc_stack.ss_sp;
}
