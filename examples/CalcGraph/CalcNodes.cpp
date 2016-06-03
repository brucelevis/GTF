//
//  CalcNodes.cpp
//  gtf
//
//  Created by David Gallardo on 31/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#include "CalcNodes.h"

void CalcNumberNode::update()
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

void CalcMathOpNode::update()
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