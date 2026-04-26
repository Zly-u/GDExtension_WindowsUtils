#include <chrono>

#include "WindowsUtils.h"
#include "godot_cpp/variant/utility_functions.hpp"


__forceinline bool IsKeyDown(const SHORT State)
{
	return State & 0x8000;
}


void WindowsUtils::StartKeyPolling()
{
	Worker = std::make_unique<std::jthread>(
		[this](const std::stop_token& token) {
			Worker_KeyPoller(token);
		}
	);

	Worker->detach();
}


void WindowsUtils::StopKeyPolling()
{
	if (Worker->joinable()) {
		Worker->request_stop();
		Worker->join();
	}
}


void WindowsUtils::Worker_KeyPoller(const std::stop_token& token)
{
	call_deferred("emit_on_started_polling_keys");

	UserInput.resize(255);

	while (true) {
		if (token.stop_requested()) {
			break;
		}

		bool any_key_down = false;

		for (uint8_t vk_id = 0; vk_id < 255; ++vk_id) {
			if (token.stop_requested()) {
				break;
			}

			const SHORT key_state = GetAsyncKeyState(vk_id);

			const bool is_key_down = IsKeyDown(key_state);
			godot::Variant& prev_key_state = UserInput[vk_id];

			any_key_down = is_key_down && !prev_key_state.booleanize();

			prev_key_state = is_key_down;

			if (any_key_down) {
				call_deferred("emit_any_key_pressed_deferred");
				break;
			}
		}

		if (token.stop_requested()) {
			break;
		}


		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	UserInput.clear();

	call_deferred("emit_on_stopped_polling_keys");
}


void WindowsUtils::emit_on_started_polling_keys() {
	emit_signal(SignalName_EmitStartedPollingKeys);
}


void WindowsUtils::emit_any_key_pressed_deferred() {
	emit_signal(SignalName_EmitAnyKeyPressed);
}


void WindowsUtils::emit_on_stopped_polling_keys() {
	emit_signal(SignalName_EmitStoppedPollingKeys);
}
