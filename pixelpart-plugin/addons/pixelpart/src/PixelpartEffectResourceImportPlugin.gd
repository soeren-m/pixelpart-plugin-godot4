extends EditorImportPlugin

func _get_importer_name():
	return "pixelpart.effect"

func _get_visible_name():
	return "Pixelpart Effect";

func _get_recognized_extensions():
	return ["ppfx"]

func _get_import_order():
	return 1

func _get_save_extension():
	return "res";

func _get_resource_type():
	return "Resource";

func _get_priority():
	return 1.0;

enum Presets { DEFAULT }

func _get_preset_count():
	return Presets.size()

func _get_preset_name(preset):
	match preset:
		Presets.DEFAULT:
			return "Default"
		_:
			return "Unknown"

func _get_import_options(path, preset):
	return []

func _get_option_visibility(path, option, options):
	return true

func _import(source_file, save_path, options, r_platform_variants, r_gen_files):
	var effect = PixelpartEffectResource.new();
	effect.import(source_file)

	return ResourceSaver.save(effect, "%s.%s" % [save_path, _get_save_extension()])
