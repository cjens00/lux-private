#pragma once

#include <functional>
#include <thread>
#include <mutex>

namespace lux
{
	class TimedWorkerThread
	{
		float time_elapsed = 0.00f;
		std::mutex mutex;
		std::function<float> function;
		std::condition_variable cv;
	public:
		TimedWorkerThread();
	};
}
