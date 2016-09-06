

#ifndef KEYDECIDER_H_INCLUDED
#define KEYDECIDER_H_INCLUDED

#include "../Logging.h"
#include <JuceHeader.h>
#include <list>

class KeyDecider {
public:
	KeyDecider();
	~KeyDecider();

	int pickNewKeyMajor(std::list<int> chordsHeardSinceLastKey);
	int pickNewKey(std::list<int> chordsHeardSinceLastKey);

private:

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

	std::vector<int> CM;
	std::vector<int> DbCsM;
	std::vector<int> DM;
	std::vector<int> EbDsM;
	std::vector<int> EM;
	std::vector<int> FM;
	std::vector<int> GbFsM;
	std::vector<int> GM;
	std::vector<int> AbGsM;
	std::vector<int> AM;
	std::vector<int> BbAsM;
	std::vector<int> BCbM;
};

#endif

