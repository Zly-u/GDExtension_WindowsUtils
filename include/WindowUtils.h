#pragma once

#include <godot_cpp/classes/object.hpp>

class WindowUtils : public godot::Object
{
    GDCLASS(WindowUtils, godot::Object);

	public:
	    WindowUtils();
	    ~WindowUtils() override;

		static WindowUtils* get_singleton() {
			if (!instance) {
				instance = memnew(WindowUtils);
			}
			return instance;
		}

		static void delete_singleton(){
			memdelete(instance);
			instance = nullptr;
		};


	public:
		// Window Pass through for the mouse cursor
		static bool SetPassThrough(bool isEnabled);


	protected:
	    static void _bind_methods();


	private:
		inline static WindowUtils* instance = nullptr;
};
