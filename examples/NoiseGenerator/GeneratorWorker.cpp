//
//  GeneratorWorker.cpp
//  gtf
//
//  Created by David Gallardo on 29/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#include "GeneratorWorker.h"
#include "PerlinNoise.h"
#include "GTFGradient.h"

#include <cmath>
#include <thread>
#include <mutex>
#include <iostream>

GeneratorWorker::GeneratorWorker()
{
    m_gradient.marks.clear();
    m_gradient.addMark(0.0f, GTFColor(0xA0793D));
    m_gradient.addMark(0.2f, GTFColor(0xAA8347));
    m_gradient.addMark(0.3f, GTFColor(0xB48D51));
    m_gradient.addMark(0.4f, GTFColor(0xBE975B));
    m_gradient.addMark(0.6f, GTFColor(0xC8A165));
    m_gradient.addMark(0.7f, GTFColor(0xD2AB6F));
    m_gradient.addMark(0.8f, GTFColor(0xDCB579));
    m_gradient.addMark(1.0f, GTFColor(0xE6BF83));
}

bool GeneratorWorker::update(GeneratorInfo& info)
{
    if(info.dirty)
    {
        if(m_running)
        {
            m_cancel = true;
            m_workingThread.join();
            delete [] m_info.image;
        }
        
        m_info = info;
        m_ready = false;
        m_running = true;
        m_cancel = false;
        info.dirty = false;
        m_info.image = new unsigned char[m_info.resX * m_info.resY * 3u];
        m_workingThread = std::thread(&GeneratorWorker::run, this);
    }
    
    if(m_running && m_ready)
    {
        m_workingThread.join();
        m_running = false;
        m_ready = false;
        info.image = m_info.image;
        return true;
    }
    
    return false;
}

void GeneratorWorker::run()
{
    PerlinNoise pn(m_info.seed);

    unsigned num_cpus = std::thread::hardware_concurrency();
    std::cout << "Launching " << num_cpus << " threads" << std::endl;
    
    std::vector<std::thread> threads(num_cpus);
    unsigned int elementsPerCPU = m_info.resX * m_info.resY / num_cpus;
    
    for (unsigned threadIndex = 0; threadIndex < num_cpus; ++threadIndex)
    {
        threads[threadIndex] = std::thread([=]
        {
            // Visit every pixel of the image and assign a color generated with Perlin noise
            int from = threadIndex * elementsPerCPU;
            int to = (threadIndex + 1) * elementsPerCPU;
            GTFColor color;
            for(int e = from; e < to; e++)
            {
                int i = e % m_info.resX;
                int j = e / m_info.resY;
                
                if(m_cancel)
                {
                    return;
                }
                
                double x = (double)i/((double)m_info.resX);
                double y = (double)j/((double)m_info.resY);
                
                double n = 0;
                
                if(!m_info.wood)
                {
                    // Typical Perlin noise
                    n = pn.noise(m_info.panX * m_info.density * x, m_info.panY *  m_info.density * y, m_info.panZ);
                }
                else
                {
                    // Wood like structure
                    n = m_info.density * pn.noise(m_info.panX  * x, m_info.panY * y, m_info.panZ);
                    n = n - floor(n);
                }
                
                
                // Map the values to the [0, 255] interval, for simplicity we use
                // 50 shaders of grey
                m_gradient.getColorAt(n, color);
                color.asU8A(&m_info.image[(e*3)+0]);
                
                
                //m_info.image[(kk*3)+0] = floor(255 * n);
                //m_info.image[(kk*3)+1] = floor(255 * n);
                //m_info.image[(kk*3)+2] = floor(255 * n);
            }
        });
	}
    
    for (auto& t : threads) {
        t.join();
    }
    
    m_ready = true;
}