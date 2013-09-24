#include "stdafx.h"
#include "PopupMsg.h"

void Popup(std::string msg,std::string title, int flags){
	#ifdef WIN32
		MessageBox(0,(LPCSTR)msg.c_str(), (LPCSTR)title.c_str(), flags);
	#else
		std::cout<<msg<<std::endl;
	#endif
}
