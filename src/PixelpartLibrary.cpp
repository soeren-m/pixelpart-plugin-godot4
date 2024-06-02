#include "PixelpartLibrary.h"
#include "PixelpartEffect.h"
#include "PixelpartEffect2D.h"
#include "property/PixelpartStaticPropertyFloat2.h"
#include "property/PixelpartAnimatedPropertyInt.h"
#include "property/PixelpartAnimatedPropertyFloat2.h"
#include "PixelpartSystem.h"
#include <gdextension_interface.h>
#include <godot_cpp/godot.hpp>

godot::PixelpartSystem* pixelpartSystem = nullptr;

void pixelpart_register(godot::ModuleInitializationLevel p_level) {
	if(p_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	godot::ClassDB::register_class<godot::PixelpartStaticPropertyBool>();
	godot::ClassDB::register_class<godot::PixelpartStaticPropertyInt>();
	godot::ClassDB::register_class<godot::PixelpartStaticPropertyFloat>();
	godot::ClassDB::register_class<godot::PixelpartStaticPropertyFloat2>();
	godot::ClassDB::register_class<godot::PixelpartStaticPropertyFloat3>();
	godot::ClassDB::register_class<godot::PixelpartStaticPropertyFloat4>();
	godot::ClassDB::register_class<godot::PixelpartAnimatedPropertyInt>();
	godot::ClassDB::register_class<godot::PixelpartAnimatedPropertyFloat>();
	godot::ClassDB::register_class<godot::PixelpartAnimatedPropertyFloat2>();
	godot::ClassDB::register_class<godot::PixelpartAnimatedPropertyFloat3>();
	godot::ClassDB::register_class<godot::PixelpartAnimatedPropertyFloat4>();
	godot::ClassDB::register_class<godot::PixelpartEffectResource>();
	godot::ClassDB::register_class<godot::PixelpartEffect>();
	godot::ClassDB::register_class<godot::PixelpartEffect2D>();
	godot::ClassDB::register_class<godot::PixelpartParticleEmitter>();
	godot::ClassDB::register_class<godot::PixelpartParticleType>();
	godot::ClassDB::register_class<godot::PixelpartForceField>();
	godot::ClassDB::register_class<godot::PixelpartCollider>();

	if(!pixelpartSystem) {
		pixelpartSystem = new godot::PixelpartSystem();
	}
}
void pixelpart_unregister(godot::ModuleInitializationLevel p_level) {
	if(p_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	if(pixelpartSystem) {
		delete pixelpartSystem;
		pixelpartSystem = nullptr;
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