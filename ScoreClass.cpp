#include "StdAfx.h"
#include "ScoreClass.h"

vector<ScoreStruct> ScoreClass::GetScores(string songName){
	for (auto i = songScores.begin(); i != songScores.end(); i++){
		if ((*i).name == songName) return (*i).scores;
	}
	return vector<ScoreStruct>(0);
}

vector<string> ScoreClass::GetScoreNames(){
	vector<string> result;
	for (auto i = songScores.begin(); i != songScores.end(); i++){
		result.push_back((*i).name);
	}
	return result;
}

unsigned short ScoreClass::GetNumSongs(){
	return songScores.size();
}

void ScoreClass::Save(){
	ofstream file;
	file.open(SCOREFILENAME,ios::out);
	file<<songScores.size()<<'\n';
	for (auto i = songScores.begin(); i != songScores.end(); i++){
		file<<(*i).name<<'\n'<<(*i).scores.size()<<'\n';
		for (auto j = (*i).scores.begin(); j != (*i).scores.end(); j++){
			file<<(*j).name<<'\n'<<(*j).score<<'\n';
		}
	}
	file.close();
}

void ScoreClass::InsertScore(string songName, ScoreStruct score){
	bool unfound = true;
	for (auto i = songScores.begin(); i != songScores.end(); i++){
		if ((*i).name == songName){
			vector<ScoreStruct>& scores = (*i).scores;
			scores.push_back(score);
			for (short j = scores.size()-1; j > 0; j--){
				if (scores[j].score > scores[j-1].score) swap(scores[j],scores[j-1]);
			}
			if (scores.size() > maxNumScores) scores.resize(maxNumScores);
			
			unfound = false;
			break;
		}
	}
	if (unfound){
		SongScoresStruct s;
		s.name = songName;
		s.scores.push_back(score);
		songScores.push_back(s);
	}
	Save();
}

void ScoreClass::LoadScores(){
	ifstream file;
	try{
		file.open(SCOREFILENAME,ios::in);
		file.exceptions(ifstream::failbit|ifstream::badbit);
		unsigned short numSongs;
		file>>numSongs;
		file.ignore();
		for (unsigned short i = 0; i < numSongs; i++){
			SongScoresStruct song;
			char buffer[MAX_PATH] = {0};
			file.getline(buffer,MAX_PATH);
			song.name.assign(buffer);
			unsigned short numScores;
			file>>numScores;
			file.ignore();
			for (unsigned short j = 0; j < numScores; j++){
				ScoreStruct s;
				file.getline(buffer,maxNameLength);
				s.name.assign(buffer);
				file>>s.score;
				if (!file.eof()) file.ignore();
				song.scores.push_back(s);
			}
			songScores.push_back(song);
		}
	} catch (exception){
		songScores.clear();
	}
	file.exceptions(0);
	file.close();
}

vector<SongScoresStruct> ScoreClass::songScores(0);