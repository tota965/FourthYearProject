/*
==============================================================================

DatabaseReader.cpp
Author:  Toni

Takes the last 4 notes playes and uses a pattern matching algorithm to check to see whether the current input is in a repeating segment.
Returns either the next note to play, or -1.

==============================================================================
*/

#include "DatabaseReader.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

DatabaseReader::DatabaseReader() {

	//vector<vector<double>> data;
	std::ifstream songs;
	songs.open("H:\\noteFrequencies.txt");
	while (songs) {
		string s;
		if (!getline(songs, s)) break;

		istringstream ss(s);
		vector<double> record;
		while (ss) {
			string s;
			if (!getline(ss, s, ',')) break;
			double s1 = atoi(s.c_str());
			data.push_back(s1);
		}

		//data.push_back(record);

	}


}

DatabaseReader::~DatabaseReader() {

}

double DatabaseReader::compareArrayToSongs(std::vector<double> songPlaying)
{
	std::vector<int> pos;
	int foundSong = -1;
	currVector = -1;
	if ((data.size() > 0) && (songPlaying.size() > 0)) {
		std::vector<double>::iterator iter = data.begin();
		while ((iter = std::find(iter, data.end(), songPlaying[0])) != data.end()) {
			int idx = std::distance(data.begin(), iter);
			pos.push_back(idx);
		}

		for (int i = 0; i < pos.size(); i++) {
			int freqPos = pos[i];
			if (data[freqPos + 4] != NULL) {
				if ((data[freqPos + 1] == songPlaying[1]) && (data[freqPos + 2] == songPlaying[2]) && (data[freqPos + 3] == songPlaying[3])) {
					return data[freqPos + 4];
				}
			}
		}
	}

		return -1;

}

double DatabaseReader::getNextNote(int song, int length) {

	std::vector<double> tempSong = data;

	if (tempSong.size() > length) {
		return tempSong[length + 1];

	}
	else {
		return 0;
	}

}



