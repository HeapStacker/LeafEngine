#pragma once

class IdentifiableObject {
protected:
	unsigned int id;
	virtual void setNewId() = 0;
public:
	unsigned int getId() { return id; }
};