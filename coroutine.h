/*******************************************************************************

 File Name: coroutine.h
 Author: Shaopeng Li
 mail: lsp001@mail.ustc.edu.cn
 Created Time: 2016年02月25日 星期四 10时05分29秒

*******************************************************************************/

#ifndef __COROUTINE_H__
#define __COROUTINE_H__

#include <memory>
#include <functional>

#include <ucontext.h>

namespace uc {

class coroutine;
typedef std::shared_ptr<coroutine> coroutine_ptr;
	
class coroutine : public std::enable_shared_from_this<coroutine>
{
public:
	coroutine(const coroutine&) = delete;
	const coroutine& operator=(const coroutine&) = delete;
	
	typedef std::function<void (coroutine_ptr)> task_t;
	
	static coroutine_ptr create(task_t task, size_t stack_size = 4096)
	{
		return coroutine_ptr(new coroutine(task, stack_size));
	}

	template<typename TRet>
	TRet& resume()
	{
		resume();
		return * static_cast<TRet*>(_ud);
	}
	
	template<typename TArg, typename TRet>
	TRet& resume(TArg& arg)
	{
		_ud = &arg;
		resume();
		return * static_cast<TRet*>(_ud);
	}

	template<typename TRet>
	TRet& yield()
	{
		yield();
		return * static_cast<TRet*>(_ud);
	}

	template<typename TArg, typename TRet>
	TRet& yield(TArg& arg)
	{
		_ud = &arg;
		yield();
		return * static_cast<TRet*>(_ud);
	}

	bool stopped()
	{ return _stat == STOP; }
	
	~coroutine();

private:
	coroutine(task_t task, size_t ss);

	void resume();
	void yield();
	
	void worker();

	enum {
		START,
		RUNING,
		STOP
	};
	
	task_t _task;
	int _stat;
	
	ucontext_t _back;
	ucontext_t _ctx;

	void* _ud;
};

}

#endif
