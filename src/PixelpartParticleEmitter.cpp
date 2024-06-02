#include "PixelpartParticleEmitter.h"
#include "util/PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
void PixelpartParticleEmitter::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_id"), &PixelpartParticleEmitter::get_id);
	ClassDB::bind_method(D_METHOD("get_parent_id"), &PixelpartParticleEmitter::get_parent_id);
	ClassDB::bind_method(D_METHOD("get_name"), &PixelpartParticleEmitter::get_name);
	ClassDB::bind_method(D_METHOD("set_lifetime_start", "time"), &PixelpartParticleEmitter::set_lifetime_start);
	ClassDB::bind_method(D_METHOD("set_lifetime_duration", "time"), &PixelpartParticleEmitter::set_lifetime_duration);
	ClassDB::bind_method(D_METHOD("set_repeat", "value"), &PixelpartParticleEmitter::set_repeat);
	ClassDB::bind_method(D_METHOD("get_lifetime_start"), &PixelpartParticleEmitter::get_lifetime_start);
	ClassDB::bind_method(D_METHOD("get_lifetime_duration"), &PixelpartParticleEmitter::get_lifetime_duration);
	ClassDB::bind_method(D_METHOD("get_repeat"), &PixelpartParticleEmitter::get_repeat);
	ClassDB::bind_method(D_METHOD("is_active"), &PixelpartParticleEmitter::is_active);
	ClassDB::bind_method(D_METHOD("get_local_time"), &PixelpartParticleEmitter::get_local_time);
	ClassDB::bind_method(D_METHOD("get_position"), &PixelpartParticleEmitter::get_position);
	ClassDB::bind_method(D_METHOD("set_shape", "type"), &PixelpartParticleEmitter::set_shape);
	ClassDB::bind_method(D_METHOD("get_shape"), &PixelpartParticleEmitter::get_shape);
	ClassDB::bind_method(D_METHOD("add_shape_point", "point"), &PixelpartParticleEmitter::add_shape_point);
	ClassDB::bind_method(D_METHOD("remove_shape_point", "index"), &PixelpartParticleEmitter::remove_shape_point);
	ClassDB::bind_method(D_METHOD("set_shape_point", "index", "point"), &PixelpartParticleEmitter::set_shape_point);
	ClassDB::bind_method(D_METHOD("get_num_shape_points"), &PixelpartParticleEmitter::get_num_shape_points);
	ClassDB::bind_method(D_METHOD("get_shape_point", "index"), &PixelpartParticleEmitter::get_shape_point);
	ClassDB::bind_method(D_METHOD("get_size"), &PixelpartParticleEmitter::get_size);
	ClassDB::bind_method(D_METHOD("get_orientation"), &PixelpartParticleEmitter::get_orientation);
	ClassDB::bind_method(D_METHOD("set_distribution", "mode"), &PixelpartParticleEmitter::set_distribution);
	ClassDB::bind_method(D_METHOD("get_distribution"), &PixelpartParticleEmitter::get_distribution);
	ClassDB::bind_method(D_METHOD("set_grid_order", "mode"), &PixelpartParticleEmitter::set_grid_order);
	ClassDB::bind_method(D_METHOD("get_grid_order"), &PixelpartParticleEmitter::get_grid_order);
	ClassDB::bind_method(D_METHOD("set_grid_size", "width", "height", "depth"), &PixelpartParticleEmitter::set_grid_size);
	ClassDB::bind_method(D_METHOD("get_grid_width"), &PixelpartParticleEmitter::get_grid_width);
	ClassDB::bind_method(D_METHOD("get_grid_height"), &PixelpartParticleEmitter::get_grid_height);
	ClassDB::bind_method(D_METHOD("get_grid_depth"), &PixelpartParticleEmitter::get_grid_depth);
	ClassDB::bind_method(D_METHOD("set_emission_mode", "mode"), &PixelpartParticleEmitter::set_emission_mode);
	ClassDB::bind_method(D_METHOD("get_emission_mode"), &PixelpartParticleEmitter::get_emission_mode);
	ClassDB::bind_method(D_METHOD("set_direction_mode", "mode"), &PixelpartParticleEmitter::set_direction_mode);
	ClassDB::bind_method(D_METHOD("get_direction_mode"), &PixelpartParticleEmitter::get_direction_mode);
	ClassDB::bind_method(D_METHOD("get_direction"), &PixelpartParticleEmitter::get_direction);
	ClassDB::bind_method(D_METHOD("get_spread"), &PixelpartParticleEmitter::get_spread);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lifetime_start"), "set_lifetime_start", "get_lifetime_start");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lifetime_duration"), "set_lifetime_duration", "get_lifetime_duration");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "repeat"), "set_repeat", "get_repeat");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "shape"), "set_shape", "get_shape");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "distribution"), "set_distribution", "get_distribution");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "grid_order"), "set_grid_order", "get_grid_order");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "emission_mode"), "set_emission_mode", "get_emission_mode");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "direction_mode"), "set_direction_mode", "get_direction_mode");
}

PixelpartParticleEmitter::PixelpartParticleEmitter() {

}

void PixelpartParticleEmitter::init(Ref<PixelpartEffectResource> resource, pixelpart::ParticleEmitter* particleEmitterPtr, pixelpart::ParticleEngine* particleEnginePtr) {
	effectResource = resource;
	particleEmitter = particleEmitterPtr;
	particleEngine = particleEnginePtr;
}

int PixelpartParticleEmitter::get_id() const {
	if(particleEmitter) {
		return static_cast<int>(particleEmitter->id);
	}

	return -1;
}
int PixelpartParticleEmitter::get_parent_id() const {
	if(particleEmitter) {
		return static_cast<int>(particleEmitter->parentId);
	}

	return -1;
}
String PixelpartParticleEmitter::get_name() const {
	if(particleEmitter) {
		return String(particleEmitter->name.c_str());
	}

	return String();
}

void PixelpartParticleEmitter::set_lifetime_start(float time) {
	if(particleEmitter) {
		particleEmitter->lifetimeStart = time;
	}
}
void PixelpartParticleEmitter::set_lifetime_duration(float time) {
	if(particleEmitter) {
		particleEmitter->lifetimeDuration = time;
	}
}
void PixelpartParticleEmitter::set_repeat(bool value) {
	if(particleEmitter) {
		particleEmitter->repeat = value;
	}
}
float PixelpartParticleEmitter::get_lifetime_start() const {
	if(particleEmitter) {
		return static_cast<float>(particleEmitter->lifetimeStart);
	}

	return 0.0f;
}
float PixelpartParticleEmitter::get_lifetime_duration() const {
	if(particleEmitter) {
		return static_cast<float>(particleEmitter->lifetimeDuration);
	}

	return 0.0f;
}
bool PixelpartParticleEmitter::get_repeat() const {
	if(particleEmitter) {
		return particleEmitter->repeat;
	}

	return false;
}
bool PixelpartParticleEmitter::is_active() const {
	if(particleEmitter && particleEngine) {
		return
			particleEngine->getTime() >= particleEmitter->lifetimeStart &&
			(particleEngine->getTime() <= particleEmitter->lifetimeStart + particleEmitter->lifetimeDuration || particleEmitter->repeat);
	}

	return false;
}
float PixelpartParticleEmitter::get_local_time() const {
	if(particleEmitter && particleEngine) {
		return static_cast<float>(std::fmod(
			particleEngine->getTime() - particleEmitter->lifetimeStart, particleEmitter->lifetimeDuration) / particleEmitter->lifetimeDuration);
	}

	return 0.0f;
}

Ref<PixelpartAnimatedPropertyFloat3> PixelpartParticleEmitter::get_position() const {
	if(particleEmitter) {
		Ref<PixelpartAnimatedPropertyFloat3> path;
		path.instantiate();
		path->init(&particleEmitter->position);

		return path;
	}

	return Ref<PixelpartAnimatedPropertyFloat3>();
}

void PixelpartParticleEmitter::set_shape(int type) {
	if(particleEmitter) {
		particleEmitter->shape = static_cast<pixelpart::ParticleEmitter::Shape>(type);
	}
}
int PixelpartParticleEmitter::get_shape() const {
	if(particleEmitter) {
		return static_cast<int>(particleEmitter->shape);
	}

	return static_cast<int>(pixelpart::ParticleEmitter::Shape::point);
}
void PixelpartParticleEmitter::add_shape_point(Vector3 point) {
	if(particleEmitter) {
		std::vector<pixelpart::vec3_t> points = get_shape_points();
		points.push_back(fromGd(point));

		set_shape_points(points);
	}
}
void PixelpartParticleEmitter::remove_shape_point(int index) {
	if(particleEmitter) {
		std::vector<pixelpart::vec3_t> points = get_shape_points();
		if(index < 0 || index >= static_cast<int>(points.size())) {
			return;
		}

		points.erase(points.begin() + index);

		set_shape_points(points);
	}
}
void PixelpartParticleEmitter::set_shape_point(int index, Vector3 point) {
	if(particleEmitter) {
		std::vector<pixelpart::vec3_t> points = get_shape_points();
		if(index < 0 || index > static_cast<int>(points.size())) {
			return;
		}

		points[index] = fromGd(point);

		set_shape_points(points);
	}
}
int PixelpartParticleEmitter::get_num_shape_points() const {
	if(particleEmitter) {
		return static_cast<int>(particleEmitter->path.getNumPoints());
	}

	return 0;
}
Vector3 PixelpartParticleEmitter::get_shape_point(int index) const {
	if(particleEmitter && index >= 0 && index < get_num_shape_points()) {
		return toGd(particleEmitter->path.getPoint(static_cast<std::size_t>(index)).value);
	}

	return Vector3(0.0f, 0.0f, 0.0f);
}

Ref<PixelpartAnimatedPropertyFloat3> PixelpartParticleEmitter::get_size() const {
	if(particleEmitter) {
		Ref<PixelpartAnimatedPropertyFloat3> property;
		property.instantiate();
		property->init(&particleEmitter->size);

		return property;
	}

	return Ref<PixelpartAnimatedPropertyFloat3>();
}
Ref<PixelpartAnimatedPropertyFloat3> PixelpartParticleEmitter::get_orientation() const {
	if(particleEmitter) {
		Ref<PixelpartAnimatedPropertyFloat3> property;
		property.instantiate();
		property->init(&particleEmitter->orientation);

		return property;
	}

	return Ref<PixelpartAnimatedPropertyFloat3>();
}

void PixelpartParticleEmitter::set_distribution(int mode) {
	if(particleEmitter) {
		particleEmitter->distribution = static_cast<pixelpart::ParticleEmitter::Distribution>(mode);
	}
}
int PixelpartParticleEmitter::get_distribution() const {
	if(particleEmitter) {
		return static_cast<int>(particleEmitter->distribution);
	}

	return static_cast<int>(pixelpart::ParticleEmitter::Distribution::uniform);
}
void PixelpartParticleEmitter::set_grid_order(int mode) {
	if(particleEmitter) {
		particleEmitter->gridOrder = static_cast<pixelpart::ParticleEmitter::GridOrder>(mode);
	}
}
int PixelpartParticleEmitter::get_grid_order() const {
	if(particleEmitter) {
		return static_cast<int>(particleEmitter->gridOrder);
	}

	return static_cast<int>(pixelpart::ParticleEmitter::GridOrder::x_y_z);
}
void PixelpartParticleEmitter::set_grid_size(int width, int height, int depth) {
	if(particleEmitter) {
		particleEmitter->gridSize[0] = static_cast<uint32_t>(width);
		particleEmitter->gridSize[1] = static_cast<uint32_t>(height);
		particleEmitter->gridSize[2] = static_cast<uint32_t>(depth);
	}
}
int PixelpartParticleEmitter::get_grid_width() const {
	if(particleEmitter) {
		return static_cast<int>(particleEmitter->gridSize[0]);
	}

	return 0;
}
int PixelpartParticleEmitter::get_grid_height() const {
	if(particleEmitter) {
		return static_cast<int>(particleEmitter->gridSize[1]);
	}

	return 0;
}
int PixelpartParticleEmitter::get_grid_depth() const {
	if(particleEmitter) {
		return static_cast<int>(particleEmitter->gridSize[2]);
	}

	return 0;
}
void PixelpartParticleEmitter::set_emission_mode(int mode) {
	if(particleEmitter) {
		particleEmitter->emissionMode = static_cast<pixelpart::ParticleEmitter::EmissionMode>(mode);
	}
}
int PixelpartParticleEmitter::get_emission_mode() const {
	if(particleEmitter) {
		return static_cast<int>(particleEmitter->emissionMode);
	}

	return static_cast<int>(pixelpart::ParticleEmitter::EmissionMode::continuous);
}
void PixelpartParticleEmitter::set_direction_mode(int mode) {
	if(particleEmitter) {
		particleEmitter->directionMode = static_cast<pixelpart::ParticleEmitter::DirectionMode>(mode);
	}
}
int PixelpartParticleEmitter::get_direction_mode() const {
	if(particleEmitter) {
		return static_cast<int>(particleEmitter->directionMode);
	}

	return static_cast<int>(pixelpart::ParticleEmitter::DirectionMode::fixed);
}
Ref<PixelpartAnimatedPropertyFloat3> PixelpartParticleEmitter::get_direction() const {
	if(particleEmitter) {
		Ref<PixelpartAnimatedPropertyFloat3> property;
		property.instantiate();
		property->init(&particleEmitter->direction);

		return property;
	}

	return Ref<PixelpartAnimatedPropertyFloat3>();
}
Ref<PixelpartAnimatedPropertyFloat> PixelpartParticleEmitter::get_spread() const {
	if(particleEmitter) {
		Ref<PixelpartAnimatedPropertyFloat> property;
		property.instantiate();
		property->init(&particleEmitter->spread);

		return property;
	}

	return Ref<PixelpartAnimatedPropertyFloat>();
}

void PixelpartParticleEmitter::set_shape_points(const std::vector<pixelpart::vec3_t>& points) {
	std::vector<pixelpart::float_t> distances(points.size(), 0.0);
	pixelpart::float_t length = 0.0;

	for(std::size_t i = 1u; i < points.size(); i++) {
		length += std::max(glm::distance(points[i], points[i - 1u]), 0.000001);
		distances[i] = length;
	}

	pixelpart::Curve<pixelpart::vec3_t> modifiedPath;
	for(std::size_t i = 0u; i < points.size(); i++) {
		modifiedPath.addPoint(distances[i] / length, points[i]);
	}

	particleEmitter->path = modifiedPath;
}
std::vector<pixelpart::vec3_t> PixelpartParticleEmitter::get_shape_points() const {
	std::vector<pixelpart::vec3_t> points(particleEmitter->path.getNumPoints(), pixelpart::vec3_t(0.0));
	for(std::size_t i = 0u; i < particleEmitter->path.getNumPoints(); i++) {
		points[i] = particleEmitter->path.getPoint(i).value;
	}

	return points;
}
}