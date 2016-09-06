#include "KeyDetermination.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

KeyDetermination::KeyDetermination() {

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

	//Major scales
	CM = { 1,  6, 8 };
	DbCsM = { 2,7,9 };
	DM = { 3,8,10 };
	EbDsM = { 4,9,11 };
	EM = { 5,10,12 };
	FM = { 6,11,1 };
	GbFsM = { 7,12,2 };
	GM = { 8,1,3 };
	AbGsM = { 9,2,4 };
	AM = { 10,3,4 };
	BbAsM = { 11,4,6 };
	BCbM = { 12,5,7 };


}

KeyDetermination::~KeyDetermination() {

}

int KeyDetermination::pickNewKeyMajor(std::list<int> chordsHeardSinceLastKey)
{
	int newKey = 0;
	int currentMax = 0;

	//checking if current chords match C key
	int C_count = 0;
	for (std::list<int>::iterator it = chordsHeardSinceLastKey.begin(); it != chordsHeardSinceLastKey.end(); ++it) {
		if ((std::find(CM.begin(), CM.end(), *it) != CM.end())) {
			C_count++;
		}
	}

	newKey = 1;
	currentMax = C_count;
	//checking if current chords match Db or C# keys
	int DbCs_count = 0;
	for (std::list<int>::iterator it = chordsHeardSinceLastKey.begin(); it != chordsHeardSinceLastKey.end(); ++it) {
		if ((std::find(DbCsM.begin(), DbCsM.end(), *it) != DbCsM.end())) {
			DbCs_count++;
		}
	}
	if (DbCs_count > currentMax) {
		currentMax = DbCs_count;
		newKey = 2;
	}

	//checking if current chords match D key 
	int D_count = 0;
	for (std::list<int>::iterator it = chordsHeardSinceLastKey.begin(); it != chordsHeardSinceLastKey.end(); ++it) {
		if (!(std::find(DM.begin(), DM.end(), *it) != DM.end())) {
			D_count++;
		}
	}
	if (D_count > currentMax) {
		currentMax = D_count;
		newKey = 3;
	}

	//checking if current chords match Eb or D# keys
	int EbDs_count = 0;
	for (std::list<int>::iterator it = chordsHeardSinceLastKey.begin(); it != chordsHeardSinceLastKey.end(); ++it) {
		if (!(std::find(EbDsM.begin(), EbDsM.end(), *it) != EbDsM.end())) {
			EbDs_count++;
		}
	}
	if (EbDs_count > currentMax) {
		currentMax = EbDs_count;
		newKey = 4;
	}

	//checking if current chords match E key
	int E_count = 0;
	for (std::list<int>::iterator it = chordsHeardSinceLastKey.begin(); it != chordsHeardSinceLastKey.end(); ++it) {
		if (!(std::find(EM.begin(), EM.end(), *it) != EM.end())) {
			E_count++;
		}
	}
	if (E_count > currentMax) {
		currentMax = E_count;
		newKey = 5;
	}

	//checking if current chords match F key
	int F_count = 0;
	for (std::list<int>::iterator it = chordsHeardSinceLastKey.begin(); it != chordsHeardSinceLastKey.end(); ++it) {
		if (!(std::find(FM.begin(), FM.end(), *it) != FM.end())) {
			F_count++;
		}
	}
	if (F_count > currentMax) {
		currentMax = F_count;
		newKey = 6;
	}

	//checking if current chords match Gb or F# keys
	int GbFs_count = 0;
	for (std::list<int>::iterator it = chordsHeardSinceLastKey.begin(); it != chordsHeardSinceLastKey.end(); ++it) {
		if (!(std::find(GbFsM.begin(), GbFsM.end(), *it) != GbFsM.end())) {
			GbFs_count++;
		}
	}
	if (GbFs_count > currentMax) {
		currentMax = GbFs_count;
		newKey = 7;
	}


	//checking if current chords match G key
	int G_count = 0;
	for (std::list<int>::iterator it = chordsHeardSinceLastKey.begin(); it != chordsHeardSinceLastKey.end(); ++it) {
		if (!(std::find(GM.begin(), GM.end(), *it) != GM.end())) {
			G_count++;
		}
	}
	if (G_count > currentMax) {
		currentMax = G_count;
		newKey = 8;
	}

	//checking if current chords match Ab or G# keys
	int AbGs_count = 0;
	for (std::list<int>::iterator it = chordsHeardSinceLastKey.begin(); it != chordsHeardSinceLastKey.end(); ++it) {
		if (!(std::find(AbGsM.begin(), AbGsM.end(), *it) != AbGsM.end())) {
			AbGs_count++;
		}
	}
	if (AbGs_count > currentMax) {
		currentMax = AbGs_count;
		newKey = 9;
	}

	//checking if current chords match A key
	int A_count = 0;
	for (std::list<int>::iterator it = chordsHeardSinceLastKey.begin(); it != chordsHeardSinceLastKey.end(); ++it) {
		if (!(std::find(AM.begin(), AM.end(), *it) != AM.end())) {
			A_count++;
		}
	}
	if (A_count > currentMax) {
		currentMax = A_count;
		newKey = 10;
	}

	//checking if current chords match Bb or A# keys
	int BbAs_count = 0;
	for (std::list<int>::iterator it = chordsHeardSinceLastKey.begin(); it != chordsHeardSinceLastKey.end(); ++it) {
		if (!(std::find(BbAsM.begin(), BbAsM.end(), *it) != BbAsM.end())) {
			BbAs_count++;
		}
	}
	if (BbAs_count > currentMax) {
		currentMax = BbAs_count;
		newKey = 11;
	}

	//checking if current chords match B or Cb keys
	int BCb_count = 0;
	for (std::list<int>::iterator it = chordsHeardSinceLastKey.begin(); it != chordsHeardSinceLastKey.end(); ++it) {
		if (!(std::find(BCbM.begin(), BCbM.end(), *it) != BCbM.end())) {
			BCb_count++;
		}
	}
	if (BCb_count > currentMax) {
		currentMax = BCb_count;
		newKey = 12;
	}

	return newKey;

}

int KeyDetermination::pickNewKey(std::list<int> chordsHeardSinceLastKey) {

	int newKey = 0;
	int currentMax = 0;

	//checking if current chords match C key
	int C_count = 0;
	for (std::list<int>::iterator it = chordsHeardSinceLastKey.begin(); it != chordsHeardSinceLastKey.end(); ++it) {
		if ((std::find(C.begin(), C.end(), *it) != C.end())) {
			C_count++;
		}
	}

	newKey = 1;
	currentMax = C_count;
	//checking if current chords match Db or C# keys
	int DbCs_count = 0;
	for (std::list<int>::iterator it = chordsHeardSinceLastKey.begin(); it != chordsHeardSinceLastKey.end(); ++it) {
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
	for (std::list<int>::iterator it = chordsHeardSinceLastKey.begin(); it != chordsHeardSinceLastKey.end(); ++it) {
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
	for (std::list<int>::iterator it = chordsHeardSinceLastKey.begin(); it != chordsHeardSinceLastKey.end(); ++it) {
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
	for (std::list<int>::iterator it = chordsHeardSinceLastKey.begin(); it != chordsHeardSinceLastKey.end(); ++it) {
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
	for (std::list<int>::iterator it = chordsHeardSinceLastKey.begin(); it != chordsHeardSinceLastKey.end(); ++it) {
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
	for (std::list<int>::iterator it = chordsHeardSinceLastKey.begin(); it != chordsHeardSinceLastKey.end(); ++it) {
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
	for (std::list<int>::iterator it = chordsHeardSinceLastKey.begin(); it != chordsHeardSinceLastKey.end(); ++it) {
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
	for (std::list<int>::iterator it = chordsHeardSinceLastKey.begin(); it != chordsHeardSinceLastKey.end(); ++it) {
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
	for (std::list<int>::iterator it = chordsHeardSinceLastKey.begin(); it != chordsHeardSinceLastKey.end(); ++it) {
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
	for (std::list<int>::iterator it = chordsHeardSinceLastKey.begin(); it != chordsHeardSinceLastKey.end(); ++it) {
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
	for (std::list<int>::iterator it = chordsHeardSinceLastKey.begin(); it != chordsHeardSinceLastKey.end(); ++it) {
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



