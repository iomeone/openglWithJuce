/*
  ==============================================================================

    engine.h
    Created: 26 Sep 2019 10:12:57am
    Author:  user

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"



class Tutorial1 : public OpenGLAppComponent
{
public:
	//==============================================================================
	Tutorial1()
	{
		setSize(800, 600);
	}
	~Tutorial1()
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
		OpenGLHelpers::clear(Colours::red);

	
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


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tutorial1)
};



 


 