#pragma once
#include "StdAfx.h"

namespace MIDI{
	class Note{
	public:
		UINT32 deltaTime;
		unsigned char code, track, data1, data2;
		bool isMeta;
		bool done;
		UINT32 metaLength;
		vector<unsigned char> metaData;

		bool IsNoteOn();
		bool IsNoteOff();
		void DebugPrint();
		void PlayNote(HMIDIOUT device);
		Note();
	};

	class Song{
	protected:
		double
			startTime,
			targetTime;
		vector<Note*>::iterator noteIterator;

	public:
		static HMIDIOUT device;
		double tempo;
		unsigned char highestNote, lowestNote;
		vector<Note*> notes;
		bool
			playing,
			done;
		UINT16 numTracks;
		UINT16 ticksPerBeat;

		void BeginPlaySong(double currentTime);
		double UpdatePlaySong(double currentTime, double deltaTime, bool paused);
		void EndPlaySong();
		Song(string filename);
		virtual ~Song();
	};
}
