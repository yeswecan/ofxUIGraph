
#ifndef UIKeyboardEventReciever_h
#define UIKeyboardEventReciever_h

#include "UIObject.h"

class UIKeyboardEventReciever {
public:
    UIKeyboardEventReciever() {}
    
    virtual void keyPressed(int key) {};
    virtual void keyReleased(int key) {};
    virtual void lostFocus() {};
    
    // Static part
    static void broadcastKeyPressed(int key) {
        if (focusedObject != NULL) focusedObject->keyPressed(key);
    }
    
    static void broadcastKeyReleased(int key) {
        if (focusedObject != NULL) focusedObject->keyReleased(key);
    }
    

    static void initializeKeyboardRecievers() {
        focusedObject = NULL;
    };
    
    static UIKeyboardEventReciever *focusedObject;
    
    static void focusOnObject(UIKeyboardEventReciever *o) {
        if (focusedObject != NULL) focusedObject->lostFocus();
        focusedObject = o;
    }
    
    static void unfocusKeyboardRecievers() {
        if (focusedObject != NULL) focusedObject->lostFocus();
        focusedObject = NULL;
    }
    
    UIKeyboardEventReciever* getFocusedObject() {
        return focusedObject;
    }
};

#endif /* UIKeyboardEventReciever_h */
