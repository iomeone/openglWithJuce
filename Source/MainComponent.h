/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "engine.h"
#include <vector>
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public Component , public ListBoxModel
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;


	int getNumRows() override
	{

		return   _tutorialList.size();
	}

	void paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected) override
	{
		if (rowIsSelected)
			g.fillAll(Colours::deepskyblue);

		if (rowNumber < _tutorialList.size())
		{
			auto s = _tutorialList[rowNumber];
			AttributedString a;
			a.setJustification(Justification::centredLeft);
			a.append(s, Font(20.0f), Colours::white);
			a.draw(g, Rectangle<int>(width + 10, height).reduced(6, 0).toFloat());
		}

		
	}


	void selectedRowsChanged(int lastRowSelected) override
	{
		if (_curTutorialIndex == lastRowSelected)
		{
			return;
		}
		_curTutorialIndex = lastRowSelected;

		if (lastRowSelected < _tutorialList.size())
		{
			auto s = _tutorialList[lastRowSelected];
			

		}


	
	}


private:
    //==============================================================================
    // Your private member variables go here...
	MainScreen _ms;

	std::unique_ptr<ListBox> _lstBox;
	int _curTutorialIndex{ 0 };
	std::vector<String> _tutorialList{ "tutorial 1", "tutorial 2" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
