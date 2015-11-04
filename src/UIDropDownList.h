//
//  UIDropDownList.h
//  uiGraphTest
//
//  Created by zebra on 04.11.15.
//
//

#ifndef UIDropDownList_h
#define UIDropDownList_h

#include "UIObject.h"

class UIDropDownList: public UIObject {
public:
    UIDropDownList() {
        update = [&](UIObject *o) {
            
        };
        
    }
    
    vector<string> *values;
    int selectedValueIndex;
    int rowHeight;
};

#endif /* UIDropDownList_h */
