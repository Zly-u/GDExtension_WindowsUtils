#pragma once

#include <godot_cpp/classes/object.hpp>

#include <thread>
#include <stop_token>

#if defined(_WIN32)
#include <windows.h>
#endif




class WindowsUtils : public godot::Object
{
    GDCLASS(WindowsUtils, godot::Object);

	public:
	    WindowsUtils();
	    ~WindowsUtils() override;

		static WindowsUtils* get_singleton() {
			if (!instance) {
				instance = memnew(WindowsUtils);
			}
			return instance;
		}

		static void delete_singleton(){
			memdelete(instance);
			instance = nullptr;
		};


	public:
		static HWND GetMainHWND();

	public:
		// Window Pass through for the mouse cursor
		static bool SetMousePassThrough(bool isEnabled);

	public:
		void StartKeyPolling();
		void StopKeyPolling();


	private:
		void Worker_KeyPoller(const std::stop_token& token);
		void emit_on_started_polling_keys();
		void emit_any_key_pressed_deferred();
		void emit_on_stopped_polling_keys();

	private:
		inline static auto SignalName_EmitStartedPollingKeys	= "on_started_polling_keys";
		inline static auto SignalName_EmitAnyKeyPressed			= "winAPI_any_key_pressed";
		inline static auto SignalName_EmitStoppedPollingKeys	= "on_stopped_polling_keys";

		godot::TypedArray<bool> UserInput;
		std::unique_ptr<std::jthread> Worker;


protected:
	    static void _bind_methods();


	private:
		inline static WindowsUtils* instance = nullptr;
};
