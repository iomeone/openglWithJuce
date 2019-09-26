/*
  ==============================================================================

    Tutorial2.h
    Created: 26 Sep 2019 11:36:18am
    Author:  user

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"



class Tutorial2 : public OpenGLAppComponent
{
public:
	//==============================================================================
	Tutorial2()
	{
		setSize(800, 600);
	}
	~Tutorial2()
	{

		shutdownOpenGL();
	}

	//==============================================================================
	void initialise() override
	{

	}
	void shutdown() override
	{
	
	}
	void render() override
	{
		OpenGLHelpers::clear(Colours::blueviolet);

	
	}

	//==============================================================================
	void paint(Graphics& g) override
	{
	
	}
	void resized() override
	{
		
	}

private:
	//==============================================================================
	// Your private member variables go here...


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tutorial2)
};