extends Object

class_name NodeUtil

static func print_info(node: PixelpartNode):
	if not node:
		return

	var group_node := node as PixelpartGroupNode
	var emitter := node as PixelpartParticleEmitter
	var attraction_field := node as PixelpartAttractionField
	var acceleration_field := node as PixelpartAccelerationField
	var vector_field := node as PixelpartVectorField
	var noise_field := node as PixelpartNoiseField
	var drag_field := node as PixelpartDragField
	var line_collider := node as PixelpartLineCollider
	var plane_collider := node as PixelpartPlaneCollider
	var dir_light := node as PixelpartDirectionalLightSource
	var point_light := node as PixelpartPointLightSource
	var spot_light := node as PixelpartSpotLightSource

	var type_string = "Node"
	if group_node: type_string = "GroupNode"
	elif emitter: type_string = "ParticleEmitter"
	elif attraction_field: type_string = "AttractionField"
	elif acceleration_field: type_string = "AccelerationField"
	elif vector_field: type_string = "VectorField"
	elif noise_field: type_string = "NoiseField"
	elif drag_field: type_string = "DragField"
	elif line_collider: type_string = "LineCollider"
	elif plane_collider: type_string = "PlaneCollider"
	elif dir_light: type_string = "DirectionalLightSource"
	elif point_light: type_string = "PointLightSource"
	elif spot_light: type_string = "SpotLightSource"

	print("Node \"", node.get_name(), "\":",
		" id=", node.get_id(),
		" parent_id=", node.get_parent_id(),
		" type=", type_string)
