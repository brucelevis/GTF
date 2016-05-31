//
//  GTFUniqueId.cpp
//  gtf
//
//  Created by David Gallardo on 31/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#include "GTFUniqueId.h"
#include "GTFCRC32.h"

#include <map>

std::map<uint32_t, std::string> GTF_UNIQUEID_REGISTRY;

void GTFUniqueId::reg(uint32_t uniqueId, const char * nameStr)
{
    auto ref = GTF_UNIQUEID_REGISTRY.find(m_idU32);
    if(ref == GTF_UNIQUEID_REGISTRY.end())
    {
        GTF_UNIQUEID_REGISTRY.insert(std::make_pair(uniqueId, std::string(nameStr)));
    }
}

GTFUniqueId::GTFUniqueId()
: m_idU32(0)
{}

GTFUniqueId::GTFUniqueId(const char * nameStr)
: m_idU32(GTF_CRC32(nameStr))
{
    reg(m_idU32, nameStr);
}

GTFUniqueId::GTFUniqueId(uint32_t uniqueId)
{
    m_idU32 = uniqueId;
}

GTFUniqueId::GTFUniqueId(uint32_t uniqueId, const char * nameStr)
{
    m_idU32 = uniqueId;
    
    reg(m_idU32, nameStr);
}

GTFUniqueId::operator uint32_t() const
{
    return m_idU32;
}

GTFUniqueId::operator const char *() const
{
    return GTF_UNIQUEID_REGISTRY[m_idU32].c_str();
}

GTFUniqueId::operator std::string() const
{
    return GTF_UNIQUEID_REGISTRY[m_idU32];
}

const char * GTFUniqueId::operator*() const
{
    return GTF_UNIQUEID_REGISTRY[m_idU32].c_str();
}

bool GTFUniqueId::operator <(GTFUniqueId const & other) const
{
    return m_idU32 < other.m_idU32;
}

bool GTFUniqueId::operator ==(GTFUniqueId const & other) const
{
    return m_idU32 == other.m_idU32;
}

GTFUniqueId const & GTFUniqueId::operator=(GTFUniqueId const & other)
{
    m_idU32 = other.m_idU32;
    return *this;
}

GTFUniqueId const & GTFUniqueId::operator=(const char * nameStr)
{
    m_idU32 = GTF_CRC32(nameStr);
    return *this;
}

GTFUniqueId const & GTFUniqueId::operator=(uint32_t uniqueId)
{
    m_idU32 = uniqueId;
    return *this;
}

