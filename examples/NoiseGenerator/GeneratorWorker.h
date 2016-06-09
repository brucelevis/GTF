//
//  GeneratorWorker.h
//  gtf
//
//  Created by David Gallardo on 29/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#pragma once 

#include <thread>
#include "GTFGradient.h"

struct GeneratorInfo
{
    unsigned int seed { 200 };
    bool reestrictPan { true };
    float panX { 1 };
    float panY { 1 };
    float panZ { 1 };
    float density { 20 };
    unsigned int resX { 512 };
    unsigned int resY { 512 };
    unsigned char* image { nullptr };
    bool wood { false };
    bool dirty { false };
};

class GeneratorWorker
{
public:
    GeneratorWorker();
    bool update(GeneratorInfo& info);
private:
    void run();
    GeneratorInfo m_info;
    std::thread m_workingThread;
    bool m_cancel { false };
    bool m_running { false };
    bool m_ready { false };
    
    GTFGradient m_gradient;
};
