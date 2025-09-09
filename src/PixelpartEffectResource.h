#ifndef PIXELPART_EFFECT_RESOURCE_H
#define PIXELPART_EFFECT_RESOURCE_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <pixelpart-runtime/asset/EffectAsset.h>

namespace godot {
/**
 * @brief Resource for a Pixelpart effect that is contained in a @a ppfx file.
 *
 * This class is derived from the Godot `Resource` class and stores the effect
 * as a byte array.
 */
class PixelpartEffectResource : public Resource {
	GDCLASS(PixelpartEffectResource, Resource)

public:
	PixelpartEffectResource();
	virtual ~PixelpartEffectResource();

#ifdef DOXYGEN
	/**
	 * @brief Effect data as byte array.
	 */
	PackedByteArray data;
#endif

	/**
	 * @brief Import data from a @a ppfx file.
	 *
	 * @param filepath File to import
	 */
	void import(String filepath);

	/**
	 * @brief Load the imported effect data.
	 *
	 * The effect data needs to be imported with @ref import "import(String filepath)" before calling this method.
	 */
	void load();

	/**
	 * @brief Release the loaded effect.
	 *
	 */
	void release();

	/**
	 * @brief Set the effect data manually.
	 *
	 * @param bytes Effect data as byte array
	 */
	void set_data(PackedByteArray bytes);

	/**
	 * @brief Return the effect data.
	 *
	 * @return Byte array of the effect data
	 */
	PackedByteArray get_data() const;

	const pixelpart::EffectAsset& get_asset() const;

protected:
	static void _bind_methods();

private:
	PackedByteArray data;

	pixelpart::EffectAsset effectAsset;
	bool loaded = false;
};
}

#endif
