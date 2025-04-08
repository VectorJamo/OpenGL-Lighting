#include "Cube.h"
#pragma once

Cube::Cube()
{
	float verts[] = {
		-0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // Front 
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // Back
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, // Left
		-0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,

		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Right
		 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,

		 -0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // UP
		  0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
		  0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 -0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
		  0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,

		 -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, // DOWN
		  0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
		  0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
		 -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
		  0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
		 -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
	};

	memcpy(vertices, verts, sizeof(float) * totalVertices * vertexSize);
}
