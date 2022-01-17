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

		void* callback_start;
		void* callback_preupdate;
		void* callback_update;
		void* callback_postupdate;
		void* callback_physicsupdate;
	public:
		Clock(float t_fps);
		void Run();
		int Tick();
	};
}