/*
	*** Select Dock Widget
	*** src/gui/select_funcs.cpp
	Copyright T. Youngs 2007-2015

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

#include "main/aten.h"
#include "gui/mainwindow.h"
#include "gui/select.h"
#include "gui/selectelement.h"
#include "model/model.h"
#include "parser/commandnode.h"

// Constructor
SelectWidget::SelectWidget(AtenWindow& parent, Qt::WindowFlags flags) : QDockWidget(&parent, flags), parent_(parent)
{
	// Private variables
	refreshing_ = FALSE;

	ui.setupUi(this);
}

// Show window
void SelectWidget::showWidget()
{
	show();
	refresh();
}

void SelectWidget::setHistories(QStringList select, QStringList forlist, QStringList netalist)
{
	refreshing_ = TRUE;
	ui.SelectCombo->addItems(select);
	ui.SelectForCombo->addItems(forlist);
	ui.SelectNetaCombo->addItems(netalist);
	ui.SelectCombo->setCurrentIndex(-1);
	ui.SelectForCombo->setCurrentIndex(-1);
	ui.SelectNetaCombo->setCurrentIndex(-1);
	refreshing_ = FALSE;
}

void SelectWidget::on_SelectAllButton_clicked(bool on)
{
	CommandNode::run(Command::SelectAll, "");
	parent_.updateWidgets(AtenWindow::CanvasTarget+AtenWindow::AtomsTarget+AtenWindow::SelectTarget);
}

void SelectWidget::on_SelectNoneButton_clicked(bool on)
{
	CommandNode::run(Command::SelectNone, "");
	parent_.updateWidgets(AtenWindow::CanvasTarget+AtenWindow::AtomsTarget+AtenWindow::SelectTarget);
}

void SelectWidget::on_SelectionExpandButton_clicked(bool on)
{
	CommandNode::run(Command::Expand, "");
	parent_.updateWidgets(AtenWindow::CanvasTarget+AtenWindow::AtomsTarget+AtenWindow::SelectTarget);
}

void SelectWidget::on_SelectionInvertButton_clicked(bool on)
{
	CommandNode::run(Command::Invert, "");
	parent_.updateWidgets(AtenWindow::CanvasTarget+AtenWindow::AtomsTarget+AtenWindow::SelectTarget);
}

void SelectWidget::on_SelectButton_clicked(bool on)
{
	CommandNode::run(Command::Select, "c", qPrintable(ui.SelectCombo->currentText()));
	parent_.updateWidgets(AtenWindow::CanvasTarget+AtenWindow::AtomsTarget+AtenWindow::SelectTarget);
}

void SelectWidget::on_DeselectButton_clicked(bool on)
{
	CommandNode::run(Command::DeSelect, "c", qPrintable(ui.SelectCombo->currentText()));
	parent_.updateWidgets(AtenWindow::CanvasTarget+AtenWindow::AtomsTarget+AtenWindow::SelectTarget);
}

void SelectWidget::on_TypeSelectElementButton_clicked(bool on)
{
	AtenSelectElement elementSelect(this);
	int newel = elementSelect.selectElement();
	if (newel != -1) ui.TypeElementEdit->setText( Elements().symbol(newel) );
}

void SelectWidget::on_SelectCombo_currentIndexChanged(int n)
{
	if (refreshing_) return;
}

void SelectWidget::on_SelectTypeButton_clicked(bool on)
{
	// Make sure we have a valid element
	int el = Elements().find(qPrintable(ui.TypeElementEdit->text()));
	if (el == 0) msg.print("Invalid element '%s'\n", qPrintable(ui.TypeElementEdit->text()));
	else
	{
		CommandNode::run(Command::SelectType, "ic", el, qPrintable(ui.SelectNetaCombo->currentText()));
		parent_.updateWidgets(AtenWindow::CanvasTarget+AtenWindow::AtomsTarget+AtenWindow::SelectTarget);
	}
}

void SelectWidget::on_DeselectTypeButton_clicked(bool on)
{
	// Make sure we have a valid element
	int el = Elements().find(qPrintable(ui.TypeElementEdit->text()));
	if (el == 0) msg.print("Invalid element '%s'\n", qPrintable(ui.TypeElementEdit->text()));
	else
	{
		CommandNode::run(Command::DeSelectType, "ic", el, qPrintable(ui.SelectNetaCombo->currentText()));
		parent_.updateWidgets(AtenWindow::CanvasTarget+AtenWindow::AtomsTarget+AtenWindow::SelectTarget);
	}
}

void SelectWidget::on_SelectForButton_clicked(bool on)
{
	CommandNode::run(Command::SelectType, "c", qPrintable(ui.SelectForCombo->currentText()));
	parent_.updateWidgets(AtenWindow::CanvasTarget+AtenWindow::AtomsTarget+AtenWindow::SelectTarget);
}

void SelectWidget::on_DeselectForButton_clicked(bool on)
{
	CommandNode::run(Command::DeSelectType, "c", qPrintable(ui.SelectForCombo->currentText()));
	parent_.updateWidgets(AtenWindow::CanvasTarget+AtenWindow::AtomsTarget+AtenWindow::SelectTarget);
}

void SelectWidget::refresh()
{
	Model *m = parent_.aten().currentModelOrFrame();

	// Update selection text details
	// First label, total number of selected atoms.
	Dnchar text(-1,"Total selected : %i\n", m->nSelected());
	ui.SelectionLabel1->setText(text.get());
	// Second label contains empirical formula of selection
	m->selectionEmpirical(text, FALSE, TRUE);
	ui.SelectionLabel2->setText(text.get());
}

void SelectWidget::closeEvent(QCloseEvent *event)
{
	event->accept();
}
