#include "StdAfx.h"
#include "ChestNoteBlock.h"

glm::vec4 ChestNoteBlock::chestColour(.5f,.25f,0,1);

void ChestNoteBlock::RenderCoin(glm::mat4& pv, double songPos){
	songPos /= 8;
	shader->UseShader();
	shader->UploadUniforms(&glm::scale(glm::rotate(glm::translate(pv,position+size+glm::vec3(0,1,0)),90.0f,glm::vec3(0,1,0)),glm::vec3(chestScale,chestScale,chestScale)),&chestColour);
	chestMeshBase->LoadMesh();
	chestMeshBase->Draw();
	shader->UploadUniforms(&glm::scale(glm::rotate(glm::rotate(glm::translate(pv,position+size+glm::vec3(0,1,0)+chestScale*glm::vec3(0,1.25f,-1)),90.0f,glm::vec3(0,1,0)),(float)(45*cos(songPos*PI/90+coinRotationSeed)-45),glm::vec3(0,0,1)),glm::vec3(chestScale,chestScale,chestScale)),&chestColour);
	chestMeshLid->LoadMesh();
	chestMeshLid->Draw();
	shader->UploadUniforms(&glm::scale(glm::rotate(glm::translate(pv,position+size+glm::vec3(0,1,0)),90.0f,glm::vec3(0,1,0)),glm::vec3(chestScale,chestScale,chestScale)),&coinColour);
	chestMeshGold->LoadMesh();
	chestMeshGold->Draw();
}

ChestNoteBlock::ChestNoteBlock(State* state, MIDI::Song* song, MIDI::Note* n, UINT64 notePos, double songSpeed):NoteBlock(state,song,n,notePos,songSpeed,0){
	chestBlocksList.push_back(this);
	coinRotationSeed = rand()%360;
	chestMeshBase = LoadMesh("models/chest_base.scm");
	chestMeshLid = LoadMesh("models/chest_lid.scm");
	chestMeshGold = LoadMesh("models/chest_gold.scm");
	hasCoin = true;
	coinScore = 10;
}

ChestNoteBlock::~ChestNoteBlock(){
	for (auto i = chestBlocksList.begin(); i != chestBlocksList.end(); i++){
		if ((*i) == this){
			chestBlocksList.erase(i);
			break;
		}
	}
	UnregisterMesh(chestMeshBase);
	UnregisterMesh(chestMeshLid);
	UnregisterMesh(chestMeshGold);
}

vector<ChestNoteBlock*> ChestNoteBlock::chestBlocksList;
const float ChestNoteBlock::chestScale = 1.5f;