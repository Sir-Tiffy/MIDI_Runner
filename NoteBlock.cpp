#include "StdAfx.h"
#include "NoteBlock.h"

void NoteBlock::Render(glm::mat4& pv, double songPos, double songSpeed){
	if (!loaded){
		shader->UseShader();
		blockMesh->LoadMesh();
		loaded = true;
	}
	shader->UploadUniforms(&(pv*mat),&(IsPlaying(songPos, songSpeed)?(1-colour+glm::vec4(.25,.25,.25,1)):colour));
	blockMesh->Draw();
}

void NoteBlock::RenderCoin(glm::mat4& pv, double songPos){
	if (!loaded){
		shader->UseShader();
		coinMesh->LoadMesh();
		loaded = true;
	}
	shader->UploadUniforms(&glm::rotate(glm::translate(pv,position+size+glm::vec3(0,2,0)),(float)(songPos/4+coinRotationSeed),glm::vec3(0,1,0)),&coinColour);
	coinMesh->Draw();
}

bool NoteBlock::IsPlaying(double songPos, double songSpeed){
	return songPos >= notePos && songPos <= notePos + length * songSpeed;
}

void NoteBlock::Update(double currentTime, double deltaTime, double songPos, double songSpeed){
	__super::Update(currentTime,deltaTime);
}

void NoteBlock::UpdateLength(float len){
	length = len;
	size.x = len;
	RebuildMatrix();
}

void NoteBlock::RebuildMatrix(){
	//mat = glm::translate(glm::scale(glm::mat4(1),glm::vec3(length,1,2.0/16)),position);
	mat = glm::scale(glm::translate(glm::mat4(1),position+glm::vec3(length,0,0)),size);
}

NoteBlock::NoteBlock(State* state, MIDI::Song* song, MIDI::Note* n, UINT64 notePos, double songSpeed, unsigned int coinChance):RenderableObject(state){
	if (coinChance == 0) hasCoin = false;
	else hasCoin = (rand()%coinChance)==0;
	shader = DiffuseShader::GetShader();
	blockMesh = LoadMesh("models/beveled_cube_base.scm");
	if (hasCoin){
		coinMesh = LoadMesh("models/coin.scm");
		coinRotationSeed = rand()%360;
	}
	else coinMesh = 0;
	coinScore = 1;
	//endMesh = LoadMesh("models/beveled_cube_end.scm");

	this->notePos = notePos;
	note = n;
	colour = colours[note->code&0xf];
	size = glm::vec3(0,1,1.0/8);
	position = glm::vec3(notePos*2/songSpeed,2*note->data1,-2*(note->code&0xf)/8.0);
	length = 0;
	//mat = glm::translate(glm::scale(glm::mat4(1),glm::vec3(length,1,1)), position);
}

NoteBlock::~NoteBlock(){ 
	UnregisterMesh(blockMesh);
	if (hasCoin) UnregisterMesh(coinMesh);
	//UnregisterMesh("models/beveled_cube_end.scm");
}

bool NoteBlock::loaded = false;

glm::vec4 NoteBlock::coinColour = colours[YELLOW];