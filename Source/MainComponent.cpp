/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "Tutorial1.h"
#include "Tutorial2.h"
//==============================================================================
MainComponent::MainComponent()
{
	_lstBox.reset(new ListBox("_lstBox"));
	_lstBox->setModel(this);
	addAndMakeVisible(_lstBox.get());

	_lstBox->selectRow(curSubCompIndex);

	HashMap <int, TCompType*>& hm = TCompType::getCompTypeList();
	if (hm.contains(curSubCompIndex))
	{
		TCompType * ct = hm[curSubCompIndex];
		_curSubComp = nullptr;
		_curSubComp = ct->createComponent();
		//_curSubComp->setFocusContainer(true);
		addAndMakeVisible(_curSubComp);
	}

	setSize(1200, 800);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

	auto r = getLocalBounds();
	int listBoxWidth = proportionOfWidth(0.2000f);
	if (_lstBox)
	{
		_lstBox->setBounds(r.removeFromLeft(listBoxWidth));
	}
	if (_curSubComp)
	{
		_curSubComp->setBounds(r);
	}
		
}


static ComponentList<Tutorial1> td1((const String)("Tutorial1"), 0);
static ComponentList<Tutorial2> td2((const String)("Tutorial2"), 1);