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
    Typeface::Ptr typeface = Typeface::createSystemTypefaceFor(BinaryData::SpaceGroteskBold_ttf, BinaryData::SpaceGroteskBold_ttfSize);
    ScopedPointer<Font> font = new Font(typeface);
    font->setHeight(18);
    return *font;
}
