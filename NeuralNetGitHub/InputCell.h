#pragma once
#include "Cell.h"

class InputCell : Cell {
private:
	
public:
	virtual std::uint64_t transform() const override;


	InputCell() : Cell() {
		slope = -11;
		offset = -11;
	}


};