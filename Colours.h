#pragma once

const glm::vec4 colours[] = {
	glm::vec4(1,0,0,1),
	glm::vec4(1,.5,0,1),
	glm::vec4(1,1,0,1),
	glm::vec4(.5,1,0,1),
	glm::vec4(0,1,0,1),
	glm::vec4(0,1,.5,1),
	glm::vec4(0,1,1,1),
	glm::vec4(0,.5,1,1),
	glm::vec4(0,0,1,1),
	glm::vec4(0,0,0,1),
	glm::vec4(.5,0,1,1),
	glm::vec4(1,0,1,1),
	glm::vec4(1,0,.5,1),
	glm::vec4(.25,.25,.25,1),
	glm::vec4(.5,.5,.5,1),
	glm::vec4(.75,.75,.75,1),
	glm::vec4(1,1,1,1)
};

const int NUMCOLOURS = sizeof(colours)/sizeof(glm::vec3);

enum colours{
	RED,
	ORANGE,
	YELLOW,
	LIME,
	GREEN,
	TURQUOISE,
	CYAN,
	COBALT,
	BLUE,
	BLACK,
	PURPLE,
	MAGENTA,
	CRIMSON,
	DARKGREY,
	GREY,
	LIGHTGREY,
	WHITE
};

glm::vec3 GetColourFromHue(double hue);