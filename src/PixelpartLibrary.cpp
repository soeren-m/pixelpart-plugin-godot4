#include "PixelpartLibrary.h"
#include "PixelpartEffect.h"
#include "PixelpartEffect2D.h"
#include "PixelpartShaders.h"
#include <gdextension_interface.h>
#include <godot_cpp/godot.hpp>

godot::PixelpartShaders* shaders = nullptr;

void pixelpart_register(godot::ModuleInitializationLevel p_level) {
	if(p_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	godot::ClassDB::register_class<godot::PixelpartCurve>();
	godot::ClassDB::register_class<godot::PixelpartCurve3>();
	godot::ClassDB::register_class<godot::PixelpartGradient>();
	godot::ClassDB::register_class<godot::PixelpartEffectResource>();
	godot::ClassDB::register_class<godot::PixelpartEffect>();
	godot::ClassDB::register_class<godot::PixelpartEffect2D>();
	godot::ClassDB::register_class<godot::PixelpartParticleEmitter>();
	godot::ClassDB::register_class<godot::PixelpartParticleType>();
	godot::ClassDB::register_class<godot::PixelpartForceField>();
	godot::ClassDB::register_class<godot::PixelpartCollider>();

	if(!shaders) {
		shaders = new godot::PixelpartShaders();
	}
}
void pixelpart_unregister(godot::ModuleInitializationLevel p_level) {
	if(p_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	if(shaders) {
		delete shaders;
		shaders = nullptr;
	}
}

extern "C" {
GDExtensionBool GDE_EXPORT pixelpart_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization* r_initialization) {
	godot::GDExtensionBinding::InitObject initObj(p_get_proc_address, p_library, r_initialization);

	initObj.register_initializer(pixelpart_register);
	initObj.register_terminator(pixelpart_unregister);
	initObj.set_minimum_library_initialization_level(godot::MODULE_INITIALIZATION_LEVEL_SCENE);

	return initObj.init();
}
}