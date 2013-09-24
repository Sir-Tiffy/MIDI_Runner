#include "Stdafx.h"
#include "FileReader.h"

UINT32 ReadVarVal(ifstream& file, UINT32& total){
	UINT32 result = 0;
	unsigned char c;
	unsigned char counter = 0;
	do {
		c = file.get();
		++counter;
		result = (result<<7)|(c&0x7f);
	} while (c & 0x80);
	total += counter;
	if (counter > 4){
		stringstream s;
		s<<"Variable lengh number at position "<<((UINT32)file.tellg()-counter)<<" read "<<counter<<" bytes!";
		throw runtime_error(s.str());
	}
	return result;
}

UINT32 ReadNum32(ifstream& file){
	UINT32 result = file.get();
	for (unsigned char i = 0; i < 3; i++){
		result = (result<<8) | file.get();
	}
	return result;
}

UINT16 ReadNum16(ifstream& file){
	UINT16 result = file.get();
	return (result<<8)|file.get();
}