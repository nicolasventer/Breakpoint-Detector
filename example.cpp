#include <chrono>
#include <iostream>
#include <thread>

#include "BreakpointDetector.hpp"

int main()
{
	using namespace std::chrono_literals;

	BreakpointDetector breakpointDetector{false};
	breakpointDetector.addDetectedBreakpointCallback([](const std::chrono::milliseconds& breakpointDuration)
		{ std::cout << "Breakpoint detected: " << breakpointDuration.count() << "ms" << std::endl; });
	breakpointDetector.run();
	std::this_thread::sleep_for(200ms);
	std::cout << "Hello" << std::endl;
	std::this_thread::sleep_for(200ms);
	std::cout << "Nice to meet you" << std::endl;
	std::this_thread::sleep_for(200ms);
	std::cout << "Bye" << std::endl;
	std::cout << "Total breakpoint duration : " << breakpointDetector.getBreakpointTotalDuration().count() << "ms" << std::endl;
	return 0;
}
