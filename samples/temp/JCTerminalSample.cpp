
#include <c/JCTerminal.h>

#include <SAELib_Timer.h>

#include <chrono>
#include <iostream>
#include <cassert>

int main()
{
	auto _terminal = jcTerminal_open(64, 24, "nosegay");

	sae::timer _timer{ std::chrono::seconds{5} };
	_timer.start();

	while (!_timer.finished())
	{
		jcTerminal_refresh(_terminal);
		
	};

	jcTerminal_close(&_terminal);
	
	assert(_terminal == nullptr);

	return 0;
};