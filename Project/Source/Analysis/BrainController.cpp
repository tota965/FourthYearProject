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
	if (currentChord != 0) {
		if (!(std::find(chordsHeardSinceLastKey.begin(), chordsHeardSinceLastKey.end(), currentChord) != chordsHeardSinceLastKey.end())) {
			chordsHeardSinceLastKey.push_back(currentChord);
		}
	}

	int thisNote = mSenseMaker.frequencyToNoteInKey(currentFreq);
	if (thisNote != 0) {
		previousNotePlayed = thisNote;
		if (!(std::find(notesHeardSinceLastChord.begin(), notesHeardSinceLastChord.end(), thisNote) != notesHeardSinceLastChord.end())) {
			notesHeardSinceLastChord.push_back(thisNote);

			if (notesHeardSinceLastChord.size() < 4) {
				return mSenseMaker.noteInKeytoFrequency(thisNote);
			}
			else {
				key = pickNewKey();
				currentKeyTonic = mSenseMaker.setKeyTonic(key);
			}
		}
	}

	//key = pickNewKey();
	//currentKeyTonic = mSenseMaker.setKeyTonic(key);
	//int currentNote = mSenseMaker.frequencyToNoteInKey(currentFreq);

	if (isBeatTick && (beatSkip != 0))
	{
		if (!(beatCounter >= beatSkip))
		{
			beatCounter ++;
			return 0;
		}
		beatCounter = 0;
	}

	//notesHeardSinceLastChord.push_back(currentNote);

	int noteToPlay = 0;
	if (newChord)
	{
		noteToPlay = 1;
		//previousNotePlayed = 1;
		newChord = false;
	}
	else {

		if (!doesNoteBelong(thisNote))
		{
			currentChord = pickNewChord(thisNote, currentChord);
			if (!(std::find(chordsHeardSinceLastKey.begin(), chordsHeardSinceLastKey.end(), currentChord) != chordsHeardSinceLastKey.end())) {
				//if (chordsHeardSinceLastKey.size() < 7 && currentChord != 0) {
					chordsHeardSinceLastKey.push_back(currentChord);
					/*key = pickNewKey();
					if (key == 0) {
						chordsHeardSinceLastKey.clear();
						chordsHeardSinceLastKey.push_back(currentChord);
						key = pickNewKey();
					}
					currentKeyTonic = mSenseMaker.setKeyTonic(key);*/

				//}
				//else {
					//chordsHeardSinceLastKey.clear();
					//chordsHeardSinceLastKey.push_back(currentChord);
					//key = pickNewKey();
					//currentKeyTonic = mSenseMaker.setKeyTonic(key);
				//}
			}

			//noteToPlay = 1;
			//previousNotePlayed = 1;
			//notesHeardSinceLastChord.clear();
			//notesHeardSinceLastChord.push_back(currentNote);
		}
		/*else{
			noteToPlay = mMarkov.getNextNote(previousNotePlayed);
			previousNotePlayed = noteToPlay;
		}*/
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

	return mSenseMaker.noteInKeytoFrequency(noteInKey);
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
int BrainController::pickNewKey() {

	int newKey = 0;
	int currentMax = 0;

	//checking if current chords match C key
	int C_count = 0;
	for (std::list<int>::iterator it = notesHeardSinceLastChord.begin(); it != notesHeardSinceLastChord.end(); ++it) {
		if ((std::find(C.begin(), C.end(), *it) != C.end())) {
			C_count++;
		}
	}

	newKey = 1;
	currentMax = C_count;
	//checking if current chords match Db or C# keys
	int DbCs_count = 0;
	for (std::list<int>::iterator it = notesHeardSinceLastChord.begin(); it != notesHeardSinceLastChord.end(); ++it) {
		if ((std::find(DbCs.begin(), DbCs.end(), *it) != DbCs.end())) {
			DbCs_count++;
		}
	}
	if (DbCs_count > currentMax) {
		currentMax = DbCs_count;
		newKey = 2;
	}
	
	//checking if current chords match D key 
	int D_count = 0;
	for (std::list<int>::iterator it = notesHeardSinceLastChord.begin(); it != notesHeardSinceLastChord.end(); ++it) {
		if (!(std::find(D.begin(), D.end(), *it) != D.end())) {
			D_count++;
		}
	}
	if (D_count > currentMax) {
		currentMax = D_count;
		newKey = 3;
	}

	//checking if current chords match Eb or D# keys
	int EbDs_count = 0;
	for (std::list<int>::iterator it = notesHeardSinceLastChord.begin(); it != notesHeardSinceLastChord.end(); ++it) {
		if (!(std::find(EbDs.begin(), EbDs.end(), *it) != EbDs.end())) {
			EbDs_count++;
		}
	}
	if (EbDs_count > currentMax) {
		currentMax = EbDs_count;
		newKey = 4;
	}

	//checking if current chords match E key
	int E_count = 0;
	for (std::list<int>::iterator it = notesHeardSinceLastChord.begin(); it != notesHeardSinceLastChord.end(); ++it) {
		if (!(std::find(E.begin(), E.end(), *it) != E.end())) {
			E_count++;
		}
	}
	if (E_count > currentMax) {
		currentMax = E_count;
		newKey = 5;
	}

	//checking if current chords match F key
	int F_count = 0;
	for (std::list<int>::iterator it = notesHeardSinceLastChord.begin(); it != notesHeardSinceLastChord.end(); ++it) {
		if (!(std::find(F.begin(), F.end(), *it) != F.end())) {
			F_count++;
		}
	}
	if (F_count > currentMax) {
		currentMax = F_count;
		newKey = 6;
	}

	//checking if current chords match Gb or F# keys
	int GbFs_count = 0;
	for (std::list<int>::iterator it = notesHeardSinceLastChord.begin(); it != notesHeardSinceLastChord.end(); ++it) {
		if (!(std::find(GbFs.begin(), GbFs.end(), *it) != GbFs.end())) {
			GbFs_count++;
		}
	}
	if (GbFs_count > currentMax) {
		currentMax = GbFs_count;
		newKey = 7;
	}


	//checking if current chords match G key
	int G_count = 0;
	for (std::list<int>::iterator it = notesHeardSinceLastChord.begin(); it != notesHeardSinceLastChord.end(); ++it) {
		if (!(std::find(G.begin(), G.end(), *it) != G.end())) {
			G_count++;
		}
	}
	if (G_count > currentMax) {
		currentMax = G_count;
		newKey = 8;
	}

	//checking if current chords match Ab or G# keys
	int AbGs_count = 0;
	for (std::list<int>::iterator it = notesHeardSinceLastChord.begin(); it != notesHeardSinceLastChord.end(); ++it) {
		if (!(std::find(AbGs.begin(), AbGs.end(), *it) != AbGs.end())) {
			AbGs_count++;
		}
	}
	if (AbGs_count > currentMax) {
		currentMax = AbGs_count;
		newKey = 9;
	}

	//checking if current chords match A key
	int A_count = 0;
	for (std::list<int>::iterator it = notesHeardSinceLastChord.begin(); it != notesHeardSinceLastChord.end(); ++it) {
		if (!(std::find(A.begin(), A.end(), *it) != A.end())) {
			A_count++;
		}
	}
	if (A_count > currentMax) {
		currentMax = A_count;
		newKey = 10;
	}

	//checking if current chords match Bb or A# keys
	int BbAs_count = 0;
	for (std::list<int>::iterator it = notesHeardSinceLastChord.begin(); it != notesHeardSinceLastChord.end(); ++it) {
		if (!(std::find(BbAs.begin(), BbAs.end(), *it) != BbAs.end())) {
			BbAs_count++;
		}
	}
	if (BbAs_count > currentMax) {
		currentMax = BbAs_count;
		newKey = 11;
	}

	//checking if current chords match B or Cb keys
	int BCb_count = 0;
	for (std::list<int>::iterator it = notesHeardSinceLastChord.begin(); it != notesHeardSinceLastChord.end(); ++it) {
		if (!(std::find(BCb.begin(), BCb.end(), *it) != BCb.end())) {
			BCb_count++;
		}
	}
	if (BCb_count > currentMax) {
		currentMax = BCb_count;
		newKey = 12;
	}
	
	return newKey;
}

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

