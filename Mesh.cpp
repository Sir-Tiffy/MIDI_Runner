#include "StdAfx.h"
#include "Mesh.h"

void Mesh::Draw(){
	glDrawElements(GL_TRIANGLES, numFaces*3, GL_UNSIGNED_INT, 0);
	CheckError("Error drawing mesh");
}
void Mesh::LoadMesh(){
	glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT, false,0,0);
	if (hasNormals){
		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1,3,GL_FLOAT,true,0,0);
	}
	if (hasUVs){
		glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
		//glBufferData(GL_ARRAY_BUFFER, numVerts*2*sizeof(float), UVs.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(hasNormals?2:1);
		glVertexAttribPointer(hasNormals?2:1, 3, GL_FLOAT, true, 0,0);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceBuffer);
	CheckError("Error loading mesh");
}

Mesh::Mesh(string filename){
	name = filename;
	ifstream file;
	vector<float> verts, norms, UVs;
	vector<unsigned int> faces;
	try{
		file.open(filename,ios::in|ios::binary);
		if (!file.good()) throw runtime_error("File not found!");
		file.exceptions(ifstream::badbit|ifstream::failbit|ifstream::eofbit);
		const char header[] = "SCM\1";
		for (char i = 0; i < 4; i++){
			if (file.get() != header[i]) throw runtime_error("Not a valid Super Cool Model File!");
		}
		char flags = file.get();
		hasNormals = (flags&MESH_HAS_NORMALS)!=0;
		hasUVs = (flags&MESH_HAS_UVS)!=0;
		file.read((char*)&numVerts,sizeof(numVerts));
		verts.resize(numVerts*3);
		file.read((char*)verts.data(),numVerts*3*sizeof(float));
		file.read((char*)&numFaces,sizeof(numFaces));
		faces.resize(numFaces*3);
		file.read((char*)faces.data(),numFaces*3*sizeof(unsigned int));

		if (hasNormals){
			norms.resize(numVerts*3);
			file.read((char*)norms.data(),numVerts*3*sizeof(float));
		}
		if (hasUVs){
			UVs.resize(numVerts*2);
			file.read((char*)UVs.data(), numVerts*2*sizeof(float));
		}
		file.close();
		glGenBuffers(1,&vertBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
		glBufferData(GL_ARRAY_BUFFER, numVerts*3*sizeof(float), verts.data(), GL_STATIC_DRAW);
		
		glGenBuffers(1, &faceBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numFaces*3*sizeof(unsigned int), faces.data(), GL_STATIC_DRAW);

		if (hasNormals){
			glGenBuffers(1,&normalBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
			glBufferData(GL_ARRAY_BUFFER, numVerts*3*sizeof(float), norms.data(), GL_STATIC_DRAW);
		} else normalBuffer = 0;
		if (hasUVs){
			glGenBuffers(1, &UVBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
			glBufferData(GL_ARRAY_BUFFER, numVerts*2*sizeof(float), UVs.data(), GL_STATIC_DRAW);
		} else UVBuffer = 0;
	} catch (exception &e){
		file.close();
		stringstream s;
		s<<"Failed to load model \""<<filename<<"\":\n"<<e.what();
		throw runtime_error(s.str());
	}
	
	CheckError("Error loading mesh");
}

Mesh::~Mesh(){
	glDeleteBuffers(1,&vertBuffer);
	glDeleteBuffers(1,&faceBuffer);
	if (hasNormals) glDeleteBuffers(1,&normalBuffer);
	if (hasUVs) glDeleteBuffers(1,&UVBuffer);
}
