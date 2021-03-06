/*
	*** Atom location
	*** src/base/AtomAddress.h
	Copyright T. Youngs 2007-2018

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

#ifndef ATEN_ATOMADDRESS_H
#define ATEN_ATOMADDRESS_H

#include "templates/list.h"
#include "base/namespace.h"

ATEN_BEGIN_NAMESPACE

// Forward Declarations (Aten)
class Pattern;

// Atom Address
class AtomAddress : public ListItem<AtomAddress>
{
	public:
	// Constructor / Destructor
	AtomAddress();
	~AtomAddress();


	/*
	 * Address Information
	 */
	private:
	// Molecule in which the atom exists
	int molecule_;
	// Local (molecule) atom offset
	int offset_;
	// Pattern node in which the atom lies (must be set by model before site is used)
	Pattern* pattern_;

	public:
	// Set the local molecule offset of the atom
	void setOffset(int i);
	// Returns the local molecule offset of the atom
	int getOffset();
	// Set the molecule id of the atom
	void setMolecule(int i);
	// Returns the molecule the atom is in
	int molecule();
	// Set the pattern pointer for the atom
	void setPattern(Pattern* p);
	// Returns the current pattern for the atom
	Pattern* pattern();
};

ATEN_END_NAMESPACE

#endif
