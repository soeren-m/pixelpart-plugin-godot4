vec2 pixelpart_sprite_sheet_animation(vec2 textureCoord, float life, int origin, int startFrame, int numFrames, int rowNumber, int columnNumber, float duration, bool loop) {
	int animationFrame = startFrame;
	animationFrame += loop
		? int(mod(life / duration * float(numFrames), float(numFrames)))
		: clamp(int(life / duration * float(numFrames)), 0, numFrames - 1);

	switch(origin) {
		case 0:
			return vec2(
					float((animationFrame % (rowNumber * columnNumber)) % rowNumber) / float(rowNumber),
					float((animationFrame % (rowNumber * columnNumber)) / rowNumber) / float(columnNumber)) +
				vec2(1.0 / float(rowNumber), 1.0 / float(columnNumber)) * textureCoord;
		case 1:
			return vec2(
					1.0 - float((animationFrame % (rowNumber * columnNumber)) % rowNumber) / float(rowNumber) - 1.0 / float(rowNumber),
					float((animationFrame % (rowNumber * columnNumber)) / rowNumber) / float(columnNumber)) +
				vec2(1.0 / float(rowNumber), 1.0 / float(columnNumber)) * textureCoord;
		case 2:
			return vec2(
					float((animationFrame % (rowNumber * columnNumber)) % rowNumber) / float(rowNumber),
					1.0 - float((animationFrame % (rowNumber * columnNumber)) / rowNumber) / float(columnNumber) - 1.0 / float(columnNumber)) +
				vec2(1.0 / float(rowNumber), 1.0 / float(columnNumber)) * textureCoord;
		case 3:
			return vec2(
					1.0 - float((animationFrame % (rowNumber * columnNumber)) % rowNumber) / float(rowNumber) - 1.0 / float(rowNumber),
					1.0 - float((animationFrame % (rowNumber * columnNumber)) / rowNumber) / float(columnNumber) - 1.0 / float(columnNumber)) +
				vec2(1.0 / float(rowNumber), 1.0 / float(columnNumber)) * textureCoord;
		default:
			return textureCoord;
	}
}
