#pragma once
#include "StdAfx.h"
#include "Mesh.h"

Mesh* LoadMesh(string meshName);
void UnregisterMesh(string meshName);
void UnregisterMesh(Mesh* mesh);
void DeleteAllMeshes();
