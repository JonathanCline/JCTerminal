
#include <c/JCTerminal.h>

#include <SAELib_Timer.h>

#include <chrono>
#include <iostream>

int main()
{
	auto _result = jcTerminal_open(64, 24, "nosegay");
	std::cout << (int)_result << '\n';

	sae::timer _timer{ std::chrono::seconds{5} };
	_timer.start();

	while (!_timer.finished())
	{
		jcTerminal_refresh();
		
	};

	jcTerminal_close();
	
	return 0;
};