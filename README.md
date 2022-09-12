# Breakpoint Detector

# Description

Breakpoint Detector is a **single header c++ library** that allows you to **detect when the execution has been paused by a breakpoint** with the ability to **retrieve the elapsed time while the execution was paused**.

# Features

- Detect breakpoint
- Brekapoint detection configurable
- Callback on detection
- Multiple callback supported

# Installation

Include the file [`BreakpointDetector.hpp`](BreakpointDetector.hpp) in your project.

c++11 or later compilation required.  
No external dependencies.

# Example

*Screenshot of [example.cpp](example.cpp):*
[![breakpointExample](breakpointExample.png)](example.cpp)

<details>
  <summary><i>Show content of <a href="example.cpp">example.cpp</a></i></summary>


```c++
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
```
</details>

Possible output:
```
Hello
Breakpoint detected: 3001ms
Nice to meet you
Breakpoint detected: 1695ms
Bye
Total breakpoint duration : 4696ms
```

# Usage

```cpp

#define DECTECTED_BREAKPOINT_PARAM const std::chrono::milliseconds& breakpointDuration
// parameters are captures
#define DECTECTED_BREAKPOINT_LAMBDA(...) [__VA_ARGS__](DECTECTED_BREAKPOINT_PARAM)
using DetectedBreakpointCallback = std::function<void(DECTECTED_BREAKPOINT_PARAM)>;

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
		static void displayDetectedBreakpoint(DECTECTED_BREAKPOINT_PARAM);

	} // namespace example
} // namespace breakpoint_detector
```

**Warning: the breakpoint will not be immediately detected.**

# Licence

MIT Licence. See [LICENSE file](LICENSE).
Please refer me with:

	Copyright (c) Nicolas VENTER All rights reserved.
