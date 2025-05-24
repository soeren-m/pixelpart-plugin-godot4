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

	var typestr = "Node"
	if group_node: typestr = "GroupNode"
	elif emitter: typestr = "ParticleEmitter"
	elif attraction_field: typestr = "AttractionField"
	elif acceleration_field: typestr = "AccelerationField"
	elif vector_field: typestr = "VectorField"
	elif noise_field: typestr = "NoiseField"
	elif drag_field: typestr = "DragField"
	elif line_collider: typestr = "LineCollider"
	elif plane_collider: typestr = "PlaneCollider"
	elif dir_light: typestr = "DirectionalLightSource"
	elif point_light: typestr = "PointLightSource"
	elif spot_light: typestr = "SpotLightSource"

	print("Node \"", node.get_name(), "\":",
		" id=", node.get_id(),
		" parent_id=", node.get_parent_id(),
		" type=", typestr)
