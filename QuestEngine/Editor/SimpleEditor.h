#ifndef _SIMPLEEDITOR_H_
#define _SIMPLEEDITOR_H_
#include "../Core/Assets/Texture2D.h"
class SimpleEditor
{
private:
    static SimpleEditor* s_instance; 
    std::vector<Texture2D*> m_availableLUTs;
    int m_selectedLUTIndex = 0;
    int gradingMode = 0;
    Texture2D* m_colorWheelTexture;

private:
    void UpdateLUTList();

public:
    SimpleEditor();
    ~SimpleEditor();

    SimpleEditor(const SimpleEditor&) = delete;
    SimpleEditor& operator=(const SimpleEditor&) = delete;

    static SimpleEditor* Instance();
    static void Destroy();

    void Display();
};

#endif