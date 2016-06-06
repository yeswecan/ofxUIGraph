//
//  UIGestureRecognizerServer.cpp
//  openFrameworksLib
//
//  Created by zebra on 04.11.15.
//
//

#include <stdio.h>
#include "UIGestureRecognizer.h"
#include "UIFingerManager.h"

std::map<int, UIInputReceiver*> UIFingerManager::capturedFingers;
