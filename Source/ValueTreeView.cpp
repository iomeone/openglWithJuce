//
// Created by Jim Credland on 21/06/2015.
//

/** Display a tree. */
#include "ValueTreeView.h"

//#include "PacketProperty.h"
//extern bool SendDataToGame(int iSendCode, PBYTE szBuffer, DWORD dwSendSize);
class ValueTreeDebuggerMain :
    public Component
{
public:
    class PropertyEditor :
        public PropertyPanel
    {
    public:
        PropertyEditor()
        {
            noEditValue = "not editable";
        }

        void setSource (ValueTree& newSource)
        {
            clear();

            tree = newSource;

            const int maxChars = 200;

            Array<PropertyComponent*> pc;

            for (int i = 0; i < tree.getNumProperties(); ++i)
            {
                const Identifier name = tree.getPropertyName (i).toString();
                Value v = tree.getPropertyAsValue (name, nullptr);
                TextPropertyComponent* tpc;

				if (name.toString() != "MemoryDump")
				{
					if (v.getValue().isObject())
					{
						tpc = new TextPropertyComponent(noEditValue, name.toString(), maxChars, false);
						tpc->setEnabled(false);
					}
					else
					{
						tpc = new TextPropertyComponent(v, name.toString(), maxChars, false);
					}

					pc.add(tpc);
				}

            }
				
			const var*   binaryData = tree.getPropertyPointer("MemoryDump");

			 if (binaryData)
			 {
				
				 if (binaryData->isBinaryData())
				 {
					 MemoryBlock* mb = binaryData->getBinaryData();

					 if (mb)
					 {
						 if (mb->getSize() > 8)
						 {
							 //PacketProperty* l = new PacketProperty("Packet", *mb);
							 //pc.add(l);
						 }
					 }
				 }
			 }


		
	



            addProperties (pc);
        }

    private:
        Value noEditValue;
        ValueTree tree;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PropertyEditor);
    };

    class Item :
        public TreeViewItem,
        public ValueTree::Listener
    {
    public:
        Item (PropertyEditor* propertiesEditor, ValueTree tree)
            :
            propertiesEditor (propertiesEditor),
            t (tree)
        {
            t.addListener (this);
        }

        ~Item()
        {
            clearSubItems();
        }

        bool mightContainSubItems()
        {
            return t.getNumChildren() > 0;
        }

        void itemOpennessChanged (bool isNowOpen)
        {
            if (isNowOpen) updateSubItems();
        }

		class sorter
		{
		public:
			const int compareElements(TreeViewItem* x, TreeViewItem* y)
			{
				StringRef xs = (*((Item*)x)).t.getType();
				StringRef ys = (*((Item*)y)).t.getType();
				String xx = xs;
				String yy = ys;

				if (xx > yy)
					return 1;
				else if (xx == yy)
					return 0;
				else if (xx < yy)
					return -1;
			}
		};


        void updateSubItems()
        {
			std::unique_ptr<XmlElement> openness;
			openness.reset(getOpennessState());

            clearSubItems();
            int children = t.getNumChildren();
			sorter s;
			
			String st = t.getType().toString();
			if (st == "Process List" || st == "Packet")
			{
				this->setLinesDrawnForSubItems(true);
			}

			for (int i = 0; i < children; ++i)
			{
				if (st == "Packet")
				{
					addSubItem(new Item(propertiesEditor, t.getChild(i)));
				}
				else
				{
					addSubItemSorted<sorter>(s, new Item(propertiesEditor, t.getChild(i)));
				}
				//
				
			}
              
            if (openness)
                restoreOpennessState (*openness);
        }
		std::wstring StringToWString(const std::string &str)
		{
			std::wstring wstr(str.length(), L' ');
			std::copy(str.begin(), str.end(), wstr.begin());
			return wstr;
		}

		void itemClicked (const MouseEvent& e) override
		{
			auto newtree  = t;
			auto oldtree = t;

			while (newtree.isValid())
			{
				oldtree = newtree;
				newtree = newtree.getParent();
			}
			jassert(oldtree.getType().toString() == "root");
			
			auto _click = oldtree.getChildWithName("_click");

			if (_click.isValid())
			{
				//AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "info", "find _click");

				_click.removeAllChildren(nullptr);

				ValueTree v(t.getType());
				v.copyPropertiesAndChildrenFrom(t, nullptr);

				_click.addChild(v, -1, nullptr);
			}

			return;

			if (e.mods.isRightButtonDown())
			{
				String strData = this->t.getProperty("Data");
				if (strData.length() > 10)
				{
					PopupMenu m;
					m.addItem(1, "Send to Server");
 					m.addSeparator();
					const int result = m.show();

					if (result == 1)
					{
						MemoryBlock mb;
						mb.loadFromHexString(strData);
						auto data = mb.getData();
						//SendDataToGame(3, PBYTE(data), mb.getSize());
					}

				}

				int pid = this->t.getProperty("PID");
				//TInject i(this->t);
				if (pid != 0)
				{
					PopupMenu m;
					m.addItem(1, "Refreshing Module List");
					m.addItem(2, "Inject DLL");
					m.addSeparator();
					const int result = m.show();

					if (result == 1)
					{
						this->t.removeAllChildren(nullptr);
						//i.GetModuleInfo(pid);
					}
					else if (result == 2)
					{
						FileChooser myChooser("Please select the dll you want to inject...",
							File::getCurrentWorkingDirectory(),
							"*.dll"); //"*.dll; *.*""
						if (myChooser.browseForFileToOpen())
						{
							File mooseFile(myChooser.getResult());
							String dllpath = mooseFile.getFullPathName();
							//loadMoose(mooseFile);
							//LoadRemoteDLL()
							//LoadRemoteDLL(pid, StringToWString(dllpath.toStdString()).c_str());
						}
					}

				}

				if (t.getType().toString() == "Packet")
				{

					PopupMenu m;
					m.addItem(1, "Clear");
					m.addSeparator();
					const int result = m.show();
					if (result == 1)
					{
						t.removeAllChildren(nullptr);
					}
				}

				if (t.getType().toString() == "Process List")
				{
					PopupMenu m;
					m.addItem(1, "Refreshing Process List");
					
					m.addSeparator();
					const int result = m.show();

					if (result == 1)
					{
						this->t.removeAllChildren(nullptr);
						//i.GetProcess();
					}
				}
 

				
			}
	/*		m.showMenuAsync(PopupMenu::Options().withTargetComponent(this->getOwnerView()),
				ModalCallbackFunction::forComponent(menuItemChosenCallback, this));*/
		}
		static void menuItemChosenCallback(int result, Item* demoComponent)
		{
		/*	if (demoComponent != nullptr)
				demoComponent->menuItemChosenCallback(result);*/
		}

		void itemDoubleClicked(const MouseEvent& e) override
		{
			//Component * ec =  e.originalComponent;
			//Item* i = dynamic_cast<Item*>(ec);
			//if (i)
			//{
			//int pid = this->t.getProperty("PID");
			////AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "msg", String(pid), "ok");
			//
			//TInject i(this->t);
			//if (pid != 0)
			//{
			//	AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "msg", String("Refreshing Module List"), "ok");
			//	this->t.removeAllChildren(nullptr);
			//	i.GetModuleInfo(pid);
			//}

			//if (t.getType().toString() == "Process List")
			//{
			//	AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "msg", String("Refreshing Process List"), "ok");
			//	this->t.removeAllChildren(nullptr);
			//	i.GetProcess();

			//}

			//}
		}
        void paintItem (Graphics& g, int w, int h)
        {

            Font font;
            Font smallFont (11.0);


            if (isSelected())
                g.fillAll (Colours::white);


            const float padding = 20.0f;

            String typeName = t.getType().toString();

            const float nameWidth = font.getStringWidthFloat (typeName);
            const float propertyX = padding + nameWidth;

            g.setColour (Colours::black);

            g.setFont (font);

            g.drawText (t.getType().toString(), 0, 0, w, h, Justification::left, false);
            g.setColour (Colours::blue);

            String propertySummary;

            for (int i = 0; i < t.getNumProperties(); ++i)
            {
                const Identifier name = t.getPropertyName (i).toString();
                String propertyValue = t.getProperty (name).toString();;
#ifdef JCF_SERIALIZER

                /* Code for handling valuetree serialization classes I use internally.
                 They allow ReferenceCountedObjects that are stored in the tree
                 to save, load and display debug information easily. You won't
                 need this but it makes my life easier to leave it here - Jim. */
                if (t[name].isObject())
                {
                    ReferenceCountedObject* p = t[name].getObject();

                    if (Serializable* s = dynamic_cast<Serializable*> (p))
                        propertyValue = "[[" + s->getDebugInformation() + "]]";
                }

#endif
                propertySummary += " " + name.toString() + "=" + propertyValue;
            }

            g.drawText (propertySummary, propertyX, 0, w - propertyX, h, Justification::left, true);
        }

        void itemSelectionChanged (bool isNowSelected)
        {
            if (isNowSelected)
            {
                t.removeListener (this);
                propertiesEditor->setSource (t);
                t.addListener (this);
            }
        }

        /* Enormous list of ValueTree::Listener options... */
        void valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property)
        {
            if (t != treeWhosePropertyHasChanged) return;

            t.removeListener (this);
            //            if (isSelected())
            //                propertiesEditor->setSource(t);
            repaintItem();
            t.addListener (this);
        }

        void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded)
        {
            if (parentTree == t)
                updateSubItems();

            treeHasChanged();
        }

        void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved, int)
        {
            if (parentTree == t)
                updateSubItems();

            treeHasChanged();
        }
        void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved, int, int)
        {
            if (parentTreeWhoseChildrenHaveMoved == t)
                updateSubItems();

            treeHasChanged();
        }
        void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged)
        {
            treeHasChanged();
        }
        void valueTreeRedirected (ValueTree& treeWhichHasBeenChanged)
        {
            if (treeWhichHasBeenChanged == t)
                updateSubItems();

            treeHasChanged();
        }

        /* Works only if the ValueTree isn't updated between calls to getUniqueName. */
        String getUniqueName() const
        {
            if (!t.getParent().isValid()) return "1";

            return String (t.getParent().indexOf (t));
        }


		virtual void paintHorizontalConnectingLine(Graphics& g, const Line<float>& line) override
		{
			g.setColour(Colours::darkcyan.withAlpha(0.9f));
			g.drawLine(line);
		}

		//=================================================================================================
		virtual void paintVerticalConnectingLine(Graphics& g, const Line<float>& line) override
		{
			g.setColour(Colours::darkorange.withAlpha(0.9f));
			g.drawLine(line);
		}


		virtual void paintOpenCloseButton(Graphics& g, const juce::Rectangle<float>& area, Colour, bool) override
		{
			Path p;
			p.addTriangle(0.0f, 0.0f, 1.0f, isOpen() ? 0.0f : 0.5f, isOpen() ? 0.5f : 0.0f, 1.0f);

			g.setColour(Colours::blue.withAlpha(0.99f));
			g.fillPath(p, p.getTransformToScaleToFit(area.reduced(2, area.getHeight() / 4), true));
		}

	public:
		ValueTree t;
    private:
        PropertyEditor* propertiesEditor;
        
        Array<Identifier> currentProperties;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Item);
    };

    ValueTreeDebuggerMain() :
        layoutResizer (&layout, 1, true)
    {
        layout.setItemLayout (0, -0.1, -0.9, -0.3);
        layout.setItemLayout (1, 5, 5, 5);
        layout.setItemLayout (2, -0.1, -0.9, -0.7);

        setSize (1600, 800);
        addAndMakeVisible (treeView);
        addAndMakeVisible (propertyEditor);
        addAndMakeVisible (layoutResizer);

		treeView.setDefaultOpenness(true);
    }
    ~ValueTreeDebuggerMain()
    {
        treeView.setRootItem (nullptr);
    }

    void resized() override
    {
        Component* comps[] = { &treeView, &layoutResizer, &propertyEditor };
        layout.layOutComponents (comps, 3, 0, 0, getWidth(), getHeight(), false, true);
    }

    void setTree (ValueTree newTree)
    {
        if (!newTree.isValid())
        {
            treeView.setRootItem (nullptr);
        }
        else if (tree != newTree)
        {
            tree = newTree;
            rootItem = new Item (&propertyEditor, tree);
            treeView.setRootItem (rootItem);
			rootItem->setLinesDrawnForSubItems(true);

        }
    }

public:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ValueTreeDebuggerMain);

    ScopedPointer<Item> rootItem;
    ValueTree tree;
    TreeView treeView;
    PropertyEditor propertyEditor;
    StretchableLayoutManager layout;
    StretchableLayoutResizerBar layoutResizer;
};


ValueTreeDebugger::ValueTreeDebugger()
:
    DocumentWindow ("Value Tree Debugger",
                    Colours::lightgrey,
                    DocumentWindow::allButtons)
{
    construct();
}

ValueTreeDebugger::ValueTreeDebugger(ValueTree & tree)
:
    DocumentWindow ("Value Tree Debugger",
                    Colours::lightgrey,
                    DocumentWindow::allButtons)
{
    construct();
    setSource(tree);
}

void ValueTreeDebugger::construct()
{
    main = new ValueTreeDebuggerMain();
    setContentNonOwned (main, true);
    setResizable (true, false);
    setUsingNativeTitleBar (true);
    centreWithSize (getWidth(), getHeight());
    setVisible (true);
}

ValueTreeDebugger::~ValueTreeDebugger()
{
    main->setTree (ValueTree());
}

void ValueTreeDebugger::closeButtonPressed()
{
    setVisible (false);
}

void ValueTreeDebugger::setSource (ValueTree& v)
{
    main->setTree (v);
}



