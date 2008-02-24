/*
	*** Model trajectory functions
	*** src/model/trajectory.cpp
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

#include "base/master.h"
#include "parse/filter.h"
#include "gui/gui.h"

// Clear trajectory
void model::clear_trajectory()
{
	// Clear frames - can simply delete the master config pointed to by 'frames_head'
	dbg_begin(DM_CALLS,"model::clear_trajectory");
	frames.clear();
	if (trajfile != NULL)
	{
		trajfile->close();
		delete trajfile;
	}
	trajfilename = "Unnamed";
	ncachedframes = 0;
	totalframes = 0;
	frameposition = 0;
	trajcached = FALSE;
	dbg_end(DM_CALLS,"model::clear_trajectory");
}

// Initialise trajectory
bool model::initialise_trajectory(const char *fname, filter *f)
{
	// Associate the supplied trajectory file with the model
	dbg_begin(DM_CALLS,"model::initialise_trajectory");
	bool success;
	// Delete old frames and unset old file
	if (trajfile != NULL) trajfile->close();
	clear_trajectory();
	// Check that we can open the specified file
	trajfile = new ifstream(fname,ios::in);
	if (!trajfile->good())
	{
		msg(DM_NONE,"trajectory::initialise - File '%s' couldn't be opened.\n",fname);
		trajfile->close();
		trajfile = NULL;
		trajfilefilter = NULL;
		dbg_end(DM_CALLS,"model::initialise_trajectory");
		return FALSE;
	}
	// Associate the file with the trajectory
	trajfilename = fname;
	trajfilefilter = f;
	// Read header
	if (!trajfilefilter->execute("",trajfile,TRUE,this))
	{
		msg(DM_NONE,"Error reading header of trajectory file.\n");
		trajfile->close();
		trajfile = NULL;
		trajfilefilter = NULL;
		dbg_end(DM_CALLS,"model::initialise_trajectory");
		return FALSE;
	}
	// Store this file position, since it should represent the start of the frame data
	trajposfirst = trajfile->tellg();
	// Determine frame size and number of frames in file
	//printf("Testing frame read...\n");
	model testframe;
	testframe.set_trajparent(this);
	//printf("Initialised config\n");
	if (!trajfilefilter->execute("",trajfile,FALSE,&testframe))
	{
		msg(DM_NONE,"Error testing frame read from trajectory.\n");
		trajfile->close();
		trajfile = NULL;
		trajfilefilter = NULL;
		dbg_end(DM_CALLS,"model::initialise_trajectory");
		return FALSE;
	}
	streampos endofframe = trajfile->tellg();
	framesize = endofframe - trajposfirst;
	msg(DM_NONE,"Single frame is %i kb.\n",framesize/1024);
	trajfile->seekg(0,ios::end);
	streampos endoffile = trajfile->tellg();
	totalframes = (endoffile - trajposfirst) / framesize;
	trajposlast = trajposfirst + streampos((totalframes - 1) * framesize);
	msg(DM_VERBOSE,"File position of first = %lu, framesize = %i, nframes =%i\n",int(trajposfirst),framesize,totalframes);
	trajfile->seekg(trajposfirst);
	// Pre-Cache frame(s)
	msg(DM_NONE,"Successfully associated trajectory.\n"); 
	msg(DM_NONE,"Number of frames in file : %i\n",totalframes);
	// If we are caching the trajectory, read in all frames here. Otherwise, just the first
	msg(DM_NONE,"Estimated trajectory size is %li kb, cache limit = %i kb\n",totalframes * framesize/1024, prefs.get_cache_limit());
	if ((totalframes * framesize)/1024 < prefs.get_cache_limit())
	{
		msg(DM_NONE,"Caching all frames from trajectory...\n");
		// Read all frames from trajectory file
		for (int n=0; n<totalframes; n++)
		{
			model *newframe = add_frame();
			newframe->set_trajparent(this);
			success = trajfilefilter->execute("", trajfile, FALSE, newframe);
			if (success) msg(DM_NONE,"Read frame %i from file.\n",n+1);
			else
			{
				frames.remove(newframe);
				msg(DM_NONE,"Cached %i frames from trajectory before fail.\n", n-1);
				break;
			}
		}
		trajcached = TRUE;	
	}
	else
	{
		//printf("Reading one frame..\n");
		// Read the first frame from the trajectory only
		model *newframe = add_frame();
		newframe->set_trajparent(this);
		if (!trajfilefilter->execute("", trajfile, FALSE, newframe)) 
		{
			frames.remove(newframe);
			msg(DM_NONE,"Error when reading frame data.\n");
			dbg_end(DM_CALLS,"model::initialise_trajectory");
			return FALSE;
		}
	}
	render_from_frames();
	dbg_end(DM_CALLS,"model::initialise_trajectory");
	return TRUE;
}

// Add frame to trajectory
model *model::add_frame()
{
	dbg_begin(DM_CALLS,"model::add_frame");	
	model *newframe = frames.add();
	ncachedframes ++;
	// Set currentframe here (always points to the last added frame)
	currentframe = newframe;
	frameposition = ncachedframes;
	dbg_end(DM_CALLS,"model::add_frame");	
	return newframe;
}

// Delete cached frame from trajectory
void model::remove_frame(model *xframe)
{
	// Delete the specified frame from the trajectory structure
	dbg_begin(DM_CALLS,"model::remove_frame");
	if (xframe == currentframe) currentframe = (xframe->next == NULL ? xframe->prev : xframe->next);
	frames.remove(xframe);
	ncachedframes --;
	dbg_end(DM_CALLS,"trajectory::delete_frame");
}

// Seek to first frame
void model::seek_first_frame()
{
	// Seek to the first frame in the trajectory
	dbg_begin(DM_CALLS,"model::seek_first_frame");
	// Check that a trajectory exists!
	if (totalframes == 0)
	{
		msg(DM_NONE,"No trajectory is available.\n");
		dbg_end(DM_CALLS,"model::seek_first_frame");
		return;
	}
	if (frameposition == 1)
	{
		msg(DM_NONE,"Already at start of trajectory.\n");
		dbg_end(DM_CALLS,"model::seek_first_frame");
		return;
	}
	currentframe = frames.first();
	if (!trajcached)
	{
		// Seek to position of first frame in file
		trajfile->seekg(trajposfirst);
		bool success = trajfilefilter->execute("", trajfile, FALSE, frames.first());
	}
	frameposition = 1;
	log_change(LOG_VISUAL);
	msg(DM_NONE,"Seek to frame %i\n",frameposition);
	dbg_end(DM_CALLS,"model::seek_first_frame");
}

// Seek to next frame
void model::seek_next_frame()
{
	// Seek to the next frame in the trajectory
	dbg_begin(DM_CALLS,"model::seek_next_frame");
	// Check that a trajectory exists!
	if (totalframes == 0)
	{
		msg(DM_NONE,"No trajectory is available.\n");
		dbg_end(DM_CALLS,"model::seek_next_frame");
		return;
	}
	bool success;
	if (frameposition == totalframes)
	{
		msg(DM_NONE,"Already at end of trajectory (frame %i).\n",frameposition);
		dbg_end(DM_CALLS,"model::seek_next_frame");
		return;
	}
	if (trajcached) currentframe = currentframe->next;
	else success = trajfilefilter->execute("", trajfile, FALSE, frames.first());
	frameposition ++;
	log_change(LOG_VISUAL);
	msg(DM_NONE,"Seek to frame %i\n",frameposition);
	dbg_end(DM_CALLS,"model::seek_next_frame");
}

// Seek to previous frame
void model::seek_previous_frame()
{
	// Seek to the previous frame in the trajectory
	dbg_begin(DM_CALLS,"model::seek_previous_frame");
	// Check that a trajectory exists!
	if (totalframes == 0)
	{
		msg(DM_NONE,"No trajectory is available.\n");
		dbg_end(DM_CALLS,"model::seek_previous_frame");
		return;
	}
	if (frameposition == 1)
	{
		msg(DM_NONE,"Already at start of trajectory.\n");
		dbg_end(DM_CALLS,"model::seek_previous_frame");
		return;
	}
	if (trajcached) currentframe = currentframe->prev;
	else
	{
		// Read in previous frame from file
		streampos newpos = trajposfirst + streampos((frameposition-2)*framesize);
		trajfile->seekg(newpos);
		bool success = trajfilefilter->execute("", trajfile, FALSE, frames.first());
	}
	frameposition --;
	log_change(LOG_VISUAL);
	msg(DM_NONE,"Seek to frame %i\n",frameposition);
	dbg_end(DM_CALLS,"model::seek_previous_frame");
}

// Seek to last frame
void model::seek_last_frame()
{
	// Seek to the last frame in the trajectory
	dbg_begin(DM_CALLS,"model::seek_last_frame");
	// Check that a trajectory exists!
	if (totalframes == 0)
	{
		msg(DM_NONE,"No trajectory is available.\n");
		dbg_end(DM_CALLS,"model::seek_last_frame");
		return;
	}
	if (trajcached) currentframe = frames.last();
	else
	{
		// Read in last frame from file
		trajfile->seekg(trajposlast);
		bool success = trajfilefilter->execute("", trajfile, FALSE, frames.first());
	}
	frameposition = totalframes;
	log_change(LOG_VISUAL);
	msg(DM_NONE,"Seek to frame %i\n",frameposition);
	dbg_end(DM_CALLS,"model::seek_last_frame");
}
