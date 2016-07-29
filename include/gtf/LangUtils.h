//
//  GTFLangUtils.h
//  gtf
//
//  Created by David Gallardo on 30/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#pragma once

#define GTF_DEFAULT_CTORS_EMPTY_IMPL(__class) \
__class(){}; \
__class(__class const &){}; \
__class const & operator=(__class const &){ return *this; };

namespace gtf
{
	void getFileExtension(const char* path, char* outExt, int bufferSize);
}