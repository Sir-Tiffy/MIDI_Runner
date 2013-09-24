#pragma once
#include "StdAfx.h"

enum meshFlags{
	MESH_HAS_NORMALS = 0x80,
	MESH_HAS_UVS = 0x40
};

class Mesh{
protected:
	bool hasNormals;
	bool hasUVs;

	unsigned int numVerts;
	unsigned int numFaces;
	
	GLuint 
		vertBuffer,
		faceBuffer,
		normalBuffer,
		UVBuffer;
public:
	string name;
	void Draw();
	void LoadMesh();

	Mesh(string filename);
	virtual ~Mesh();
};
