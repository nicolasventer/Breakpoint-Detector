// Copyright (c) Nicolas VENTER All rights reserved.

#pragma once

#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>

using DetectedBreakpointCallback = std::function<void(const std::chrono::milliseconds& breakpointDuration)>;

namespace breakpoint_detector_example
{
	// example of detected breakpoint callback that displays the duration of the detected breakpoint
	static void displayDetectedBreakpoint(const std::chrono::milliseconds& breakpointDuration);
} // namespace breakpoint_detector_example

class BreakpointDetector
{
public:
	// true for bRun in order to start the breakpoint detection at construction
	BreakpointDetector(bool bRun);

	// can be called at any time, ideally in the main function
	void addDetectedBreakpointCallback(DetectedBreakpointCallback detectedBreakpointCallback);

	template <class _Rep, class _Period> void run(const std::chrono::duration<_Rep, _Period>& breakpointDetectionDuration);

	/**
	 * @brief start to check if a breakpoint has been detected
	 * should be called once, ideally in the main function
	 *
	 * @param breakpointDetectionDuration minimum duration from which a breakpoint is detected
	 * @param checkDuration duration used for periodic check of a breakpoint
	 */
	template <class _Rep1, class _Period1, class _Rep2, class _Period2>
	void run(const std::chrono::duration<_Rep1, _Period1>& breakpointDetectionDuration,
		const std::chrono::duration<_Rep2, _Period2>& checkDuration);

	void run(const std::chrono::nanoseconds& breakpointDetectionDuration = std::chrono::nanoseconds(100'000'000), // 100ms
		const std::chrono::nanoseconds& checkDuration = std::chrono::nanoseconds(16'000'000));					  // 16ms

	// sum of all breakpoint detected
	std::chrono::milliseconds getBreakpointTotalDuration() const;

private:
	std::vector<DetectedBreakpointCallback> detectedBreakpointCallbackList;
	std::chrono::milliseconds breakpointTotalDurationMs;
};

// ============================================================
// ============================================================
// ===================== Implementations  =====================
// ============================================================
// ============================================================

namespace breakpoint_detector_example
{
	static void displayDetectedBreakpoint(const std::chrono::milliseconds& breakpointDuration)
	{
		std::cout << "Breakpoint detected: " << breakpointDuration.count() << "ms" << std::endl;
	}
} // namespace breakpoint_detector_example

BreakpointDetector::BreakpointDetector(bool bRun)
{
	if (bRun) run();
}

void BreakpointDetector::addDetectedBreakpointCallback(DetectedBreakpointCallback detectedBreakpointCallback)
{
	detectedBreakpointCallbackList.push_back(detectedBreakpointCallback);
}

template <class _Rep, class _Period>
void BreakpointDetector::run(const std::chrono::duration<_Rep, _Period>& breakpointDetectionDuration)
{
	run(std::chrono::duration_cast<std::chrono::nanoseconds>(breakpointDetectionDuration));
}

template <class _Rep1, class _Period1, class _Rep2, class _Period2>
void BreakpointDetector::run(const std::chrono::duration<_Rep1, _Period1>& breakpointDetectionDuration,
	const std::chrono::duration<_Rep2, _Period2>& checkDuration)
{
	run(std::chrono::duration_cast<std::chrono::nanoseconds>(breakpointDetectionDuration),
		std::chrono::duration_cast<std::chrono::nanoseconds>(checkDuration));
}

void BreakpointDetector::run(
	const std::chrono::nanoseconds& breakpointDetectionDuration, const std::chrono::nanoseconds& checkDuration)
{
	std::thread t(
		[&]()
		{
			while (true)
			{
				auto begin = std::chrono::high_resolution_clock::now();
				std::this_thread::sleep_for(checkDuration);
				auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(
					std::chrono::high_resolution_clock::now() - begin - checkDuration);
				if (diff > breakpointDetectionDuration)
				{
					breakpointTotalDurationMs += diff;
					for (const auto& detectedBreakpointCallback : detectedBreakpointCallbackList)
						detectedBreakpointCallback(diff);
				}
			}
		});
	t.detach();
}

std::chrono::milliseconds BreakpointDetector::getBreakpointTotalDuration() const { return breakpointTotalDurationMs; }