#pragma once

#include "ofMain.h"
#include "ofxUIGraph.h"
#include "UIDropDownList.h"
#include "UICanvas.h"

// UIGraph TODO:
// 0) Разобраться с тем, как gesture recognizer получает координаты (offset) и в каком случае события идут ему, а не на коллбэки соответствующего объекта.
//  Все координаты через gesture recognizer и коллбэки должны идти в
//  размерах внутренней координатной плоскости объекта. При этом,
//  одним из аргументов EventArgs будут и экранные координаты.
// 0.5) В EventArgs события апдейта поинтера должны быть все пальцы.
// 1) UIAnimation должен апдейтить все активные анимации в своём цикле.
// 2) В UIAnimation нужны аналогичные Core Animation функции начала и конца записи
//    анимационных целей, которые будут включать триггер для всех Animatable<> так,
//    чтобы изменения в них приводили к изменениям их целей, а не их самих.
// 3) Аргументы констрейнтов должны тоже стать Animatable
// 4) Аналог класса layer из Cocoa Touch для создания виджетов, имеющих не квадратные формы или располагающихся на горизонтально. Функции проверки точки на входимость, матрица размера/поворота, а также касания - вот что потребует модернизации

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);

    ofTrueTypeFont font;
    
    UICanvas ui;
    UIObject *o1, *o2, *o4, *panel;
    
    UIConstraint2D p1, s1, p2, s2;
    
    EditableField *e, *e2;
    
    UIDropDownList *ddl;
    
    vector<string> dropdownOptions;
    
    ofParameter<string> pp;
};
