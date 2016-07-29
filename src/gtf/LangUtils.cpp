//
//  GTFLangUtils.cpp
//  gtf
//
//  Created by David Gallardo on 30/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#include "gtf/LangUtils.h"

#include <cctype>
#include <cstring>

namespace gtf
{
	void getFileExtension(const char* path, char* outExt, int bufferSize)
	{
		const char * ext = strrchr(path, '.');

		if (!ext) return;
		//if (strlen(ext) > bufferSize) return;
		const char * extIt = ext;

		while (extIt && (extIt - ext < bufferSize))
		{
			outExt[extIt - ext] = tolower(*extIt);
			++extIt;
		}
	}
}