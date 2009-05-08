/*
	*** Qt prefs window declaration
	*** src/gui/prefs.h
	Copyright T. Youngs 2007-2009

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

#ifndef ATEN_PREFSWINDOW_H
#define ATEN_PREFSWINDOW_H

#include "gui/gui.h"
#include "gui/ui_prefs.h"
#include "classes/prefs.h"

// Forwads declarations
class Element;

// Program preferences window
class AtenPrefs : public QDialog
{
	// All Qt declarations derived from QObject must include this macro
	Q_OBJECT

	/*
	// Window Functions
	*/
	private slots:
	void on_PrefsCancelButton_clicked(bool checked);
	void on_PrefsSetDefaultButton_clicked(bool checked);

	/*
	// Element Page
	*/
	private slots:
	void on_ElementList_currentRowChanged(int row);
	void on_ElementAmbientColourButton_clicked(bool checked);
	void on_ElementDiffuseColourButton_clicked(bool checked);

	/*
	// Interaction page
	*/
	private slots:
	void on_LeftMouseCombo_currentIndexChanged(int ma);
	void on_MiddleMouseCombo_currentIndexChanged(int ma);
	void on_RightMouseCombo_currentIndexChanged(int ma);
	void on_WheelMouseCombo_currentIndexChanged(int ma);
	void on_ShiftButtonCombo_currentIndexChanged(int ka);
	void on_CtrlButtonCombo_currentIndexChanged(int ka);
	void on_AltButtonCombo_currentIndexChanged(int ka);
	void on_ZoomThrottleSpin_valueChanged(double value);

	/*
	// View Page
	*/
	private:
	void updateAfterViewPrefs();
	void setRadiusChanged(Atom::DrawStyle ds, double value, bool foratom);
	void setVisibleObject(Prefs::ViewObject vo, int state, bool onscreen);
	void spotlightPosChanged(int i, double value);
	void spotlightColourChanged(Prefs::ColourComponent);
	private slots:
	// Radii
	void on_StickRadiusSpin_valueChanged(double value);
	void on_TubeRadiusSpin_valueChanged(double value);
	void on_SphereRadiusSpin_valueChanged(double value);
	void on_ScaledRadiusSpin_valueChanged(double value);
	void on_StickBondRadiusSpin_valueChanged(double value);
	void on_TubeBondRadiusSpin_valueChanged(double value);
	void on_SphereBondRadiusSpin_valueChanged(double value);
	void on_ScaledBondRadiusSpin_valueChanged(double value);
	void on_SelectionScaleSpin_valueChanged(double value);
	// Lighting
	void on_SpotlightGroup_clicked(bool checked);
	void on_SpotlightAmbientColourButton_clicked(bool checked);
	void on_SpotlightDiffuseColourButton_clicked(bool checked);
	void on_SpotlightSpecularColourButton_clicked(bool checked);
	void on_SpotlightPositionXSpin_valueChanged(double value);
	void on_SpotlightPositionYSpin_valueChanged(double value);
	void on_SpotlightPositionZSpin_valueChanged(double value);
	void on_ShininessSpin_valueChanged(int value);
	// Scene
	void on_AtomsVisibleCheck_stateChanged(int state);
	void on_CellVisibleCheck_stateChanged(int state);
	void on_AxesVisibleCheck_stateChanged(int state);
	void on_GlobeVisibleCheck_stateChanged(int state);
	void on_LabelsVisibleCheck_stateChanged(int state);
	void on_MeasurementsVisibleCheck_stateChanged(int state);
	void on_SurfacesVisibleCheck_stateChanged(int state);
	void on_RegionsVisibleCheck_stateChanged(int state);
	void on_AtomsVisibleImageCheck_stateChanged(int state);
	void on_CellVisibleImageCheck_stateChanged(int state);
	void on_AxesVisibleImageCheck_stateChanged(int state);
	void on_GlobeVisibleImageCheck_stateChanged(int state);
	void on_LabelsVisibleImageCheck_stateChanged(int state);
	void on_MeasurementsVisibleImageCheck_stateChanged(int state);
	void on_SurfacesVisibleImageCheck_stateChanged(int state);
	void on_RegionsVisibleImageCheck_stateChanged(int state);
	// GL
	void on_AtomQualitySpin_valueChanged(int value);
	void on_BondQualitySpin_valueChanged(int value);
	void on_FarClipSpin_valueChanged(double value);
	void on_FarDepthSpin_valueChanged(int value);
	void on_NearClipSpin_valueChanged(double value);
	void on_NearDepthSpin_valueChanged(int value);
	void on_LineAliasingCheck_stateChanged(int state);
	void on_PolygonAliasingCheck_stateChanged(int state);

	/*
	// Colours page
	*/
	private:
	void updateScalePointsList();
	private slots:
	void on_ForegroundColourButton_clicked(bool checked);
	void on_BackgroundColourButton_clicked(bool checked);
	void on_SpecularColourButton_clicked(bool checked);
	void on_ScaleList_currentRowChanged(int id);
	void on_ScalePointsList_currentRowChanged(int id);
	void on_ScaleNameEdit_returnPressed();
	void on_PointValueSpin_valueChanged(double d);
	void on_PointColourButton_clicked(bool checked);
	void on_AddPointButton_clicked(bool checked);
	void on_RemovePointButton_clicked(bool checked);
	void on_ScaleList_itemClicked(QListWidgetItem *item);

	/*
	// Local variables
	*/
	private:
	// Whether the window is currently refreshing
	bool refreshing_;
	// Local copy of prefs/elements data
	Prefs prefsBackup_;

	/*
	// Widgets
	*/
	public:
	// Constructor
	AtenPrefs(QWidget *parent = 0);
	// Destructor
	~AtenPrefs();
	// Main form declaration
	Ui::PrefsDialog ui;
	// Finalise widgets (things that we couldn't do in Qt Designer)
	void finaliseUi();
	// Set controls to reflect program variables
	void setControls();
};

#endif
