//
//  UIConstraint.cpp
//  uiGraphTest
//
//  Created by zebra on 03.11.15.
//
//

#include <stdio.h>
#include "UIConstraintSolver.h"

std::map<UIShape*, UIConstraint2D> UIConstraintSolver::positionConstraints;
std::map<UIShape*, UIConstraint2D> UIConstraintSolver::sizeConstraints;
