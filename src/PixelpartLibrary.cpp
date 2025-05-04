#include "PixelpartLibrary.h"
#include "PixelpartSystem.h"
#include "PixelpartEffectResource.h"
#include "PixelpartEffect.h"
#include "PixelpartEffect2D.h"
#include "particletype/PixelpartParticleType.h"
#include "node/PixelpartGroupNode.h"
#include "node/PixelpartParticleEmitter.h"
#include "node/PixelpartAttractionField.h"
#include "node/PixelpartAccelerationField.h"
#include "node/PixelpartVectorField.h"
#include "node/PixelpartNoiseField.h"
#include "node/PixelpartDragField.h"
#include "node/PixelpartLineCollider.h"
#include "node/PixelpartPlaneCollider.h"
#include "node/PixelpartDirectionalLightSource.h"
#include "node/PixelpartPointLightSource.h"
#include "node/PixelpartSpotLightSource.h"
#include "property/PixelpartStaticPropertyBool.h"
#include "property/PixelpartStaticPropertyInt.h"
#include "property/PixelpartStaticPropertyFloat.h"
#include "property/PixelpartStaticPropertyFloat2.h"
#include "property/PixelpartStaticPropertyFloat3.h"
#include "property/PixelpartStaticPropertyFloat4.h"
#include "property/PixelpartAnimatedPropertyFloat.h"
#include "property/PixelpartAnimatedPropertyFloat2.h"
#include "property/PixelpartAnimatedPropertyFloat3.h"
#include "property/PixelpartAnimatedPropertyFloat4.h"
#include <gdextension_interface.h>
#include <godot_cpp/godot.hpp>

godot::PixelpartSystem* pixelpartSystem = nullptr;

void pixelpart_register(godot::ModuleInitializationLevel level) {
	if(level != godot::MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	godot::ClassDB::register_class<godot::PixelpartStaticPropertyBool>();
	godot::ClassDB::register_class<godot::PixelpartStaticPropertyInt>();
	godot::ClassDB::register_class<godot::PixelpartStaticPropertyFloat>();
	godot::ClassDB::register_class<godot::PixelpartStaticPropertyFloat2>();
	godot::ClassDB::register_class<godot::PixelpartStaticPropertyFloat3>();
	godot::ClassDB::register_class<godot::PixelpartStaticPropertyFloat4>();
	godot::ClassDB::register_class<godot::PixelpartAnimatedPropertyFloat>();
	godot::ClassDB::register_class<godot::PixelpartAnimatedPropertyFloat2>();
	godot::ClassDB::register_class<godot::PixelpartAnimatedPropertyFloat3>();
	godot::ClassDB::register_class<godot::PixelpartAnimatedPropertyFloat4>();
	godot::ClassDB::register_class<godot::PixelpartNode>();
	godot::ClassDB::register_class<godot::PixelpartGroupNode>();
	godot::ClassDB::register_class<godot::PixelpartParticleEmitter>();
	godot::ClassDB::register_class<godot::PixelpartForceField>();
	godot::ClassDB::register_class<godot::PixelpartAttractionField>();
	godot::ClassDB::register_class<godot::PixelpartAccelerationField>();
	godot::ClassDB::register_class<godot::PixelpartVectorField>();
	godot::ClassDB::register_class<godot::PixelpartNoiseField>();
	godot::ClassDB::register_class<godot::PixelpartDragField>();
	godot::ClassDB::register_class<godot::PixelpartCollider>();
	godot::ClassDB::register_class<godot::PixelpartLineCollider>();
	godot::ClassDB::register_class<godot::PixelpartPlaneCollider>();
	godot::ClassDB::register_class<godot::PixelpartLightSource>();
	godot::ClassDB::register_class<godot::PixelpartDirectionalLightSource>();
	godot::ClassDB::register_class<godot::PixelpartPointLightSource>();
	godot::ClassDB::register_class<godot::PixelpartSpotLightSource>();
	godot::ClassDB::register_class<godot::PixelpartParticleType>();
	godot::ClassDB::register_class<godot::PixelpartEffectResource>();
	godot::ClassDB::register_class<godot::PixelpartEffect2D>();
	godot::ClassDB::register_class<godot::PixelpartEffect>();

	if(!pixelpartSystem) {
		pixelpartSystem = new godot::PixelpartSystem();
	}
}
void pixelpart_unregister(godot::ModuleInitializationLevel level) {
	if(level != godot::MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	if(pixelpartSystem) {
		delete pixelpartSystem;
		pixelpartSystem = nullptr;
	}
}

extern "C" {
GDExtensionBool GDE_EXPORT pixelpart_init(GDExtensionInterfaceGetProcAddress procAddress, GDExtensionClassLibraryPtr library, GDExtensionInitialization* initialization) {
	godot::GDExtensionBinding::InitObject initObj(procAddress, library, initialization);
	initObj.register_initializer(pixelpart_register);
	initObj.register_terminator(pixelpart_unregister);
	initObj.set_minimum_library_initialization_level(godot::MODULE_INITIALIZATION_LEVEL_SCENE);

	return initObj.init();
}
}