#include <Core/Clock.h>


lux::Clock::Clock(float t_fps)
{
	target_frame_time = duration<float, std::milli>
	{
		(1000.0f / t_fps)
	};
}

int lux::Clock::Tick()
{
	switch (state)
	{
	case Start:
		callback_start;
	case PreUpdate:
		callback_preupdate;
	case Update:
		callback_update;
	case PostUpdate:
		callback_postupdate;
	case PhysicsUpdate:
		if (updatePhysics)
		{
			callback_physicsupdate;
		}
		else
		{
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
