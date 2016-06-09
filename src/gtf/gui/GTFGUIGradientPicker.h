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
    static void displayWidget(bool* show, GTFGradient* gradient);
    static void showPopupFor(bool* show, GTFGradient* gradient);

};
