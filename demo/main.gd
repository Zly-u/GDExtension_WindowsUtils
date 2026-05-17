extends Node2D


func _ready() -> void:
	WindowsUtils.on_started_polling_keys.connect(testStart);
	WindowsUtils.winAPI_any_key_pressed.connect(testPress);
	WindowsUtils.on_stopped_polling_keys.connect(testStop);
	
	WindowsUtils.StartKeyPolling();
	
	WindowsUtils.SetMousePassThrough(true);
	
	pass

func testStart():
	print("start")

func testPress():
	print("pressed")

func testStop():
	print("stop")

func _process(delta: float) -> void:
	pass
