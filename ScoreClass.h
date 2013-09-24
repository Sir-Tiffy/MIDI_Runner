#pragma once
#include "StdAfx.h"

const unsigned short maxNameLength = 20;
const unsigned short maxNumScores = 10;

#define SCOREFILENAME "scores.data"

struct ScoreStruct{
	unsigned short score;
	string name;
};
struct SongScoresStruct{
	string name;
	vector<ScoreStruct> scores;
};

class ScoreClass{
protected:
	static vector<SongScoresStruct> songScores;
	static void Save();
public:
	static vector<ScoreStruct> GetScores(string songName);
	static vector<string> GetScoreNames();
	static unsigned short GetNumSongs();
	static void InsertScore(string songName, ScoreStruct score);
	static void LoadScores();
	//ScoreClass();
};