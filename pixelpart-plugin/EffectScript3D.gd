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
