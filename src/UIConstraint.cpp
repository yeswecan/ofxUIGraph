//
//  UIConstraint.cpp
//  uiGraphTest
//
//  Created by zebra on 03.11.15.
//
//

#include <stdio.h>
#include "UIConstraint.h"

std::map<UIObject*, UIConstraint2D> ConstraintSolver::positionConstraints;
std::map<UIObject*, UIConstraint2D> ConstraintSolver::sizeConstraints;