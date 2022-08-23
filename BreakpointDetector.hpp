// Copyright (c) Nicolas VENTER All rights reserved.

#pragma once

#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>

using DetectedBreakpointCallback = std::function<void(const std::chrono::milliseconds& breakpointDuration)>;

namespace breakpoint_detector
{
	// add a function to call on breakpoint detection
	static void addDetectedBreakpointCallback(DetectedBreakpointCallback detectedBreakpointCallback);

	// sum of all detected breakpoint duration
	static std::chrono::milliseconds getBreakpointTotalDuration();

	// set the minimum duration from which a breakpoint is detected
	template <class _Rep, class _Period>
	void setBreakpointDetectionDuration(const std::chrono::duration<_Rep, _Period>& breakpointDetectionDuration_);

	// set the duration used for periodic check of a breakpoint
	template <class _Rep, class _Period> void setCheckDuration(const std::chrono::duration<_Rep, _Period>& checkDuration_);

	namespace example
	{
		// example of detected breakpoint callback that displays the duration of the detected breakpoint
		static void displayDetectedBreakpoint(const std::chrono::milliseconds& breakpointDuration);

	} // namespace example
} // namespace breakpoint_detector

class BreakpointDetector
{
public:
	// start the breakpoint detection at construction
	BreakpointDetector();

	// add a function to call on breakpoint detection
	void addDetectedBreakpointCallback(DetectedBreakpointCallback detectedBreakpointCallback);

	// sum of all detected breakpoint duration
	std::chrono::milliseconds getBreakpointTotalDuration() const;

	// set the minimum duration from which a breakpoint is detected
	template <class _Rep, class _Period>
	void setBreakpointDetectionDuration(const std::chrono::duration<_Rep, _Period>& breakpointDetectionDuration_);

	// set the duration used for periodic check of a breakpoint
	template <class _Rep, class _Period> void setCheckDuration(const std::chrono::duration<_Rep, _Period>& checkDuration_);

private:
	void run();

	std::vector<DetectedBreakpointCallback> detectedBreakpointCallbackList;
	std::chrono::milliseconds breakpointTotalDurationMs;

	std::chrono::nanoseconds breakpointDetectionDuration = std::chrono::nanoseconds(100'000'000); // 100ms
	std::chrono::nanoseconds checkDuration = std::chrono::nanoseconds(16'000'000);				  // 16ms
};

// ============================================================
// ============================================================
// ===================== Implementations  =====================
// ============================================================
// ============================================================

BreakpointDetector::BreakpointDetector() { run(); }

void BreakpointDetector::addDetectedBreakpointCallback(DetectedBreakpointCallback detectedBreakpointCallback)
{
	detectedBreakpointCallbackList.push_back(detectedBreakpointCallback);
}

void BreakpointDetector::run()
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

template <class _Rep, class _Period>
void BreakpointDetector::setBreakpointDetectionDuration(const std::chrono::duration<_Rep, _Period>& breakpointDetectionDuration_)
{
	breakpointDetectionDuration = breakpointDetectionDuration;
}

template <class _Rep, class _Period>
void BreakpointDetector::setCheckDuration(const std::chrono::duration<_Rep, _Period>& checkDuration_)
{
	checkDuration = checkDuration;
}

namespace breakpoint_detector
{
	namespace _private
	{
		static BreakpointDetector instance;
	} // namespace _private

	static void addDetectedBreakpointCallback(DetectedBreakpointCallback detectedBreakpointCallback)
	{
		_private::instance.addDetectedBreakpointCallback(detectedBreakpointCallback);
	}

	static std::chrono::milliseconds getBreakpointTotalDuration() { return _private::instance.getBreakpointTotalDuration(); }

	template <class _Rep, class _Period>
	void setBreakpointDetectionDuration(const std::chrono::duration<_Rep, _Period>& breakpointDetectionDuration_)
	{
		_private::instance.setBreakpointDetectionDuration(breakpointDetectionDuration_);
	}

	template <class _Rep, class _Period> void setCheckDuration(const std::chrono::duration<_Rep, _Period>& checkDuration_)
	{
		_private::instance.setCheckDuration(checkDuration_);
	}

	namespace example
	{
		static void displayDetectedBreakpoint(const std::chrono::milliseconds& breakpointDuration)
		{
			std::cout << "Breakpoint detected: " << breakpointDuration.count() << "ms" << std::endl;
		}
	} // namespace example
} // namespace breakpoint_detector
