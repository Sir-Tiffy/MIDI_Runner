#pragma once
#include "StdAfx.h"
namespace PopupFlags{
	enum PopupFlags{
		#ifdef WIN32
			OK = MB_OK,
			WARNING = MB_ICONWARNING,
			INFO = MB_ICONINFORMATION,
			ERR = MB_ICONERROR
		#else
			OK,WARNING,INFO,ERR
		#endif
	};
}

void Popup(std::string msg, std::string title, int flags);
#define PopupError(msg) Popup(msg,"Error!",PopupFlags::OK|PopupFlags::ERR)
