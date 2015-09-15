/*
  ==============================================================================

    BrainController.cpp
    Created: 4 Sep 2015 11:35:56am
    Author:  Joshua

  ==============================================================================
*/

#include "BrainController.h"

BrainController::BrainController()
{
	currentKeyTonic = 0;
	currentChord = 1; //This will probably be the case at the start most of the time
	trackStarted = false;
	newChord = true;
	previousNotePlayed = 1;
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

	int currentNote = mSenseMaker.frequencyToNoteInKey(currentFreq);

	if (currentNote == 0){ return 0; }

	notesHeardSinceLastChord.push_back(currentNote);

	int noteToPlay = 0;
	if (newChord)
	{
		noteToPlay = 1;
		previousNotePlayed = 1;
		newChord = false;
	}
	else {

		if (!doesNoteBelong(currentNote))
		{
			currentChord = pickNewChord(currentNote, currentChord);
			noteToPlay = 1;
			previousNotePlayed = 1;
			notesHeardSinceLastChord.clear();
			notesHeardSinceLastChord.push_back(currentNote);
		}
		else{
			noteToPlay = mMarkov.getNextNote(previousNotePlayed);
			previousNotePlayed = noteToPlay;
		}
	}
	
	int noteInKey = convertNoteFromChordToKey(noteToPlay, currentChord);

#ifdef WIN32
	//LOG("The key is " + std::to_string(currentKeyTonic) + " The current note is " + std::to_string(currentNote) + " Is a beat " + std::to_string(isBeatTick));
	LOG("The key is " + std::to_string(currentKeyTonic) + " The current Chord is " + std::to_string(currentChord) + " The current note is " + std::to_string(noteInKey) + " Is a beat " + std::to_string(isBeatTick) + " Note Heard: " + std::to_string(currentNote));
#endif

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
		int smallestChord = 0;

		for (std::list<int>::const_iterator iterator = possibleChords.begin(), end = possibleChords.end(); iterator != end; ++iterator) {
			std::cout << *iterator;
			int diff = abs(*iterator - currentChordTransitionFrom);
			if (diff< smallestDiff)
			{
				smallestDiff = diff;
				smallestChord = *iterator;

			}
		}

		return smallestChord;
}


bool BrainController::doesNoteBelong(int note)
{
	int noteRelativeChord = convertNoteFromKeyToChord(note, currentChord);
	if (noteRelativeChord == 1 || noteRelativeChord == 3 || noteRelativeChord == 5)
	{
		return true;
	}
	return false;
}

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

