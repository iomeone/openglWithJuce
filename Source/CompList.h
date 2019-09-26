/*
  ==============================================================================

    CompList.h
    Created: 26 Sep 2019 11:30:28am
    Author:  user

  ==============================================================================
*/

#pragma once


class TCompType
{
public:
	virtual Component* createComponent() = 0;
	TCompType(const String &n, int index) : name(n) , _index(index)
	{
		getCompTypeList().set(index, (TCompType*)this);
		//getCompTypeList().set(n, (TCompType*)this);

	}


	static HashMap <int, TCompType*>& getCompTypeList()
	{
		static HashMap <int, TCompType*> list;
	
		return list;
	}
	String name;
	int _index;
};



template <class TComponent>
class ComponentList : TCompType
{
public:
	ComponentList(const  String & n, int index) : TCompType(n, index)
	{
	}

	virtual Component* createComponent()
	{
		return new TComponent();
	}
};
