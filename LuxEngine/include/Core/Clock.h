#pragma once
#include <chrono>
#include <thread>

namespace lux
{
	using namespace std::chrono;
	using namespace std::chrono_literals;

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

		std::vector<void()> callbacks_start;
		std::vector<void()> callbacks_preupdate;
		std::vector<void()> callbacks_update;
		std::vector<void()> callbacks_postupdate;
		std::vector<void()> callbacks_physicsupdate;
	public:
		Clock(float t_fps);
		void Run();
		void Tick();
		void AddCallback(int clock_state, void (func)(void));
	};
}