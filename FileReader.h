#pragma once
#include "StdAfx.h"

UINT16 ReadNum16(ifstream& file);
UINT32 ReadNum32(ifstream& file);
UINT32 ReadVarVal(ifstream& file, UINT32& counter);