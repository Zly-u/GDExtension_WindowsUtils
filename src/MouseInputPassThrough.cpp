#include "WindowsUtils.h"

#include <godot_cpp/classes/display_server.hpp>

bool WindowsUtils::SetMousePassThrough(const bool isEnabled) {
#if !defined(_WIN32)
	godot::UtilityFunctions::push_error("SetMousePassThrough is only supported on Windows.");
	return false;
#else

	godot::DisplayServer *ds = godot::DisplayServer::get_singleton();
	if (!ds) {
		godot::UtilityFunctions::push_error("DisplayServer is null; can't enable mouse pass-through.");
		return false;
	}

	const HWND hwnd = GetMainHWND();
	if (!hwnd) {
		godot::UtilityFunctions::push_error("window_get_native_handle returned 0; window may not be created yet.");
		return false;
	}

	HWND target_hwnd = GetAncestor(hwnd, GA_ROOT);
	if (!target_hwnd) {
		target_hwnd = hwnd;
	}

	constexpr uint32_t k_main_win = 0;

	SetLastError(0);
	const LONG_PTR ex_style = GetWindowLongPtr(target_hwnd, GWL_EXSTYLE);
	if (ex_style == 0 && GetLastError() != 0) {
		godot::UtilityFunctions::push_error("GetWindowLongPtr(GWL_EXSTYLE) failed. GetLastError=" + godot::String::num_int64(GetLastError()));
		return false;
	}

	// This is the ONLY way i could figure how to make the window to REFRESH its state
	// ds->window_set_flag(godot::DisplayServer::WINDOW_FLAG_NO_FOCUS, isEnabled, k_main_win);

	LONG_PTR new_ex_style = ex_style;
	if (isEnabled) {
		new_ex_style |= WS_EX_LAYERED;
		new_ex_style |= WS_EX_TRANSPARENT;
		new_ex_style |= WS_EX_NOACTIVATE;
	} else {
		new_ex_style &= ~WS_EX_TRANSPARENT;
		new_ex_style &= ~WS_EX_NOACTIVATE;
	}

	SetLastError(0);
	const LONG_PTR prev = SetWindowLongPtr(target_hwnd, GWL_EXSTYLE, new_ex_style);
	if (prev == 0 && GetLastError() != 0) {
		godot::UtilityFunctions::push_error(
			"SetWindowLongPtr(GWL_EXSTYLE) failed. GetLastError=" + godot::String::num_int64(GetLastError()));
		return false;
	}

	return true;
#endif
}
