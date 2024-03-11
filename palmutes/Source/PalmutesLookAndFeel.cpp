#include "PalmutesLookAndFeel.h"
/*
  ==============================================================================

    PalmutesLookAndFeel.cpp
    Created: 10 Mar 2024 8:57:12pm
    Author:  USER

  ==============================================================================
*/

Font PalmutesLookAndFeel::getLabelFont(Label& label)
{
    return getSpaceGroteskBold();
}

// base implementation of orginal drawToggleButton(), but adjusted to use custom font and font size
// https://github.com/juce-framework/JUCE/blob/master/modules/juce_gui_basics/lookandfeel/juce_LookAndFeel_V4.cpp
void PalmutesLookAndFeel::drawToggleButton(Graphics& g, ToggleButton& button, 
                                           bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    auto fontSize = jmin(15.0f, (float)button.getHeight() * 0.75f);
    auto tickWidth = fontSize * 1.1f;

    drawTickBox(g, button, 4.0f, ((float)button.getHeight() - tickWidth) * 0.5f,
        tickWidth, tickWidth,
        button.getToggleState(),
        button.isEnabled(),
        shouldDrawButtonAsHighlighted,
        shouldDrawButtonAsDown);

    g.setColour(button.findColour(ToggleButton::textColourId));
    g.setFont(getSpaceGroteskBold());
    g.setFont(fontSize * 1.2f);

    if (!button.isEnabled())
        g.setOpacity(0.5f);

    g.drawFittedText(button.getButtonText(),
        button.getLocalBounds().withTrimmedLeft(roundToInt(tickWidth) + 10)
        .withTrimmedRight(2),
        Justification::centredLeft, 10);
}

Font PalmutesLookAndFeel::getSpaceGroteskBold()
{
    Typeface::Ptr typeface = Typeface::createSystemTypefaceFor(BinaryData::SpaceGroteskBold_ttf, BinaryData::SpaceGroteskBold_ttfSize);
    ScopedPointer<Font> font = new Font(typeface);
    font->setHeight(18);
    return *font;
}
