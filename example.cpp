#include <chrono>
#include <iostream>
#include <thread>

#include "BreakpointDetector.hpp"

int main()
{
	// the breakpoint detection is immediately done at start

	using namespace std::chrono_literals;

	breakpoint_detector::addDetectedBreakpointCallback(breakpoint_detector::example::displayDetectedBreakpoint);

	std::this_thread::sleep_for(200ms);
	std::cout << "Hello" << std::endl;
	std::this_thread::sleep_for(200ms);
	std::cout << "Nice to meet you" << std::endl;
	std::this_thread::sleep_for(200ms);
	std::cout << "Bye" << std::endl;
	std::cout << "Total breakpoint duration: " << breakpoint_detector::getBreakpointTotalDuration().count() << "ms" << std::endl;
	return 0;
}
