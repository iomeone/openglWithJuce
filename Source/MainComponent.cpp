/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "Tutorial1.h"
#include "Tutorial2.h"
#include "Tutorial3.h"
#include "Tutorial4.h"
#include "Tutorial5.h"
#include "Tutorial6.h"
#include "Tutorial7.h"
#include "Tutorial8.h"
#include "Tutorial9.h"
#include "Tutorial10.h"
#include "Tutorial11.h"
#include "Tutorial12.h"
#include "Tutorial13.h"
#include "Tutorial14.h"
#include "Tutorial15.h"


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
		_curSubComp.reset( ct->createComponent());
		//_curSubComp->setFocusContainer(true);
		addAndMakeVisible(_curSubComp.get());
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
	if (_lstBox)
	{
		_lstBox->setBounds(r.removeFromLeft(proportionOfWidth(0.3000f)));
	}

	if (_curSubComp)
	{
		_curSubComp->setBounds(r);
	}

}


static ComponentList<T1::Tutorial1> t1((const String)("1: Triangle"), 0);
static ComponentList<T2::Tutorial2> t2((const String)("2: Shader"), 1);
static ComponentList<T3::Tutorial3> t3((const String)("3: Texture"), 2);
static ComponentList<T4::Tutorial4> t4((const String)("4: Transformations"), 3);
static ComponentList<T5::Tutorial5> t5((const String)("5: Coordinate Systems"), 4);
static ComponentList<T6::Tutorial6> t6((const String)("6: Camera"), 5);
static ComponentList<T7::Tutorial7> t7((const String)("7: Colors"), 6);
static ComponentList<T8::Tutorial8> t8((const String)("8: Basic-Lighting"), 7);
static ComponentList<T9::Tutorial9> t9((const String)("9: Materials"), 8);
static ComponentList<T10::Tutorial10> t10((const String)("10: Lighting-maps"), 9);
static ComponentList<T11::Tutorial11> t11((const String)("11: Directional-Light"), 10);
static ComponentList<T12::Tutorial12> t12((const String)("12: Point-Light"), 11);
static ComponentList<T13::Tutorial13> t13((const String)("13: Spot-Light"), 12);
static ComponentList<T14::Tutorial14> t14((const String)("14: Multiple-lights"), 13);
static ComponentList<T15::Tutorial15> t15((const String)("15: Model"), 14);