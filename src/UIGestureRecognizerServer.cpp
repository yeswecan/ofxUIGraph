//
//  UIGestureRecognizerServer.cpp
//  openFrameworksLib
//
//  Created by zebra on 04.11.15.
//
//

#include <stdio.h>
#include "UIGestureRecognizer.h"

std::map<int, UIGestureDataReciever*> UIGestureRecognizerServer::capturedFingers;
