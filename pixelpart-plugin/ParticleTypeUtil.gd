extends Object

class_name ParticleTypeUtil

static func print_info(particle_type: PixelpartParticleType):
	print("Particle type \"", particle_type.get_name(), "\":",
		" id=", particle_type.get_id(),
		" parent_id=", particle_type.get_parent_id())
