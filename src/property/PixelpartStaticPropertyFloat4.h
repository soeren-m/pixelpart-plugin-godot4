#ifndef PIXELPART_STATIC_PROPERTY_FLOAT4_H
#define PIXELPART_STATIC_PROPERTY_FLOAT4_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <pixelpart-runtime/common/Math.h>
#include <pixelpart-runtime/effect/StaticProperty.h>

namespace godot {
/**
 * @brief `Vector4` property affected by effect inputs.
 *
 * Scene objects of an effect like particle emitters and force fields have properties that do not change over time,
 * but can be affected by effect inputs. Such properties are represented by one of the `PixelpartStaticProperty` classes.
 */
class PixelpartStaticPropertyFloat4 : public RefCounted {
	GDCLASS(PixelpartStaticPropertyFloat4, RefCounted)

public:
	PixelpartStaticPropertyFloat4();

#ifdef DOXYGEN
	/**
	 * @brief Value without effect inputs taken into account.
	 */
	Vector4 base_value;
#endif

	void init(pixelpart::StaticProperty<pixelpart::float4_t>* prop);

	/**
	 * @brief Return value with effect inputs taken into account.
	 *
	 * @return Value with effect inputs taken into account
	 */
	Vector4 value() const;

	/**
	 * @brief Set value without effect inputs taken into account.
	 *
	 * @param value Value without effect inputs taken into account
	 */
	void set_base_value(Vector4 value);

	/**
	 * @brief Return value without effect inputs taken into account.
	 *
	 * @return Value without effect inputs taken into account
	 */
	Vector4 get_base_value() const;

protected:
	static void _bind_methods();

private:
	pixelpart::StaticProperty<pixelpart::float4_t>* property = nullptr;
};
}

#endif
