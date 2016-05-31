//
//  GTFNodeGraph.h
//  gtf
//
//  Created by David Gallardo on 31/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#define GTF_MAX_NODE_CONNECTION_COUNT 32

#include <functional>
#include <list>
#include <vector>

#include "GTFUniqueId.h"

/*
enum class GTFNodeConnectionType : unsigned char
{
	NCT_Color,
	NCT_Vec3,
	NCT_Float,
	NCT_Int,
    NCT_Texture_R8,
    NCT_Texture_RGB8,
    NCT_RESERVED0,
    NCT_RESERVED1,
    NCT_RESERVED2,
    NCT_RESERVED3
};*/

struct GTFNodeConnectionType
{
public:
    GTFUniqueId conTypeId;
    bool operator==(GTFNodeConnectionType const & other) const { return other.conTypeId == conTypeId; };
};

struct GTFNodeConnectionDesc
{
public:
    inline GTFNodeConnectionDesc(std::string const & _n, GTFNodeConnectionType const & _t)
    : name(_n)
    , type(_t) { }
    std::string name;
    GTFNodeConnectionType type;
    bool isInput { false };
    bool checkType(GTFNodeConnectionDesc const & other) const { return other.type == type; }
};

struct GTFNodeType
{
public:
	GTFUniqueId unique_id;
	GTFNodeConnectionDesc inputConnections[GTF_MAX_NODE_CONNECTION_COUNT];
	GTFNodeConnectionDesc outputConnections[GTF_MAX_NODE_CONNECTION_COUNT];
    std::function<void(class GTFNodeType*, class GTFNode*)> drawBodyFunction { nullptr };
};


struct GTFNodeConnection
{
	GTFNodeConnectionDesc desc;
    
	inline GTFNodeConnection(GTFNodeConnectionDesc _desc)
    : desc(_desc)
	{
		input = nullptr;
	}
    
    union
    {
        GTFNodeConnection* input;
        std::list<GTFNodeConnection*> output;
    };
};

class GTFNode
{
public:
    GTFNode(GTFUniqueId _typeId);
    
    GTFUniqueId typeId;
    int posX, posY;
    std::string name;
	std::vector<GTFNodeConnection*> inputConnections;
	std::vector<GTFNodeConnection*> outputConnections;
};

class GTFNodeGraph
{
public:
    void registerConnectionType(GTFNodeConnectionType const & connectionType);
    void registerNodeType(GTFNodeType* nodeType);
    
    std::list<GTFNode*> nodeList;
    
};
