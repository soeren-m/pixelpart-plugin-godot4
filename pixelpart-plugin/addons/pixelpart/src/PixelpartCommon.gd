enum InterpolationType {
	OFF = 0,
	LINEAR = 1,
	SPLINE = 2
}

enum BlendMode {
	OFF = 0,
	NORMAL = 1,
	ADDITIVE = 2,
	SUBTRACTIVE = 3
}

enum EmitterShapeType {
	POINT = 0,
	LINE = 1,
	ELLIPSE = 2,
	RECTANGLE = 3,
	PATH = 4,
	ELLIPSOID = 5,
	CUBOID = 6,
	CYLINDER = 7
}

enum EmitterDistributionType {
	UNIFORM = 0,
	CENTER = 1,
	HOLE = 2,
	BOUNDARY = 3,
	GRID_RANDOM = 4,
	GRID_ORDERED = 5
}

enum EmitterGridOrderType {
	X_Y_Z = 0,
	X_Z_Y = 1,
	Y_X_Z = 2,
	Y_Z_X = 3,
	Z_X_Y = 4,
	Z_Y_Z = 5
};

enum EmitterEmissionMode {
	CONTINUOUS = 0,
	BURST_START = 1,
	BURST_END = 2
}

enum EmitterDirectionMode {
	FIXED = 0,
	OUTWARDS = 1,
	INWARDS = 2,
	INHERIT = 3,
	INHERIT_INVERSE = 4
}

enum RotationMode {
	ANGLE = 0,
	VELOCITY = 1
}

enum AlignmentMode {
	NONE = 0,
	CAMERA = 1,
	MOTION = 2,
	EMISSION = 3,
	EMITTER = 4
}

enum ForceType {
	ATTRACTION_FIELD = 0,
	ACCELERATION_FIELD = 1,
	VECTOR_FIELD = 2,
	NOISE_FIELD = 3,
	DRAG_FIELD = 4
}
