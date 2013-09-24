#include "stdafx.h"
#include "MIDI_Objects.h"
#include "FileReader.h"

namespace MIDI{
	enum NOTETYPES{
		NOTE_OFF = 0x80,
		NOTE_ON = 0x90,
		KEY_PRESSURE = 0xA0,
		CONTROL_CHANGE = 0xB0,
		PROGRAM_CHANGE = 0xC0,
		CHANNEL_PRESSURE = 0xD0,
		PITCH_WHEEL = 0xE0,

		META_TEMPO = 0x51
	};

	bool Note::IsNoteOn(){
		return ((!isMeta) && ((code&0xf0)==0x90));
	}

	bool Note::IsNoteOff(){
		return ((!isMeta) && ((code&0xf0)==0x80));
	}

	void Note::DebugPrint(){
		stringstream s;
		s<<std::hex<<"Delta Time: "<<deltaTime<<"\nCode: "<<(int)code<<"\nTrack: "<<(int)track<<"\nMeta: "<<(isMeta?"Yes":"No");
		if (isMeta){
			s<<"\nMeta data length: "<<(int)metaLength<<"\nData: ";
			for (auto i = metaData.begin(); i != metaData.end(); i++){
				s<<*i;
			}
			s<<"\n\n";
		} else {
			s<<"\nData byte 1: "<<(int)data1<<"\nData byte 2: "<<(int)data2<<"\n\n";
		}
		cout<<s.str();
	}
	/*
			UINT32 deltaTime;
		unsigned char code, track, data1, data2, metaLength;
		bool isMeta;
		vector<unsigned char> metaData;
	*/
	void Note::PlayNote(HMIDIOUT device){
		union {
			char data[4];
			UINT32 word;
		} msg;
		msg.data[0] = code;
		msg.data[1] = data1;
		msg.data[2] = data2;
		msg.data[3] = 0;
		done = true;
		midiOutShortMsg(device,msg.word);
	}

	Note::Note(){
		done = false;
		isMeta = false;
	}

	void Song::EndPlaySong(){
		midiOutReset(device);
		playing = false;
	}

	void Song::BeginPlaySong(double currentTime){
		midiOutReset(device);
		tempo = 1/(120.0/60)/ticksPerBeat;
		startTime = currentTime;
		noteIterator = notes.begin();
		targetTime = 0;
		playing = true;
		done = false;
	}

	double Song::UpdatePlaySong(double currentTime, double deltaTime, bool paused){
		if (!playing) return 0;
		currentTime -= startTime;
		if (paused){
			targetTime += deltaTime;
			return 0;
		}
		while (targetTime <= currentTime){
			Note* note = *noteIterator;
			if (note->isMeta){
				switch(note->code){
					case META_TEMPO:{
						UINT32 num = 0;
						for (auto i = note->metaData.begin(); i != note->metaData.end(); i++){
							num = (num<<8)|(*i);
						}
						tempo = num/1000000.0/ticksPerBeat;
						break;
					}
				}
			} else { //not meta
				note->PlayNote(device);
			}
			noteIterator++;
			if (noteIterator == notes.end()){
				playing = false;
				done = true;
				return 0;
			} else targetTime += (*noteIterator)->deltaTime*tempo;
		}
		return deltaTime/tempo;
	}

	Song::Song(string filename):done(false), highestNote(0), lowestNote(255){

		ifstream file;
		vector<vector<Note*>> tracks;
		try{
			file.open(filename,ios::in|ios::binary);
			if (!file.good()){
				file.close();
				if (filename.compare(0,5,"midi/") != 0) filename = string("midi/").append(filename);
				if (filename.compare(filename.length()-4,4,".mid")!= 0) filename = filename.append(".mid");
				file.open(filename,ios::in|ios::binary);
				if (!file.good()) throw runtime_error("File not found!");
			}

			file.exceptions(ifstream::badbit|ifstream::failbit|ifstream::eofbit);

			char header[4];
			file.read((char*)&header,4);

			const char headerName[] = "MThd";
			const char trackName[] = "MTrk";
			for (unsigned char i = 0; i < 4; i++) if (header[i] != headerName[i]) throw runtime_error("Invalid header!");

			if (ReadNum32(file) != 6) throw runtime_error("Header has invalid size!");
			if (ReadNum16(file) > 2) throw runtime_error("MIDI file has unknown format!");
			numTracks = ReadNum16(file);
			ticksPerBeat = ReadNum16(file);
			
			tracks.resize(numTracks);
			for (unsigned char trackNum = 0; trackNum < numTracks; trackNum++){
				vector<Note*>* track = &tracks[trackNum];

				file.read((char*)&header, 4);

				for (unsigned char i = 0; i < 4; i++) if (header[i] != trackName[i]){
					stringstream s;
					s<<"Invalid track header: \""<<header<<"\" on track "<<(int)trackNum;
					throw runtime_error(s.str());
				}
				UINT32 chunkSize = ReadNum32(file);

				unsigned char prevCode = 0;
				Note* note = 0;
				UINT32 c;
				for (c = 0; c < chunkSize;){
					try {
						note = new Note();
						note->track = trackNum;
						note->deltaTime = ReadVarVal(file, c);
						if (file.peek()&0x80){
							note->code = file.get();
							++c;
							prevCode = note->code;
						} else {
							note->code = prevCode;
						}

						if (note->code == 0xff){ //meta
							note->isMeta = true;
							note->code = file.get();
							note->metaLength = ReadVarVal(file,c);
							if (note->metaLength > 0){
								note->metaData.resize(note->metaLength);
								try{
									file.read((char*)&note->metaData[0],note->metaLength);
								} catch (exception &e){
									throw runtime_error(string("Failed to read note metaData: ").append(e.what()));
								}
							}
							/*for (UINT16 i = 0; i < note->metaLength; i++){
								note->metaData.push_back(file.get());
							}*/
							c += 1+note->metaLength;
						} else if ((note->code & 0xf0) != 0xf0 || note->code == 0xf2 || note->code == 0xf3){ //not meta
							note->data1 = file.get();
							if ((note->code&0xf0)!=0xC0&&(note->code&0xf0)!=0xD0&&note->code!=0xf3){
								note->data2 = file.get();
								c += 2;
							} else ++c;
						} else if (note->code == 0xf0) {
							try{
								UINT32 sysexLength = ReadVarVal(file,c);
								file.ignore(sysexLength);
								c += sysexLength;
							} catch (exception &e){
								throw runtime_error(string("Failed to ignore system exclusive bytes: ").append(e.what()));
							}
							delete note;
							note = 0;
						}
						if (note != 0){
							track->push_back(note);
							//note->DebugPrint();
							//++trackNotes[trackNum];
						}

					} catch (exception &e){
						delete note;
						throw e;
					}
				}//end chars
				if (note == 0 || !note->isMeta || note->code != 0x2F){
					stringstream s;
					s<<"Track "<<(int)trackNum<<" did not end in an End-of-Track event!\nRead "<<c<<" chars instead of "<<chunkSize;
					throw runtime_error(s.str());
				}
			} //end tracks
			file.close();

			//combine tracks

			vector<vector<Note*>::iterator> trackIterators(numTracks);
			for (UINT16 track = 0; track < numTracks; track++){
				trackIterators[track] = tracks[track].begin();
			}
			vector<UINT64> trackTimeDiffs(numTracks,0);
			UINT64 currentTime = 0;
			UINT32 lowestTime;
			UINT32 timeDiff = 0;
			do {
				lowestTime = MAXUINT32;
				for (UINT16 track = 0; track < numTracks; track++){ //for each track
					while (trackIterators[track] != tracks[track].end()){ //for each note
						Note* note = *trackIterators[track];
						UINT64 noteTime = trackTimeDiffs[track]+note->deltaTime; //absolute time of note
						if (noteTime == currentTime){
							note->deltaTime = timeDiff;
							trackTimeDiffs[track] = currentTime;

							if (note->IsNoteOn()){
								if (note->data1 > highestNote) highestNote = note->data1;
								if (note->data1 < lowestNote) lowestNote = note->data1;
								if (note->data2 == 0) note->code &= 0x8f;
							}

							notes.push_back(note);
							timeDiff = 0;
							trackIterators[track]++;
						} else if (noteTime < currentTime) throw runtime_error("Failed to merge tracks!");
						else {
							if (noteTime - currentTime < lowestTime) lowestTime = (UINT32)(noteTime - currentTime);
							break;
						}
					} //end for each note
				} //end for each track
				timeDiff = lowestTime;
				currentTime += lowestTime;
			} while (lowestTime != MAXUINT32);

		} catch (exception &e){
			if (file.is_open()) file.close();
			stringstream s;
			for (auto i = tracks.begin(); i != tracks.end(); i++){
				for (auto j = (*i).begin(); j != (*i).end(); j++){
					delete *j;
				}
			}
			s<<"Error reading MIDI file \""<<filename<<"\":\n"<<e.what();
			throw runtime_error(s.str());
		}
	}

	Song::~Song(){
		EndPlaySong();
		for (auto note = notes.begin(); note != notes.end(); note++){
			delete (*note);
		}
	}

	HMIDIOUT Song::device = 0;
}