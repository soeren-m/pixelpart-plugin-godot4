extends Node3D

func _ready():
	# Print information about particle types
	var particle_type = $SampleEffect.get_particle_type_at_index(0)
	var particle_type_index = 0
	while particle_type:
		ParticleTypeUtil.print_info(particle_type)

		particle_type_index += 1
		particle_type = $SampleEffect.get_particle_type_at_index(particle_type_index)

	# Print information about effect nodes
	var node = $SampleEffect.get_node_at_index(0)
	var node_index = 0
	while node:
		NodeUtil.print_info(node)

		node_index += 1
		node = $SampleEffect.get_node_at_index(node_index)
