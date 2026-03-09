#include "PixelpartParticleEmitter.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>
#include <pixelpart-runtime/math/Geometry.h>
#include <algorithm>

namespace godot {
PixelpartParticleEmitter::PixelpartParticleEmitter() : PixelpartNode() {

}
PixelpartParticleEmitter::~PixelpartParticleEmitter() {

}

void PixelpartParticleEmitter::init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) {
	PixelpartNode::init(internalNode, effectEnginePtr);

	particleEmitter = dynamic_cast<pixelpart::ParticleEmitter*>(internalNode);
}

void PixelpartParticleEmitter::set_shape(ShapeType type) {
	if(!particleEmitter) {
		return;
	}

	particleEmitter->shape(static_cast<pixelpart::ParticleEmitter::Shape>(type));
}
PixelpartParticleEmitter::ShapeType PixelpartParticleEmitter::get_shape() const {
	if(!particleEmitter) {
		return SHAPE_POINT;
	}

	return static_cast<ShapeType>(particleEmitter->shape());
}
void PixelpartParticleEmitter::add_shape_point(Vector3 point) {
	if(!particleEmitter) {
		return;
	}

	std::vector<pixelpart::float3_t> points = get_shape_points();
	points.push_back(gd_to_pxpt(point));

	set_shape_points(points);
}
void PixelpartParticleEmitter::remove_shape_point(int index) {
	if(!particleEmitter) {
		return;
	}

	std::vector<pixelpart::float3_t> points = get_shape_points();
	if(index < 0 || index >= static_cast<int>(points.size())) {
		return;
	}

	points.erase(points.begin() + index);
	set_shape_points(points);
}
void PixelpartParticleEmitter::set_shape_point(int index, Vector3 point) {
	if(!particleEmitter) {
		return;
	}

	std::vector<pixelpart::float3_t> points = get_shape_points();
	if(index < 0 || index > static_cast<int>(points.size())) {
		return;
	}

	points[index] = gd_to_pxpt(point);
	set_shape_points(points);
}
Vector3 PixelpartParticleEmitter::get_shape_point(int index) const {
	if(!particleEmitter || index < 0 || index >= get_shape_point_count()) {
		return Vector3(0.0f, 0.0f, 0.0f);
	}

	return pxpt_to_gd(particleEmitter->path().point(static_cast<std::size_t>(index)).value);
}
int PixelpartParticleEmitter::get_shape_point_count() const {
	return particleEmitter ? static_cast<int>(particleEmitter->path().pointCount()) : 0;
}

void PixelpartParticleEmitter::set_distribution(DistributionType mode) {
	if(!particleEmitter) {
		return;
	}

	particleEmitter->distribution(static_cast<pixelpart::ParticleEmitter::Distribution>(mode));
}
PixelpartParticleEmitter::DistributionType PixelpartParticleEmitter::get_distribution() const {
	if(!particleEmitter) {
		return DISTRIBUTION_UNIFORM;
	}

	return static_cast<DistributionType>(particleEmitter->distribution());
}
void PixelpartParticleEmitter::set_grid_order(GridOrderType mode) {
	if(!particleEmitter) {
		return;
	}

	particleEmitter->gridOrder(static_cast<pixelpart::ParticleEmitter::GridOrder>(mode));
}
PixelpartParticleEmitter::GridOrderType PixelpartParticleEmitter::get_grid_order() const {
	if(!particleEmitter) {
		return GRID_ORDER_X_Y_Z;
	}

	return static_cast<GridOrderType>(particleEmitter->gridOrder());
}
void PixelpartParticleEmitter::set_grid_size(int x, int y, int z) {
	if(!particleEmitter) {
		return;
	}

	particleEmitter->gridSize(
		static_cast<std::uint32_t>(x),
		static_cast<std::uint32_t>(y),
		static_cast<std::uint32_t>(z));
}
int PixelpartParticleEmitter::get_grid_size_x() const {
	return particleEmitter ? static_cast<int>(particleEmitter->gridSizeX()) : 0;
}
int PixelpartParticleEmitter::get_grid_size_y() const {
	return particleEmitter ? static_cast<int>(particleEmitter->gridSizeY()) : 0;
}
int PixelpartParticleEmitter::get_grid_size_z() const {
	return particleEmitter ? static_cast<int>(particleEmitter->gridSizeZ()) : 0;
}
void PixelpartParticleEmitter::set_emission_mode(EmissionMode mode) {
	if(!particleEmitter) {
		return;
	}

	particleEmitter->emissionMode(static_cast<pixelpart::ParticleEmitter::EmissionMode>(mode));
}
PixelpartParticleEmitter::EmissionMode PixelpartParticleEmitter::get_emission_mode() const {
	if(!particleEmitter) {
		return EMISSION_CONTINUOUS;
	}

	return static_cast<EmissionMode>(particleEmitter->emissionMode());
}
void PixelpartParticleEmitter::set_direction_mode(DirectionMode mode) {
	if(!particleEmitter) {
		return;
	}

	particleEmitter->directionMode(static_cast<pixelpart::ParticleEmitter::DirectionMode>(mode));
}
PixelpartParticleEmitter::DirectionMode PixelpartParticleEmitter::get_direction_mode() const {
	if(!particleEmitter) {
		return DIRECTION_FIXED;
	}

	return static_cast<DirectionMode>(particleEmitter->directionMode());
}

Ref<PixelpartAnimatedPropertyFloat3> PixelpartParticleEmitter::get_direction() const {
	if(!particleEmitter) {
		return Ref<PixelpartAnimatedPropertyFloat3>();
	}

	Ref<PixelpartAnimatedPropertyFloat3> property;
	property.instantiate();
	property->init(&particleEmitter->direction());

	return property;
}
Ref<PixelpartAnimatedPropertyFloat> PixelpartParticleEmitter::get_spread() const {
	if(!particleEmitter) {
		return Ref<PixelpartAnimatedPropertyFloat>();
	}

	Ref<PixelpartAnimatedPropertyFloat> property;
	property.instantiate();
	property->init(&particleEmitter->spread());

	return property;
}

void PixelpartParticleEmitter::set_shape_points(const std::vector<pixelpart::float3_t>& points) {
	std::vector<pixelpart::float_t> distances(points.size(), 0.0);
	pixelpart::float_t length = 0.0;

	for(std::size_t i = 1; i < points.size(); i++) {
		length += std::max(pixelpart::math::distance(points[i], points[i - 1]), 0.000001);
		distances[i] = length;
	}

	pixelpart::Curve<pixelpart::float3_t> modifiedPath;
	for(std::size_t i = 0; i < points.size(); i++) {
		modifiedPath.addPoint(distances[i] / length, points[i]);
	}

	particleEmitter->path() = modifiedPath;
}
std::vector<pixelpart::float3_t> PixelpartParticleEmitter::get_shape_points() const {
	std::vector<pixelpart::float3_t> points(particleEmitter->path().pointCount(), pixelpart::float3_t(0.0));
	for(std::size_t i = 0; i < particleEmitter->path().pointCount(); i++) {
		points[i] = particleEmitter->path().point(i).value;
	}

	return points;
}

void PixelpartParticleEmitter::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_shape", "type"), &PixelpartParticleEmitter::set_shape);
	ClassDB::bind_method(D_METHOD("get_shape"), &PixelpartParticleEmitter::get_shape);
	ClassDB::bind_method(D_METHOD("add_shape_point", "point"), &PixelpartParticleEmitter::add_shape_point);
	ClassDB::bind_method(D_METHOD("remove_shape_point", "index"), &PixelpartParticleEmitter::remove_shape_point);
	ClassDB::bind_method(D_METHOD("set_shape_point", "index", "point"), &PixelpartParticleEmitter::set_shape_point);
	ClassDB::bind_method(D_METHOD("get_shape_point", "index"), &PixelpartParticleEmitter::get_shape_point);
	ClassDB::bind_method(D_METHOD("get_shape_point_count"), &PixelpartParticleEmitter::get_shape_point_count);
	ClassDB::bind_method(D_METHOD("set_distribution", "mode"), &PixelpartParticleEmitter::set_distribution);
	ClassDB::bind_method(D_METHOD("get_distribution"), &PixelpartParticleEmitter::get_distribution);
	ClassDB::bind_method(D_METHOD("set_grid_order", "mode"), &PixelpartParticleEmitter::set_grid_order);
	ClassDB::bind_method(D_METHOD("get_grid_order"), &PixelpartParticleEmitter::get_grid_order);
	ClassDB::bind_method(D_METHOD("set_grid_size", "x", "y", "z"), &PixelpartParticleEmitter::set_grid_size);
	ClassDB::bind_method(D_METHOD("get_grid_size_x"), &PixelpartParticleEmitter::get_grid_size_x);
	ClassDB::bind_method(D_METHOD("get_grid_size_y"), &PixelpartParticleEmitter::get_grid_size_y);
	ClassDB::bind_method(D_METHOD("get_grid_size_z"), &PixelpartParticleEmitter::get_grid_size_z);
	ClassDB::bind_method(D_METHOD("set_emission_mode", "mode"), &PixelpartParticleEmitter::set_emission_mode);
	ClassDB::bind_method(D_METHOD("get_emission_mode"), &PixelpartParticleEmitter::get_emission_mode);
	ClassDB::bind_method(D_METHOD("set_direction_mode", "mode"), &PixelpartParticleEmitter::set_direction_mode);
	ClassDB::bind_method(D_METHOD("get_direction_mode"), &PixelpartParticleEmitter::get_direction_mode);
	ClassDB::bind_method(D_METHOD("get_direction"), &PixelpartParticleEmitter::get_direction);
	ClassDB::bind_method(D_METHOD("get_spread"), &PixelpartParticleEmitter::get_spread);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "shape"), "set_shape", "get_shape");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "distribution"), "set_distribution", "get_distribution");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "grid_order"), "set_grid_order", "get_grid_order");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "emission_mode"), "set_emission_mode", "get_emission_mode");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "direction_mode"), "set_direction_mode", "get_direction_mode");

	BIND_ENUM_CONSTANT(SHAPE_POINT);
	BIND_ENUM_CONSTANT(SHAPE_LINE);
	BIND_ENUM_CONSTANT(SHAPE_ELLIPSE);
	BIND_ENUM_CONSTANT(SHAPE_RECTANGLE);
	BIND_ENUM_CONSTANT(SHAPE_PATH);
	BIND_ENUM_CONSTANT(SHAPE_ELLIPSOID);
	BIND_ENUM_CONSTANT(SHAPE_CUBOID);
	BIND_ENUM_CONSTANT(SHAPE_CYLINDER);

	BIND_ENUM_CONSTANT(DISTRIBUTION_UNIFORM);
	BIND_ENUM_CONSTANT(DISTRIBUTION_CENTER);
	BIND_ENUM_CONSTANT(DISTRIBUTION_HOLE);
	BIND_ENUM_CONSTANT(DISTRIBUTION_BOUNDARY);
	BIND_ENUM_CONSTANT(DISTRIBUTION_GRID_RANDOM);
	BIND_ENUM_CONSTANT(DISTRIBUTION_GRID_ORDERED);

	BIND_ENUM_CONSTANT(GRID_ORDER_X_Y_Z);
	BIND_ENUM_CONSTANT(GRID_ORDER_X_Z_Y);
	BIND_ENUM_CONSTANT(GRID_ORDER_Y_X_Z);
	BIND_ENUM_CONSTANT(GRID_ORDER_Y_Z_X);
	BIND_ENUM_CONSTANT(GRID_ORDER_Z_X_Y);
	BIND_ENUM_CONSTANT(GRID_ORDER_Z_Y_X);

	BIND_ENUM_CONSTANT(EMISSION_CONTINUOUS);
	BIND_ENUM_CONSTANT(EMISSION_BURST_START);
	BIND_ENUM_CONSTANT(EMISSION_BURST_END);

	BIND_ENUM_CONSTANT(DIRECTION_FIXED);
	BIND_ENUM_CONSTANT(DIRECTION_OUTWARDS);
	BIND_ENUM_CONSTANT(DIRECTION_INWARDS);
	BIND_ENUM_CONSTANT(DIRECTION_INHERIT);
	BIND_ENUM_CONSTANT(DIRECTION_INHERIT_INVERSE);
}
}
