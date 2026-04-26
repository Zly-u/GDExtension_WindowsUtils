#include "WindowUtils.h"

#include "godot_cpp/core/class_db.hpp"

#include <windows.h>
#include <godot_cpp/classes/display_server.hpp>


void WindowUtils::_bind_methods() {
	godot::ClassDB::bind_static_method(get_class_static(), godot::D_METHOD("SetPassThrough", "isEnabled"), &WindowUtils::SetPassThrough);
}

WindowUtils::WindowUtils() {
    // Init shenanigans.
}

WindowUtils::~WindowUtils() {
    // Add your cleanup here.
}


HWND GetMainHWND() {
	const int64_t handle = godot::DisplayServer::get_singleton()->window_get_native_handle(
		godot::DisplayServer::HandleType::WINDOW_HANDLE, 0
	);

	return reinterpret_cast<HWND>(handle);
}


bool WindowUtils::SetPassThrough(const bool isEnabled) {
	const HWND hwnd = GetMainHWND();
	if (!hwnd) {
		godot::UtilityFunctions::push_error("Couldn't get HWND.");
		return false;
	}

	LONG_PTR WindowPtr = GetWindowLongPtr(hwnd, GWL_EXSTYLE);

	if (!WindowPtr) {
		godot::UtilityFunctions::push_error("Couldn't get WindowPtr.");
		return false;
	}

	if (isEnabled) {
		WindowPtr |= WS_EX_TRANSPARENT;
		WindowPtr |= WS_EX_NOACTIVATE;
	}
	else {
		WindowPtr &= ~WS_EX_TRANSPARENT;
		WindowPtr &= ~WS_EX_NOACTIVATE;
	}

	if (!SetWindowLongPtr(hwnd, GWL_EXSTYLE, WindowPtr)) {
		godot::UtilityFunctions::push_error("Couldn't set SetWindowLongPtr.");
		return false;
	}

	if (!SetWindowPos(
		hwnd, nullptr,
		0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED
	)) {
		godot::UtilityFunctions::push_error("Couldn't refresh the style.");
		return false;
	}

	return true;
}
