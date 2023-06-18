extends Node3D

const Pixelpart = preload("res://addons/pixelpart/src/PixelpartCommon.gd")

func _ready():
	# Get particle type by name
	var particleType = $SampleEffect.find_particle_type("Particle")
	if particleType == null:
		return

	# Get particle emitter by name
	var particleEmitter = $SampleEffect.find_particle_emitter("Emitter")
	if particleEmitter == null:
		return

	# Set particle property
	particleType.velocity_variance = 0.2

	# Define animated property
	particleType.get_opacity().clear()
	particleType.get_opacity().add_point(0.0, 1.0)
	particleType.get_opacity().add_point(1.0, 0.0)

	# Set animated property to constant value
	particleEmitter.get_spread().set(360.0)

	# Spawn some extra particles
	particleType.spawn_particles(200)
