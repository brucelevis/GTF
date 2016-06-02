//
//  GTFUniqueId.h
//  gtf
//
//  Created by David Gallardo on 31/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#pragma once

#include <string>
#include <cstdint>
#include "GTFCRC32.h"

#define GTF_UID(__str) (GTFUniqueId(gtfcrc32::WSID_CONST(__str), __str))
#define GTF_HASH(__str) (gtfcrc32::WSID_CONST(__str))

//typedef BHashMap<U32, BString> BNameRegistry;

class GTFUniqueId
{
    
public:
    GTFUniqueId();
    explicit GTFUniqueId(const char * nameStr);
    explicit GTFUniqueId(uint32_t uniqueId);
    GTFUniqueId(uint32_t uniqueId, const char * nameStr);
    operator uint32_t() const;
    operator const char*() const;
    operator std::string() const;
    const char * operator*() const;
    bool operator <(GTFUniqueId const & other) const;
    bool operator ==(GTFUniqueId const & other) const;
    GTFUniqueId const & operator=(GTFUniqueId const & other);
    GTFUniqueId const & operator=(const char * nameStr);
    GTFUniqueId const & operator=(uint32_t uniqueId);
private:
    void reg(uint32_t uniqueId, const char * nameStr);
    uint32_t m_idU32;
};

