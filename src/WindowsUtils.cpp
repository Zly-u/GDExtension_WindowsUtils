#include "WindowsUtils.h"

#include "godot_cpp/core/class_db.hpp"

#include <godot_cpp/classes/display_server.hpp>

void WindowsUtils::_bind_methods() {
	godot::ClassDB::bind_static_method(
		get_class_static(),
		godot::D_METHOD("SetMousePassThrough", "isEnabled"),
		&WindowsUtils::SetMousePassThrough
	);



	godot::ClassDB::bind_method(
		godot::D_METHOD("StartKeyPolling"),
		&WindowsUtils::StartKeyPolling
	);
	godot::ClassDB::bind_method(
		godot::D_METHOD("StopKeyPolling"),
		&WindowsUtils::StopKeyPolling
	);


	ADD_SIGNAL(godot::MethodInfo(SignalName_EmitStartedPollingKeys));
	godot::ClassDB::bind_method(
		godot::D_METHOD("emit_on_started_polling_keys"),
		&WindowsUtils::emit_on_started_polling_keys
	);

	ADD_SIGNAL(godot::MethodInfo(SignalName_EmitAnyKeyPressed));
	godot::ClassDB::bind_method(
		godot::D_METHOD("emit_any_key_pressed_deferred"),
		&WindowsUtils::emit_any_key_pressed_deferred
	);

	ADD_SIGNAL(godot::MethodInfo(SignalName_EmitStoppedPollingKeys));
	godot::ClassDB::bind_method(
		godot::D_METHOD("emit_on_stopped_polling_keys"),
		&WindowsUtils::emit_on_stopped_polling_keys
	);
}

WindowsUtils::WindowsUtils() {
    // Init shenanigans.
}

WindowsUtils::~WindowsUtils() {
    // Add your cleanup here.
	StopKeyPolling();
}


HWND WindowsUtils::GetMainHWND() {
#if !defined(_WIN32)
	return nullptr;
#endif

	// In practice this should be the Win32 HWND for the main window, but be defensive: if it's not valid,
	// the Win32 calls below will fail and we'll surface GetLastError().
	const int64_t handle = godot::DisplayServer::get_singleton()->window_get_native_handle(
		godot::DisplayServer::HandleType::WINDOW_HANDLE, 0
	);

	return reinterpret_cast<HWND>(static_cast<intptr_t>(handle));
}