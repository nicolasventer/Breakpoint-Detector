// Copyright (c) Nicolas VENTER All rights reserved.

#pragma once

#include <chrono>
#include <functional>
#include <thread>
#include <vector>

using DetectedBreakpointCallback = std::function<void(const std::chrono::milliseconds& breakpointDuration)>;

class BreakpointDetector
{
public:
	// true for bRun in order to start the breakpoint detection at construction
	BreakpointDetector(bool bRun)
	{
		if (bRun) run();
	}

	// can be called at any time, ideally in the main function
	void addDetectedBreakpointCallback(DetectedBreakpointCallback detectedBreakpointCallback)
	{
		detectedBreakpointCallbackList.push_back(detectedBreakpointCallback);
	}

	// should be called one time, ideally in the main function
	template <class _Rep, class _Period> void run(const std::chrono::duration<_Rep, _Period>& breakpointDetectionDuration)
	{
		run(std::chrono::duration_cast<std::chrono::nanoseconds>(breakpointDetectionDuration));
	}

	// should be called one time, ideally in the main function
	template <class _Rep1, class _Period1, class _Rep2, class _Period2>
	void run(const std::chrono::duration<_Rep1, _Period1>& breakpointDetectionDuration,
		const std::chrono::duration<_Rep2, _Period2>& checkDuration)
	{
		run(std::chrono::duration_cast<std::chrono::nanoseconds>(breakpointDetectionDuration),
			std::chrono::duration_cast<std::chrono::nanoseconds>(checkDuration));
	}

	// should be called one time, ideally in the main function
	void run(const std::chrono::nanoseconds& breakpointDetectionDuration = std::chrono::nanoseconds(100'000'000), // 100ms
		const std::chrono::nanoseconds& checkDuration = std::chrono::nanoseconds(16'000'000))					  // 16ms
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

	std::chrono::milliseconds getBreakpointTotalDuration() const { return breakpointTotalDurationMs; }

private:
	std::vector<DetectedBreakpointCallback> detectedBreakpointCallbackList;
	std::chrono::milliseconds breakpointTotalDurationMs;
};
