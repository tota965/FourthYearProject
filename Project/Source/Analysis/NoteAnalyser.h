/*
  ==============================================================================

    NoteAnalyser.h
    Created: 27 Jul 2015 4:21:34pm
    Author:  Michael

  ==============================================================================
*/

#ifndef NOTEANALYSER_H_INCLUDED
#define NOTEANALYSER_H_INCLUDED

#include "../Enums.h"
#include "../Logging.h"
#include <JuceHeader.h>

class NoteAnalyser {
public:
	NoteAnalyser();
	~NoteAnalyser();

	float getNextNote(float currentFrequency);

private:
	void doTransition();

	int rows, columns = 8;
};


#endif  // NOTEANALYSER_H_INCLUDED