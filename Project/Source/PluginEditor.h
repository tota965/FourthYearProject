/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.2.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_6FA3BFA009382F0F__
#define __JUCE_HEADER_6FA3BFA009382F0F__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "PluginProcessor.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class DAWTestAudioProcessorEditor  : public AudioProcessorEditor,
                                     public Timer,
                                     public SliderListener,
                                     public ComboBoxListener
{
public:
    //==============================================================================
    DAWTestAudioProcessorEditor (DAWTestAudioProcessor& ownerFilter);
    ~DAWTestAudioProcessorEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void timerCallback();

	DAWTestAudioProcessor* getProcessor() const {
		return static_cast <DAWTestAudioProcessor*>(getAudioProcessor());
	}
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Slider> sliderVolume;
    ScopedPointer<Label> lblVolume;
    ScopedPointer<ComboBox> comboKey;
    ScopedPointer<Label> lblKey;
    ScopedPointer<ComboBox> comboBeat;
    ScopedPointer<Label> lblBeat;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DAWTestAudioProcessorEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_6FA3BFA009382F0F__
