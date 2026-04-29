#include "WindowsUtils.h"

#include <godot_cpp/classes/display_server.hpp>

bool WindowsUtils::SetMousePassThrough(const bool isEnabled) {
#if !defined(_WIN32)
	godot::UtilityFunctions::push_error("SetPassThrough is only supported on Windows.");
	return false;
#endif

	const godot::DisplayServer* ds = godot::DisplayServer::get_singleton();

	if (!ds) {
		godot::UtilityFunctions::push_error("DisplayServer is null; can't enable mouse pass-through.");
		return false;
	}

	// ds->window_set_flag(godot::DisplayServer::WINDOW_FLAG_MOUSE_PASSTHROUGH, isEnabled);

	const HWND hwnd = GetMainHWND();
	if (!hwnd) {
		godot::UtilityFunctions::push_error("window_get_native_handle returned 0; window may not be created yet.");
		return false;
	}

	// In case the returned HWND isn't the root window, apply changes on the top-level window.
	HWND target_hwnd = GetAncestor(hwnd, GA_ROOT);
	if (!target_hwnd) {
		target_hwnd = hwnd;
	}

	SetLastError(0);
	const LONG_PTR ex_style = GetWindowLongPtr(target_hwnd, GWL_EXSTYLE);
	if (ex_style == 0 && GetLastError() != 0) {
		godot::UtilityFunctions::push_error(
			"GetWindowLongPtr(GWL_EXSTYLE) failed. GetLastError=" + godot::String::num_int64((int64_t)GetLastError())
		);
		return false;
	}

	// NOTE: ex_style can legitimately be 0. Do not treat 0 as an error.
	LONG_PTR new_ex_style = ex_style;
	if (isEnabled) {
		// For transparent overlays on Windows, layered is commonly required/expected alongside transparency-related rendering.
		new_ex_style |= WS_EX_LAYERED;		// enables extended windows style
		new_ex_style |= WS_EX_TRANSPARENT;	// input passes through to underlying windows
		new_ex_style |= WS_EX_NOACTIVATE;	// keep overlay from stealing focus (common for "click-through" tools)
	} else {
		new_ex_style &= ~WS_EX_LAYERED;
		new_ex_style &= ~WS_EX_TRANSPARENT;
		new_ex_style &= ~WS_EX_NOACTIVATE;
	}

	SetLastError(0);

	const LONG_PTR prev = SetWindowLongPtr(target_hwnd, GWL_EXSTYLE, new_ex_style);
	if (prev == 0 && GetLastError() != 0) {
		godot::UtilityFunctions::push_error(
			"SetWindowLongPtr(GWL_EXSTYLE) failed. GetLastError=" + godot::String::num_int64(GetLastError())
		);
		return false;
	}

	// Force a frame change without changing geometry; avoid activating the window.
	// SetWindowPos(
	// 	target_hwnd,
	// 	nullptr,
	// 	0, 0, 0, 0,
	// 	SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED
	// );

	return true;
}
