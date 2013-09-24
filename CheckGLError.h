#pragma once

void CheckErrorWithInfo(string msg, const char* filename, int line);
#define CheckError(msg) CheckErrorWithInfo(msg,__FILE__,__LINE__)