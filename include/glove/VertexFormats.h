#pragma once

struct Vertex2D {
	float x, y;
};

struct Vertex3D {
	float x, y, z;
};

struct Vertex2DRgb {
	float x, y;
	float r, g, b;
};

struct Vertex3DRgb {
	float x, y, z;
	float r, g, b;
};

struct Vertex2DTex {
	float x, y;
	float u, v;
};

struct Vertex3DTex {
	float x, y, z;
	float u, v;
};