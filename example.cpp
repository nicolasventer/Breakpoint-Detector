#include <chrono>
#include <iostream>
#include <thread>

#include "BreakpointDetector.hpp"

int main()
{
	using namespace std::chrono_literals;

	BreakpointDetector breakpointDetector{false};
	breakpointDetector.addDetectedBreakpointCallback(breakpoint_detector_example::displayDetectedBreakpoint);
	breakpointDetector.run();
	std::this_thread::sleep_for(200ms);
	std::cout << "Hello" << std::endl;
	std::this_thread::sleep_for(200ms);
	std::cout << "Nice to meet you" << std::endl;
	std::this_thread::sleep_for(200ms);
	std::cout << "Bye" << std::endl;
	std::cout << "Total breakpoint duration: " << breakpointDetector.getBreakpointTotalDuration().count() << "ms" << std::endl;
	return 0;
}
