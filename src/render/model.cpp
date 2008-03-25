/*
	*** Model parts rendering
	*** src/render/model.cpp
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

#include "model/model.h"
#include "classes/forcefield.h"
#include "base/elements.h"
#include "gui/canvas.h"

// Render atom labels
void Canvas::renderModelLabels()
{
	dbgBegin(DM_CALLS,"Canvas::renderModelLabels");
	// Annotate the model with 2D labels
	static char text[64];
	static Atom *i;
	static int labels;
	static ForcefieldAtom *ffa;
	static Vec3<double> cellCentre;
	// If we have a unit cell we must account for the origin translation
	cellCentre = displayModel_->cell()->centre();
	for (Atom *i = displayModel_->atoms(); i != NULL; i = i->next)
	{
		// Check if atom has labels
		if (!i->hasLabels()) continue;
		labels = i->labels();
		ffa = i->type();
		// Blank label string
		text[0] = '\0';
		// Now add on all parts of the label that are required
		if (labels&Atom::IdLabel)
		{
			strcat(text,itoa(i->id()+1));
			strcat(text," ");
		}
		if (labels&Atom::ElementLabel)
		{
			strcat(text,elements.symbol(i));
			strcat(text," ");
		}
		if (labels&Atom::TypeLabel)
		{
			strcat(text,"[");
			if (ffa == NULL) strcat(text, "None");
			else
			{
				strcat(text,itoa(ffa->typeId()));
				strcat(text," ");
				strcat(text,ffa->name());
			}
			strcat(text,"] ");
		}
		if (labels&Atom::EquivLabel)
		{ 
			strcat(text,"[=");
			strcat(text,(ffa == NULL ? "None" : ffa->equivalent()));
			strcat(text,"] ");
		}
		if (labels&Atom::ChargeLabel)
		{
			strcat(text,"(");
			strcat(text,ftoa(i->charge()));
			strcat(text," e)");
		}
		glText(i->r() - cellCentre, text);
	}
	dbgEnd(DM_CALLS,"Canvas::renderModelLabels");
}

// Render measurements
void Canvas::renderModelMeasurements()
{
	dbgBegin(DM_CALLS,"Canvas::renderModelMeasurements");
	static Vec3<double> ri, rj, rk, rl, labpos, cellCentre;
	static char text[256];
	static Atom **atoms;
	// Grab cell origin to get correct positioning
	cellCentre = -displayModel_->cell()->centre();
	glPushMatrix();
	  glTranslated(cellCentre.x, cellCentre.y, cellCentre.z);
	  // Go through list of measurements
	  for (Measurement *m = displayModel_->measurements(); m != NULL; m = m->next)
	  {
		atoms = m->atoms();
		glBegin(GL_LINE_STRIP);
		  switch (m->type())
		  {
			case (GT_DISTANCE):
				ri = atoms[0]->r();
				rj = atoms[1]->r();
				labpos = (ri + rj) * 0.5;
				glVertex3d(ri.x, ri.y, ri.z);
				glVertex3d(rj.x, rj.y, rj.z);
				sprintf(text,"%f A",m->value());
				break;
			case (GT_ANGLE):
				ri = atoms[0]->r();
				rj = atoms[1]->r();
				rk = atoms[2]->r();
				labpos = rj;
				glVertex3d(ri.x, ri.y, ri.z);
				glVertex3d(rj.x, rj.y, rj.z);
				glVertex3d(rk.x, rk.y, rk.z);
				sprintf(text,"%f Deg",m->value());
				break;
			case (GT_TORSION):
				ri = atoms[0]->r();
				rj = atoms[1]->r();
				rk = atoms[2]->r();
				rl = atoms[3]->r();
				glVertex3d(ri.x, ri.y, ri.z);
				glVertex3d(rj.x, rj.y, rj.z);
				glVertex3d(rk.x, rk.y, rk.z);
				glVertex3d(rl.x, rl.y, rl.z);
				labpos = (rj + rk) * 0.5;
				sprintf(text,"%f Deg",m->value());
				break;
		  }
		glEnd();
		// Draw on label
		glText(labpos, text);
	  }
	glPopMatrix();
	dbgEnd(DM_CALLS,"Canvas::renderModelMeasurements");
}

// Render force arrows
void Canvas::renderModelForceArrows()
{
	dbgBegin(DM_CALLS,"Canvas::renderModelForceArrows");
	for (Atom *i = displayModel_->atoms(); i != NULL; i = i->next)
	{
		// Scale forces to more reasonable values  TODO User scaling
		glArrow(i->r(),i->f() / 30.0);
	}
	dbgEnd(DM_CALLS,"Canvas::renderModelForceArrows");
}

// Render model cell
void Canvas::renderModelCell()
{
	// Draw the unit cell of the model
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, prefs.colour(COL_PEN));
	glColor3fv(prefs.colour(COL_PEN));
	glLineWidth(1.0f);
	static Vec3<double> cellCentre, lengths;
	static Mat4<double> matrix;
	Cell *cell = displayModel_->cell();
	if (cell->type() != CT_NONE)
	{
		// All cell types are transformations of a unit cube.
		// So, multiply modelview matrix by cell axes matrix and draw a unit cube
		//Mat4<double> mat = displayModel_->get_cell()->get_transpose_as_mat4();
		//mat.get_column_major(glmat);
		GLdouble glmat[16];
		displayModel_->cell()->axesForGl(glmat);
		glPushMatrix();
		  glMultMatrixd(glmat);
		  if (prefs.shouldRender(VO_CELL)) glCallList(list_[GLOB_WIREUNITCUBE]);
		  lengths = displayModel_->cell()->lengths();
		  // Render cell axis arrows
		  if (prefs.shouldRender(VO_CELLAXES))
		  {
			glTranslated(-0.5,-0.5,-0.5);
			glScaled(1.0/lengths.x,1.0/lengths.y,1.0/lengths.z);
			glCallList(list_[GLOB_CELLAXES]);
		  }
		glPopMatrix();
		// Here, translate the initial drawing position to be 0,0,0 in cell coordinates
		cellCentre = -cell->centre();
		glTranslated(cellCentre.x,cellCentre.y,cellCentre.z);
	}
}
