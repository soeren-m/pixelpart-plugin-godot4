extends EditorInspectorPlugin

func _can_handle(object):
	return object is PixelpartEffect or object is PixelpartEffect2D

func _parse_group(object, group):
	if group != "Playback":
		return

	var restart_button = Button.new()
	restart_button.text = "Restart"
	restart_button.pressed.connect(_on_restart_button_pressed.bind(object))

	add_custom_control(restart_button)

func _on_restart_button_pressed(object):
	object.restart()
