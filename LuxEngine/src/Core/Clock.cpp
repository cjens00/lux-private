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

		for (auto fn : callbacks_start)
		{
			fn();
		}
		break;
	case PreUpdate:

		for (auto fn : callbacks_preupdate)
		{
			fn();
		}
		break;
	case Update:

		for (auto fn : callbacks_update)
		{
			fn();
		}
		break;
	case PostUpdate:

		for (auto fn : callbacks_postupdate)
		{
			fn();
		}
		break;
	case PhysicsUpdate:

		if (updatePhysics)
		{
			for (auto fn : callbacks_physicsupdate)
			{
				fn();
			}
		}
		break;
	}
	state++;
}

void lux::Clock::Run()
{
	pass_start = steady_clock::now();
	while (state <= 4) 
		this->Tick();
	state = 1;
	if (updatePhysics)
		updatePhysics = false;
	else
		updatePhysics = true;
	pass_end = steady_clock::now();

	auto x = (target_frame_time - (pass_end - pass_start));
	std::this_thread::sleep_for(x);
}

void lux::Clock::AddCallback(int clock_state, void_function vf)
{
	if (clock_state == Start)
	{
		callbacks_start.emplace_back(vf);
	}
	if (clock_state == PreUpdate)
	{
		callbacks_preupdate.emplace_back(vf);
	}
	if (clock_state == Update)
	{
		callbacks_update.emplace_back(vf);
	}
	if (clock_state == PostUpdate)
	{
		callbacks_postupdate.emplace_back(vf);
	}
	if (clock_state == PhysicsUpdate)
	{
		callbacks_physicsupdate.emplace_back(vf);
	}
}
