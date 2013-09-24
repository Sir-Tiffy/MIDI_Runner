#pragma once
#include "StdAfx.h"
#include "NoteBlock.h"

class ChestNoteBlock:public NoteBlock{
protected:
	Mesh* chestMeshBase;
	Mesh* chestMeshLid;
	Mesh* chestMeshGold;
	static glm::vec4 chestColour;
	const static float chestScale;
public:
	static vector<ChestNoteBlock*> chestBlocksList;
	virtual void RenderCoin(glm::mat4& pv, double songPos) override;
	ChestNoteBlock(State* state, MIDI::Song* song, MIDI::Note* n, UINT64 notePos, double songSpeed);
	virtual ~ChestNoteBlock();
};