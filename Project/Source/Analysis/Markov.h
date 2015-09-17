/*
  ==============================================================================

    Markov.h
    Created: 27 Jul 2015 4:21:34pm
    Author:  Michael

  ==============================================================================
*/

#ifndef MARKOV_H_INCLUDED
#define MARKOV_H_INCLUDED

#include "../Logging.h"
#include <JuceHeader.h>

class Markov {
public:
	Markov();
	~Markov();

	int getNextNote(int currentNote);
	int getNextChord(int currentChord);

private:

	std::vector<std::vector<double>>* note;
	std::vector<std::vector<double>>* chord;
};


#endif  // MARKOV_H_INCLUDED