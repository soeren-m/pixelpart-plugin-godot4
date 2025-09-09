#include "PixelpartParticleEmitter.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>
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

void PixelpartParticleEmitter::set_shape(int type) {
	if(!particleEmitter) {
		return;
	}

	particleEmitter->shape(static_cast<pixelpart::ParticleEmitter::Shape>(type));
}
int PixelpartParticleEmitter::get_shape() const {
	if(!particleEmitter) {
		return static_cast<int>(pixelpart::ParticleEmitter::Shape::point);
	}

	return static_cast<int>(particleEmitter->shape());
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

void PixelpartParticleEmitter::set_distribution(int mode) {
	if(!particleEmitter) {
		return;
	}

	particleEmitter->distribution(static_cast<pixelpart::ParticleEmitter::Distribution>(mode));
}
int PixelpartParticleEmitter::get_distribution() const {
	if(!particleEmitter) {
		return static_cast<int>(pixelpart::ParticleEmitter::Distribution::uniform);
	}

	return static_cast<int>(particleEmitter->distribution());
}
void PixelpartParticleEmitter::set_grid_order(int mode) {
	if(!particleEmitter) {
		return;
	}

	particleEmitter->gridOrder(static_cast<pixelpart::ParticleEmitter::GridOrder>(mode));
}
int PixelpartParticleEmitter::get_grid_order() const {
	if(!particleEmitter) {
		return static_cast<int>(pixelpart::ParticleEmitter::GridOrder::x_y_z);
	}

	return static_cast<int>(particleEmitter->gridOrder());
}
void PixelpartParticleEmitter::set_grid_size(int x, int y, int z) {
	if(!particleEmitter) {
		return;
	}

	particleEmitter->gridSize(
		static_cast<uint32_t>(x),
		static_cast<uint32_t>(y),
		static_cast<uint32_t>(z));
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
void PixelpartParticleEmitter::set_emission_mode(int mode) {
	if(!particleEmitter) {
		return;
	}

	particleEmitter->emissionMode(static_cast<pixelpart::ParticleEmitter::EmissionMode>(mode));
}
int PixelpartParticleEmitter::get_emission_mode() const {
	if(!particleEmitter) {
		return static_cast<int>(pixelpart::ParticleEmitter::EmissionMode::continuous);
	}

	return static_cast<int>(particleEmitter->emissionMode());
}
void PixelpartParticleEmitter::set_direction_mode(int mode) {
	if(!particleEmitter) {
		return;
	}

	particleEmitter->directionMode(static_cast<pixelpart::ParticleEmitter::DirectionMode>(mode));
}
int PixelpartParticleEmitter::get_direction_mode() const {
	if(!particleEmitter) {
		return static_cast<int>(pixelpart::ParticleEmitter::DirectionMode::fixed);
	}

	return static_cast<int>(particleEmitter->directionMode());
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
		length += std::max(glm::distance(points[i], points[i - 1]), 0.000001);
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

	// Deprecated
	ClassDB::bind_method(D_METHOD("get_num_shape_points"), &PixelpartParticleEmitter::get_shape_point);
	ClassDB::bind_method(D_METHOD("get_grid_width"), &PixelpartParticleEmitter::get_grid_size_x);
	ClassDB::bind_method(D_METHOD("get_grid_height"), &PixelpartParticleEmitter::get_grid_size_y);
	ClassDB::bind_method(D_METHOD("get_grid_depth"), &PixelpartParticleEmitter::get_grid_size_z);
}
}
