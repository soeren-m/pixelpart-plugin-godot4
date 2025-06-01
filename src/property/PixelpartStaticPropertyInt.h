#ifndef PIXELPART_STATIC_PROPERTY_INT_H
#define PIXELPART_STATIC_PROPERTY_INT_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <pixelpart-runtime/common/Types.h>
#include <pixelpart-runtime/effect/StaticProperty.h>

namespace godot {
/**
 * @brief `int` property affected by effect inputs.
 *
 * Scene objects of an effect like particle emitters and force fields have properties that do not change over time,
 * but can be affected by effect inputs. Such properties are represented by one of the `PixelpartStaticProperty` classes.
 */
class PixelpartStaticPropertyInt : public RefCounted {
	GDCLASS(PixelpartStaticPropertyInt, RefCounted)

public:
	PixelpartStaticPropertyInt();

#ifdef DOXYGEN
	/**
	 * @brief Value without effect inputs taken into account.
	 */
	int base_value;
#endif

	void init(pixelpart::StaticProperty<pixelpart::int_t>* prop);

	/**
	 * @brief Return value with effect inputs taken into account.
	 *
	 * @return Value with effect inputs taken into account
	 */
	int value() const;

	/**
	 * @brief Set value without effect inputs taken into account.
	 *
	 * @param value Value without effect inputs taken into account
	 */
	void set_base_value(int value);

	/**
	 * @brief Return value without effect inputs taken into account.
	 *
	 * @return Value without effect inputs taken into account
	 */
	int get_base_value() const;

protected:
	static void _bind_methods();

private:
	pixelpart::StaticProperty<pixelpart::int_t>* property = nullptr;
};
}

#endif