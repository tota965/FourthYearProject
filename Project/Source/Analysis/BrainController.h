/*
  ==============================================================================

    BrainController.h
    Created: 4 Sep 2015 11:35:56am
    Author:  Joshua

	Controller class to abstract the use of the analysis classes (SenseMaker and Markov)
	away from PluginProcessor. Takes the frequency of the current note heard and returns
	the frequency of the next note to play.

  ==============================================================================
*/

#ifndef BRAINCONTROLLER_H_INCLUDED
#define BRAINCONTROLLER_H_INCLUDED
#include "SenseMaker.h"
#include "Markov.h"
#include "DatabaseReader.h"
#include "KeyDetermination.h"
#include <list>
#include <vector>

class BrainController
{
public:
	BrainController();
	~BrainController();

	double clockTickFrequency(double currentFreq, bool isBeatTick, MidiBuffer& midiMessages);

	void SetVolume(float newVolume){ volume = newVolume; };
	float GetVolume(void){ return volume; };

	void SetBeat(int newBeat) { beat = newBeat; beatSkip = newBeat - 1; if (newBeat == 3){ beatSkip++; } };
	int GetBeat(void){ return beat; };
	/* Beats:
				1/16: 1
				1/8: 2
				1/4: 3
			*/

	void SetKey(int newKey) {
		key = newKey;

		//where the key is set
	currentKeyTonic = mSenseMaker.setKeyTonic(key);
	};
	int GetKey(void){ return key; };

private:

	SenseMaker mSenseMaker;
	Markov mMarkov;
	DatabaseReader mDatabaseReader;
	KeyDetermination mKeyDetermination;
	std::list<int> notesHeardSinceLastChord;
	std::list<int> chordsHeardSinceLastKey;
	std::vector<int> convertChordToList(int currentKey);
	std::vector<double> allFreqHeard;

	bool doesNoteBelong(int note);
	bool doesChordBelong(int chord);

	std::list<int> chordsNoteMayBelongTo(int note);
	int convertNoteFromChordToKey(int note, int chord);
	int convertNoteFromKeyToChord(int note, int chord);
	int pickNewChord(int currentNote, int currentChord);

	double currentKeyTonic;
	int currentChord;
	int previousNotePlayed;
	bool newChord;
	int noteCounter;

	std::vector<int> C;
	std::vector<int> DbCs;
	std::vector<int> D;
	std::vector<int> EbDs;
	std::vector<int> E;
	std::vector<int> F;
	std::vector<int> GbFs;
	std::vector<int> G;
	std::vector<int> AbGs;
	std::vector<int> A;
	std::vector<int> BbAs;
	std::vector<int> BCb;

	int beatCounter;
	int beatSkip;

	bool trackStarted;

	// Values from GUI
	float volume;
	int key;
	int beat;
};



#endif  // BRAINCONTROLLER_H_INCLUDED
