enum InterpolationType {
	NONE = 0,
	LINEAR = 1,
	SPLINE = 2
}

enum BlendMode {
	NORMAL = 0,
	ADDITIVE = 1,
	SUBTRACTIVE = 2
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
	BOUNDARY = 3
}

enum EmitterEmissionMode {
	CONTINUOUS = 0,
	BURST_START = 1,
	BURST_END = 2
}

enum EmitterDirectionMode {
	FIXED = 0,
	OUTWARDS = 1,
	INWARDS = 2
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
	POINT = 0,
	AREA = 1
}
