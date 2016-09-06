/*
  ==============================================================================

    BrainController.cpp
    Created: 4 Sep 2015 11:35:56am
    Author:  Joshua

	Controller class to abstract the use of the analysis classes (SenseMaker and Markov)
	away from PluginProcessor. Takes the frequency of the current note heard and returns
	the frequency of the next note to play.

  ==============================================================================
*/

#include "BrainController.h"

BrainController::BrainController()
{
	currentKeyTonic;
	key;
	currentChord = 1; //This will probably be the case at the start most of the time
	trackStarted = false;
	newChord = true;
	previousNotePlayed = 1;
	beatCounter = 0;
	noteCounter = 0;


	//Note scales
	C = { 1, 3, 5, 6, 8, 10, 12 };
	DbCs = { 2,4,6,7,9,11,1 };
	D = { 3,5,7,8,10,11,2 };
	EbDs = { 4,6,8,9,11,1,3 };
	E = { 5,7,9,10,12,2,4 };
	F = { 6,8,10,11,1,3,5 };
	GbFs = { 7,9,11,12,2,4,6 };
	G = { 8,10,12,1,3,5,7 };
	AbGs = { 9,11,1,2,4,6,8 };
	A = { 10,12,2,3,4,6,8 };
	BbAs = { 11,1,3,4,6,8,10 };
	BCb = { 12,2,4,5,7,9,11 };
}

BrainController::~BrainController(){}

double BrainController::clockTickFrequency(double currentFreq, bool isBeatTick, MidiBuffer& midiMessages)
{
	//Wait for the midi track to start so that chord doesn't get changed to something weird from background noise
	if (!trackStarted)
	{
		if (!isBeatTick){ return 0; }
		trackStarted = true;
	}

	if (allFreqHeard.size() >= 4) {
		allFreqHeard.erase(allFreqHeard.begin());
	}
	
	allFreqHeard.push_back(currentFreq);
		
	if (currentChord != 0) {
		if (!(std::find(chordsHeardSinceLastKey.begin(), chordsHeardSinceLastKey.end(), currentChord) != chordsHeardSinceLastKey.end())) {
			chordsHeardSinceLastKey.push_back(currentChord);
		}
	}

	if (chordsHeardSinceLastKey.size() <= 3) {
		key = mKeyDetermination.pickNewKeyMajor(chordsHeardSinceLastKey);
		if (key != -1) {
			currentKeyTonic = mSenseMaker.setKeyTonic(key);
		}
	}
	noteCounter++;
	int thisNote = mSenseMaker.frequencyToNoteInKey(currentFreq);
	if (thisNote != 0) {
		previousNotePlayed = thisNote;
		if (!(std::find(notesHeardSinceLastChord.begin(), notesHeardSinceLastChord.end(), thisNote) != notesHeardSinceLastChord.end())) {
			notesHeardSinceLastChord.push_back(thisNote);
		}
	}

	if (isBeatTick && (beatSkip != 0))
	{
		if (!(beatCounter >= beatSkip))
		{
			beatCounter ++;
			return 0;
		}
		beatCounter = 0;
	}


	int noteToPlay = 0;
	if (newChord)
	{
		noteToPlay = 1;
		newChord = false;
	}
	else {

		if (!doesNoteBelong(thisNote))
		{
			currentChord = pickNewChord(thisNote, currentChord);

			if (!(std::find(chordsHeardSinceLastKey.begin(), chordsHeardSinceLastKey.end(), currentChord) != chordsHeardSinceLastKey.end())) {
				if (currentChord != 0) {
					chordsHeardSinceLastKey.push_back(currentChord);
				}
					chordsHeardSinceLastKey.push_back(currentChord);
					if (chordsHeardSinceLastKey.size() > 3) {
						key = mKeyDetermination.pickNewKey(chordsHeardSinceLastKey);
						currentKeyTonic = mSenseMaker.setKeyTonic(key);
					}
					
			}

		}

	}
	if (previousNotePlayed <= 0) {
		previousNotePlayed = 1;
	}
	else if (previousNotePlayed > 8) {
		previousNotePlayed = 8;
	}
	noteToPlay = mMarkov.getNextNote(previousNotePlayed);
	previousNotePlayed = noteToPlay;
	
	int noteInKey = convertNoteFromChordToKey(noteToPlay, currentChord);

	bool foundSong = false;

	double temp = mDatabaseReader.compareArrayToSongs(allFreqHeard);
	if (temp >= 0) {
		foundSong = true;
	}
	
	if (foundSong) {
		return temp;
	}
	else {
		return mSenseMaker.noteInKeytoFrequency(noteInKey);

	}
}

int BrainController::convertNoteFromChordToKey(int note, int chord)
{
	int noteInKey = note + chord - 1;
	if (noteInKey > 7){ noteInKey -= 7; }
	return noteInKey;
}

int BrainController::convertNoteFromKeyToChord(int note, int chord)
{
	int noteInChord = note - chord + 1;
	if (noteInChord > 7){ noteInChord -= 7; }
	return noteInChord;
}

//returns the first key that it matches with order of preference C, Db/C#, D, Eb/D#, E, F, Gb/F3, G, Ab/G#, A, Bb/A#, B/Cb


// When a note is heard that is not in the current chord, this function is used to choose the new chord.
int BrainController::pickNewChord(int currentNote, int currentChordTransitionFrom)
{
	std::list<int> possibleChords = chordsNoteMayBelongTo(currentNote);
	int probableChord = mMarkov.getNextChord(currentChordTransitionFrom);
	if ((std::find(possibleChords.begin(), possibleChords.end(), probableChord) != possibleChords.end()))
	{
		//if it might be the suggested chord, then return suggested
		return probableChord;
	}
	//return the closest chord (this might not make the most sense musically, but for the timebeing, meh)
	int smallestDiff = 999;
	int smallestChord = 1;

	for (std::list<int>::const_iterator iterator = possibleChords.begin(), end = possibleChords.end(); iterator != end; ++iterator) {
		int diff = abs(*iterator - currentChordTransitionFrom);
		if (diff< smallestDiff)
		{
			smallestDiff = diff;
			smallestChord = *iterator;

		}
	}

	return smallestChord;
}
bool BrainController::doesChordBelong(int chord)
{
	std::vector<int> keyChords = convertChordToList(key);

	if (std::binary_search(keyChords.begin(), keyChords.end(), chord)) {
		return true;
	}
	if (chord == 0) { return true; } //Don't change chord on 0 note
	return false;
}

std::vector<int> BrainController::convertChordToList(int currentKey) {

	switch (currentKey)
	{
	case 0:
		return C;
	case 1:
		//Key of C
		return C;
	case 2:
		//Key of Db / C#: 2
		return DbCs;
	case 3:
		//Key of D: 3
		return D;
	case 4:
		//Key of Eb / D#: 4
		return EbDs;
	case 5:
		//Key of E: 5
		return E;
	case 6:
		//Key of F: 6
		return F;
	case 7:
		//Key of Gb / Key of F#: 7
		return GbFs;
	case 8:
		//Key of G: 8
		return G;
	case 9:
		//Key of Ab / G#: 9
		return AbGs;
	case 10:
		//Key of A: 10
		return A;
	case 11:
		//Key of Bb / A#: 11
		return BbAs;
	case 12:
		//Key of B / Key of Cb: 12
		return BCb;
	default:
		//Unknown key selected - default to C
		return C;
	}

}


// Check if a given note is in the current chord or not.
bool BrainController::doesNoteBelong(int note)
{
	int noteRelativeChord = convertNoteFromKeyToChord(note, currentChord);
	if (noteRelativeChord == 1 || noteRelativeChord == 3 || noteRelativeChord == 5)
	{
		return true;
	}
	//if (note == 0){ return true; } //Don't change chord on 0 note
	return false;
}

// Calculates the list of chords that contain a given note.
std::list<int> BrainController::chordsNoteMayBelongTo(int note)
{
	std::list<int> possibilites = std::list<int>();

	possibilites.push_back(note); //may be the tonic of its own chord

	int temp = note + 5;
	if (temp > 7){temp -= 7;}

	possibilites.push_back(temp); //may be the 3rd of a chord

	temp = note + 3;
	if (temp > 7){ temp -= 7; }

	possibilites.push_back(temp); //may be the 5th of a chord

	return possibilites;
}

