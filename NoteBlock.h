#pragma once
#include "StdAfx.h"
#include "RenderableObject.h"
#include "MIDI_Objects.h"
#include "DiffuseShader.h"
#include "MeshHandler.h"

class NoteBlock:public RenderableObject{
protected:
	DiffuseShader* shader;
	Mesh* blockMesh;
	Mesh* coinMesh;
	glm::mat4 mat;
	static glm::vec4 coinColour;
public:
	unsigned char coinScore;
	double coinRotationSeed;
	bool hasCoin;
	glm::vec4 colour;
	UINT64 notePos;
	static bool loaded;
	glm::vec3 position;
	glm::vec3 size;
	double length;
	MIDI::Note* note;

	inline bool IsPlaying(double songPos, double songSpeed);
	void UpdateLength(float length);
	virtual void RebuildMatrix();
	virtual void Render(glm::mat4& pv, double songPos, double songSpeed);
	virtual void RenderCoin(glm::mat4& pv, double songPos);
	virtual void Update(double currentTime, double deltaTime, double songPos, double songSpeed);
	NoteBlock(State* state, MIDI::Song* song, MIDI::Note* n, UINT64 notePos, double songSpeed, unsigned int coinChance);
	virtual ~NoteBlock();
};