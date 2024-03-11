/*
  ==============================================================================

    PalmutesLookAndFeel.h
    Created: 10 Mar 2024 8:57:12pm
    Author:  USER

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
using namespace juce;

class PalmutesLookAndFeel : public LookAndFeel_V4 
{
public:
    Font getLabelFont(Label& label) override;
    void drawToggleButton(Graphics& g, ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
private:
    Font getSpaceGroteskBold();
};