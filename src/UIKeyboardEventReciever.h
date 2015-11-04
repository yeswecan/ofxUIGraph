
#ifndef UIKeyboardEventReciever_h
#define UIKeyboardEventReciever_h

#include "UIObject.h"

class UIKeyboardEventReciever {
public:
    UIKeyboardEventReciever() {}
    
    virtual void keyPressed(int key) {};
    virtual void keyReleased(int key) {};
    
    // Static part
    static void broadcastKeyPressed(int key) {
        if (focusedObject != NULL) focusedObject->keyPressed(key);
    }
    
    static void broadcastKeyReleased(int key) {
        if (focusedObject != NULL) focusedObject->keyReleased(key);
    }
    

    static void initializeEditableFields() {
        focusedObject = NULL;
    };
    
    static UIKeyboardEventReciever *focusedObject;
    
    static void focusOnObject(UIKeyboardEventReciever *o) {
        focusedObject = o;
    }
    
    static void unfocusKeyboardRecievers() {
        focusedObject = NULL;
    }
    
    UIKeyboardEventReciever* getFocusedObject() {
        return focusedObject;
    }
};

#endif /* UIKeyboardEventReciever_h */
