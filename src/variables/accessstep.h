/*
	*** Variable Access Step
	*** src/variables/accessstep.h
	Copyright T. Youngs 2007,2008

	This file is part of Aten.

	Aten is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Aten is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Aten.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ATEN_ACCESSSTEP_H
#define ATEN_ACCESSSTEP_H

#include "base/vtypes.h"

// Forward declarations
class AccessPath;
class Variable;
class VariableList;

// Variable Access Step
class AccessStep
{
	public:
	// Constructor
	AccessStep();
	// List pointers
	AccessStep *prev, *next;

	private:
	// Target variable
	Variable *target_;
	// Array index associated to variable (if any)
	AccessPath *arrayIndex_;

	public:
	// Set target variable
	void setTarget(Variable *var);
	// Create arrayindex 'branch'
	bool setArrayIndex(const char *path, VariableList *sourcevars);
	// Get return value as integer
	int asInteger();
	// Get return value as double
	int asDouble();
	// Get return value as bool
	int asBool();
	// Get return value as pointer
	int asPointer(VTypes::DataType dt);
};

#endif