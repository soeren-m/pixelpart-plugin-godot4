extends Node2D

const Pixelpart = preload("res://addons/pixelpart/src/PixelpartCommon.gd")

func _ready():
	#$SampleEffect.set_input_float("TestInput", 0.5)
	
	# Get particle type by name
	var particleType = $SampleEffect.find_particle_type("Particle")
	if particleType == null:
		return

	# Get particle emitter by name
	var particleEmitter = $SampleEffect.find_particle_emitter("Emitter")
	if particleEmitter == null:
		return
		
	# Spawn some extra particles
	#$SampleEffect.spawn_particles("Particle", 200)

	# Define animated property
	#particleType.get_opacity().clear()
	#particleType.get_opacity().add_point(0.0, 1.0)
	#particleType.get_opacity().add_point(1.0, 0.0)

	# Set animated property to constant value
	#particleEmitter.get_spread().clear()
	#particleEmitter.get_spread().add_point(0.0, 360.0)
