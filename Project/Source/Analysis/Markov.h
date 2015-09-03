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

	int getNextNote(int currentNote, int chord);

private:
	void doTransition();

	std::vector<std::vector<float>> matrix(std::vector<float>());
};


#endif  // MARKOV_H_INCLUDED
