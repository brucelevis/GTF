//
//  CalcNodes.h
//  gtf
//
//  Created by David Gallardo on 31/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#pragma once

#include "GTFNodeGraph.h"

#include <cstdint>
#include <list>
#include <vector>

#define MAX_CONNECTION_COUNT 32

enum class ECalcConnectionType : uint8_t
{
    CCT_INT32
};

enum class ECalcNodeType : uint8_t
{
    CNT_UNDEFINED,
    CNT_NUMBER,
    CNT_ADD,
    CNT_SUBSTRACT,
    CNT_MULTIPLY,
    CNT_DIVIDE
};

struct ConnectionDesc
{
	const char* name;
	ECalcConnectionType type;
};


class CalcNode : public GTFNode
{
public:
    CalcNode(ECalcNodeType _t) : type(_t){};
    
    bool dirty { true };
    ECalcNodeType type;
    int number { 0 };
    
    static CalcNode* CAST(GTFNode* node){ return dynamic_cast<CalcNode*>(node); }
};

class CalcNumberNode : public CalcNode
{
public:
    CalcNumberNode(ECalcNodeType _t) : CalcNode(_t){};
    CalcNumberNode() : CalcNode(ECalcNodeType::CNT_NUMBER)
    {
        number = rand() % 60;
    };
    
    void update() override
    {
        if(dirty)
        {
            
            for (GTFNodeConnectionBase* con : outputConnections)
            {
                GTFNodeConnectionI32 * numberCon = GTFNodeConnectionI32::CAST(con);
                numberCon->data = number;
            }
            
            for (GTFNodeConnectionBase* con : outputConnections[0]->output)
            {
                GTFNodeConnectionI32 * numberCon = GTFNodeConnectionI32::CAST(con);
                numberCon->isDirty = true;
            }
            
            outputConnections[0]->isReady = true;
            dirty = false;
        }
    }
    
};

class CalcMathOpNode : public CalcNumberNode
{
public:
    CalcMathOpNode(ECalcNodeType _t) : CalcNumberNode(_t){};
    void update() override
    {
        if(inputConnections[0]->isDirty || inputConnections[1]->isDirty)
        {
            dirty = true;
        }
        
        if(dirty)
        {
            GTFNodeConnectionI32 * inputA = GTFNodeConnectionI32::CAST(inputConnections[0]);
            GTFNodeConnectionI32 * inputB = GTFNodeConnectionI32::CAST(inputConnections[1]);
            
            int readyInputs = 0;
            
            if(inputConnections[0]->input != nullptr &&
               inputConnections[0]->input->isReady)
            {
                GTFNodeConnectionI32 const * inputASource = GTFNodeConnectionI32::CAST(inputConnections[0]->input);
                inputA->data = inputASource->data;
                readyInputs++;
            }
            else
            {
                inputA->data = 0;
            }
            
            if(inputConnections[1]->input != nullptr &&
               inputConnections[1]->input->isReady)
            {
                GTFNodeConnectionI32 const * inputBSource = GTFNodeConnectionI32::CAST(inputConnections[1]->input);
                inputB->data = inputBSource->data;
                readyInputs++;
            }
            else
            {
                inputB->data = 0;
            }
            
            {
                inputA->isDirty = false;
                inputB->isDirty = false;
                GTFNodeConnectionI32 * output = GTFNodeConnectionI32::CAST(outputConnections[0]);
                
                number = output->data = MathOp(inputA->data, inputB->data);
                output->isReady = true; //(readyInputs == 2);

                for(auto otherOut : output->output)
                {
                    otherOut->isDirty = true;
                }
                
                dirty = false;
            }
        }
    }
    
    virtual int MathOp(int a, int b){ return 0; }
};

class CalcAddNode : public CalcMathOpNode
{
public:
    CalcAddNode() : CalcMathOpNode(ECalcNodeType::CNT_ADD){};
    int MathOp(int a, int b) override { return a + b; }
};

class CalcSubstractNode : public CalcMathOpNode
{
public:
    CalcSubstractNode() : CalcMathOpNode(ECalcNodeType::CNT_SUBSTRACT){};
    int MathOp(int a, int b){ return a - b; }
};

class CalcMultiplyNode : public CalcMathOpNode
{
public:
    CalcMultiplyNode() : CalcMathOpNode(ECalcNodeType::CNT_MULTIPLY){};
    int MathOp(int a, int b){ return a * b; }
};

class CalcDivideNode : public CalcMathOpNode
{
public:
    CalcDivideNode() : CalcMathOpNode(ECalcNodeType::CNT_DIVIDE){};
    int MathOp(int a, int b){ return (b == 0) ? 0 : a / b; }
};
