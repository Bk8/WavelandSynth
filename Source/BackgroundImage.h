/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 4.1.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_68D6B978521B631A__
#define __JUCE_HEADER_68D6B978521B631A__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class BackgroundImage  : public Component
{
public:
    //==============================================================================
    BackgroundImage ();
    ~BackgroundImage();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void resized(int newWidth, int newHeight);
    //[/UserMethods]

    void paint (Graphics& g);
    //void resized();

    // Binary resources:
    static const char* waveLandBackNoTitle_jpg;
    static const int waveLandBackNoTitle_jpgSize;
    static const char* takara_ttf;
    static const int takara_ttfSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    int imageHeight = 600;
    int imageWidth = 400;
    GlowEffect titleGlow;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Label> label;
    Image cachedImage_waveLandBackNoTitle_jpg_1;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BackgroundImage)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_68D6B978521B631A__
