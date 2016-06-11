//
//  GTFGUIGradientPicker.h
//  gtf
//
//  Created by David Gallardo on 30/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#pragma once

#include "GTFGradient.h"

class GTFGUIGradientPicker
{
public:
    static bool displayWidget(bool* showPopup,
                              GTFGradient* gradient,
                              GTFGradientMark* & draggingMark,
                              GTFGradientMark* & selectedMark);
    static bool showPopupFor(bool* showPopup,
                             GTFGradient* gradient,
                             GTFGradientMark* & draggingMark,
                             GTFGradientMark* & selectedMark);
private:
    static void drawBar(GTFGradient* gradient,
                        bool withMarks,
                        GTFGradientMark* & draggingMark,
                        GTFGradientMark* & selectedMark,
                        struct ImVec2 const & bar_pos,
                        float maxWidth,
                        float height);
    
};
