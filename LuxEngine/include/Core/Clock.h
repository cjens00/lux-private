#pragma once
#include <chrono>
#include <functional>
#include <thread>

namespace lux
{
	using namespace std::chrono;
	using namespace std::chrono_literals;
	typedef std::function<void(void)> void_function;

	enum Clock_States
	{
		Start = 0,
		PreUpdate = 1,
		Update = 2,
		PostUpdate = 3,
		PhysicsUpdate = 4,
	};

	class Clock
	{
		int state = 0;
		bool updatePhysics = false;
		time_point<steady_clock> pass_start;
		time_point<steady_clock> pass_end;
		duration<float, std::milli> target_frame_time = 9.0ms;

		std::vector<void_function> callbacks_start;
		std::vector<void_function> callbacks_preupdate;
		std::vector<void_function> callbacks_update;
		std::vector<void_function> callbacks_postupdate;
		std::vector<void_function> callbacks_physicsupdate;
	public:
		Clock(float t_fps);
		void Run();
		void Tick();
		void AddCallback(int clock_state, void (func)(void));
	};
}