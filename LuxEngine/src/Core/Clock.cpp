#include <Core/Clock.h>


lux::Clock::Clock(float t_fps)
{
	target_frame_time = duration<float, std::milli>
	{
		(1000.0f / t_fps)
	};
}

void lux::Clock::Tick()
{
	switch (state)
	{
	case Start:
		for(auto fn: callbacks_start)
		{
			fn();
		}
	case PreUpdate:
		for (auto fn : callbacks_preupdate)
		{
			fn();
		}
	case Update:
		for (auto fn : callbacks_update)
		{
			fn();
		}
	case PostUpdate:
		for (auto fn : callbacks_postupdate)
		{
			fn();
		}
	case PhysicsUpdate:
		if (updatePhysics)
		{
			for (auto fn : callbacks_physicsupdate)
			{
				fn();
			}
		}
		state = 0;
	}
	state++;
}

void lux::Clock::Run()
{
	pass_start = steady_clock::now();
	while (state <= 4)
	{
		this->Tick();
	}
	pass_end = steady_clock::now();
	std::this_thread::sleep_for(target_frame_time - (pass_end - pass_start));
	Run();
}
