//
//  GTFNodeGraph.h
//  gtf
//
//  Created by David Gallardo on 31/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#pragma once

#include <functional>
#include <list>
#include <vector>
#include <map>

#include "GTFUniqueId.h"
#include "GTFRHI.h"


class GTFNodeConnectionDescBase
{
public:
    inline GTFNodeConnectionDescBase( GTFUniqueId _t, std::string const & _n)
    : unique_id(_t)
    ,  displayName(_n){}
    
    std::string displayName;
    GTFUniqueId unique_id;
    bool checkType(GTFNodeConnectionDescBase const * other) const { return other->unique_id == unique_id; }
    
    virtual class GTFNodeConnectionBase* createConnectionInstance(bool _input) = 0;
};

class GTFNodeTypeBase
{
public:
    inline GTFNodeTypeBase(GTFUniqueId uid, std::string name)
    : unique_id(uid)
    , displayName(name){};
    
	GTFUniqueId unique_id;
    std::string displayName;
    std::vector<GTFNodeConnectionDescBase*> inputConnectionsDesc;
	std::vector<GTFNodeConnectionDescBase*> outputConnectionsDesc;
    virtual class GTFNode* createNodeInstance() = 0;
    
};

class GTFNodeConnectionBase
{
public:
	inline GTFNodeConnectionBase(GTFNodeConnectionDescBase const * _desc, bool _input)
    : desc(_desc)
    , isInput(_input){};
    
    virtual ~GTFNodeConnectionBase(){};
    
    int posX, posY;
    bool isInput { false };
    bool isReady { true };
    bool isDirty { false };
    
    GTFNodeConnectionDescBase const * desc;
    
    //connections
    
    GTFNodeConnectionBase * input { nullptr };
    std::list<GTFNodeConnectionBase*> output;
    
};

template <typename T> class GTFNodeConnection : public GTFNodeConnectionBase
{
public:
    inline GTFNodeConnection(GTFNodeConnectionDescBase const * _desc, bool _input)
    : GTFNodeConnectionBase(_desc, _input){};
    
    using PTR_TYPE = GTFNodeConnection<T>*;
    using PTR_TYPE_CONST = GTFNodeConnection<T> const *;
    
    T data { 0 };
    
    static PTR_TYPE CAST(GTFNodeConnectionBase* con){ return dynamic_cast<PTR_TYPE>(con); };
    static PTR_TYPE_CONST CAST(GTFNodeConnectionBase const * con){ return dynamic_cast<PTR_TYPE_CONST>(con); };
};

using GTFNodeConnectionI32 = GTFNodeConnection<int32_t>;
using GTFNodeConnectionU32 = GTFNodeConnection<uint32_t>;
using GTFNodeConnectionF32 = GTFNodeConnection<float>;
using GTFNodeConnectionF32V3 = GTFNodeConnection<float[3]>;

class GTFNode
{
public:
    virtual void update() = 0;
    
    int posX, posY;
    int width, height;
    bool dirty;
    unsigned int nodeId;
    GTFNodeTypeBase const * type;
    std::string name;
	std::vector<GTFNodeConnectionBase*> inputConnections;
	std::vector<GTFNodeConnectionBase*> outputConnections;
};

template <typename CON_T> class GTFNodeConnectionDesc : public GTFNodeConnectionDescBase
{
public:
    inline GTFNodeConnectionDesc(GTFUniqueId _t, std::string const & _dn)
    : GTFNodeConnectionDescBase(_t, _dn){};
    
    GTFNodeConnectionBase* createConnectionInstance(bool _input) override
    {
        GTFNodeConnectionBase * con = new CON_T(this, _input);
        con->desc = this;
        return con;
    };
};

template <typename NODE_T> class GTFNodeType : public GTFNodeTypeBase
{
public:
    inline GTFNodeType(GTFUniqueId uid, std::string name) : GTFNodeTypeBase(uid, name){};
    GTFNode* createNodeInstance() override
    {
        GTFNode* node = new NODE_T();
        node->type = this;
        return node;
    };
};

class GTFNodeGraphType
{
public:
    GTFNodeGraphType(GTFUniqueId graphId) : unique_id(graphId){};
    
    void registerNodeType(GTFNodeTypeBase* nodeType);

    std::map<GTFUniqueId, GTFNodeTypeBase*> nodeTypeRegistry;
    
    GTFUniqueId unique_id;
};

class GTFNodeGraph
{
public:
    GTFNodeGraph(GTFNodeGraphType* type) : m_graphType(type){};
    virtual ~GTFNodeGraph();
    virtual GTFNode* createNode(GTFUniqueId typeId, int atX, int atY);
    virtual void updateNodes();
    virtual void updateGUI() = 0;
    
protected:
    GTFNodeGraphType * m_graphType { nullptr };
    std::list<GTFNode*> nodeList;
    
    unsigned int nodeCounter {0};
};
