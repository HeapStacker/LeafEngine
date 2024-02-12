#include "GuiSelection.h"

std::vector<GuiSelection*> GuiSelection::selections;
unsigned int GuiSelection::idTracker = 0;

GuiSelection* GuiSelection::FindSelectionById(unsigned int id) {
	for (GuiSelection* selection : selections) {
		if (selection->id == id) return selection;
	}
	return nullptr;
}

void addGuiElementToSelection(GuiElement* element) {
	GuiSelection* selection = GuiSelection::FindSelectionById(GuiSelection::getLastId());
	selection->addGuiElement(element);
}

unsigned int GuiSelection::getLastId()
{
	return GuiSelection::idTracker;
}