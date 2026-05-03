@tool
extends EditorPlugin

const plugin_path = "res://addons/pixelpart"

var resource_import_plugin
var gizmo_plugin
var inspector_plugin

func _enter_tree():
	add_project_setting("pixelpart/editor_preview", true, TYPE_BOOL, PROPERTY_HINT_NONE, "")

	resource_import_plugin = load(plugin_path + "/src/PixelpartEffectResourceImportPlugin.gd").new()
	add_import_plugin(resource_import_plugin)

	gizmo_plugin = load(plugin_path + "/src/Pixelpart3DGizmoPlugin.gd").new()
	add_node_3d_gizmo_plugin(gizmo_plugin)

	inspector_plugin = load(plugin_path + "/src/PixelpartInspectorPlugin.gd").new()
	add_inspector_plugin(inspector_plugin)

func _exit_tree():
	remove_inspector_plugin(inspector_plugin)
	remove_node_3d_gizmo_plugin(gizmo_plugin)
	remove_import_plugin(resource_import_plugin)
	remove_project_setting("pixelpart/editor_preview")

func add_project_setting(name: String, initial_value, type: int, hint: int, hint_string: String) -> void:
	if ProjectSettings.has_setting(name):
		return

	var property_info: Dictionary = {
		"name": name,
		"type": type,
		"hint": hint,
		"hint_string": hint_string
	}

	ProjectSettings.set_setting(name, initial_value)
	ProjectSettings.add_property_info(property_info)
	ProjectSettings.set_initial_value(name, initial_value)

func remove_project_setting(name: String):
	ProjectSettings.clear(name)
