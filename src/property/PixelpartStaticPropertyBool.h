#ifndef PIXELPART_STATIC_PROPERTY_BOOL_H
#define PIXELPART_STATIC_PROPERTY_BOOL_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <pixelpart-runtime/common/Types.h>
#include <pixelpart-runtime/effect/StaticProperty.h>

namespace godot {
/**
 * @brief `bool` property affected by effect inputs.
 *
 * Scene objects of an effect like particle emitters and force fields have properties that do not change over time,
 * but can be affected by effect inputs. Such properties are represented by one of the `PixelpartStaticProperty` classes.
 */
class PixelpartStaticPropertyBool : public RefCounted {
	GDCLASS(PixelpartStaticPropertyBool, RefCounted)

public:
	PixelpartStaticPropertyBool();

#ifdef DOXYGEN
	/**
	 * @brief Value without effect inputs taken into account.
	 */
	bool base_value;
#endif

	void init(pixelpart::StaticProperty<bool>* prop);

	/**
	 * @brief Return value with effect inputs taken into account.
	 *
	 * @return Value with effect inputs taken into account
	 */
	bool value() const;

	/**
	 * @brief Set value without effect inputs taken into account.
	 *
	 * @param value Value without effect inputs taken into account
	 */
	void set_base_value(bool value);

	/**
	 * @brief Return value without effect inputs taken into account.
	 *
	 * @return Value without effect inputs taken into account
	 */
	bool get_base_value() const;

protected:
	static void _bind_methods();

private:
	pixelpart::StaticProperty<bool>* property = nullptr;
};
}

#endif
