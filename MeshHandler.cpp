#include "StdAfx.h"
#include "MeshHandler.h"

unordered_map<string, Mesh*> meshes;
unordered_map<string, unsigned int> meshRegistry;

Mesh* LoadMesh(string meshName){
	auto meshIter = meshes.find(meshName);
	Mesh* m;
	if (meshIter == meshes.end()){
		m = new Mesh(meshName);
		meshes[meshName] = m;
		meshRegistry[meshName] = 1;
	} else {
		m = meshIter->second;
		meshRegistry[meshName]++;
	}
	return m;
}

void UnregisterMesh(string meshName){  
	return;
	auto meshIter = meshRegistry.find(meshName);
	if (meshIter == meshRegistry.end()){
		stringstream s;
		s<<"Tried to unregister non-existent mesh \""<<meshName<<"\"!";
		throw runtime_error(s.str());
	}
	if (--meshRegistry[meshName] == 0){
		delete meshes[meshName];
		meshRegistry.erase(meshName);
	}
}

void UnregisterMesh(Mesh* mesh){
	if (mesh != 0) UnregisterMesh(mesh->name);
}

void DeleteAllMeshes(){
	for (auto i = meshes.begin(); i != meshes.end(); i++){
		delete i->second;
	}
	meshes.clear();
	meshRegistry.clear();
}
