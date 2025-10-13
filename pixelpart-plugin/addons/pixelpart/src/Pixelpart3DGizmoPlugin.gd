extends EditorNode3DGizmoPlugin

const plugin_path = "res://addons/pixelpart"

func _init():
	var gizmo_icon = load(plugin_path + "/pixelpart-gizmo.png") as Texture
	create_icon_material("pixelpart_icon", gizmo_icon)

func _get_gizmo_name():
	return "PixelpartEffectGizmo"

func _get_priority():
	return 0

func _has_gizmo(node):
	return node is PixelpartEffect

func _is_selectable_when_hidden():
	return false

func _redraw(gizmo):
	gizmo.clear()
	gizmo.add_unscaled_billboard(get_material("pixelpart_icon", gizmo), 8.0 / 128.0)
