

#ifndef DATABASEREADER_H_INCLUDED
#define DATABASEREADER_H_INCLUDED

#include "../Logging.h"
#include <JuceHeader.h>

class DatabaseReader {
public:
	DatabaseReader();
	~DatabaseReader();

	double compareArrayToSongs(std::vector<double> songPlaying);
	double getNextNote(int song, int length);
	//int getNextNote(int currentNote);
	//int getNextChord(int currentChord);

private:

	std::vector<double> data;
	int currVector;
	//std::vector<std::vector<double>>* chord;
};

#endif

