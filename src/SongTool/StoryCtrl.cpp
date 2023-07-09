#include "SongTool.h"


StoryCtrl::StoryCtrl() {
	CtrlLayout(*this);
	
	meaning <<= THISBACK(SaveStory);
	devices <<= THISBACK(SaveStory);
	emotion <<= THISBACK(SaveStory);
	references <<= THISBACK(SaveStory);
	structure <<= THISBACK(SaveStory);
	history <<= THISBACK(SaveStory);
	storyline <<= THISBACK(SaveStory);
	implications <<= THISBACK(SaveStory);
	
}

void StoryCtrl::Data() {
	Database& db = Database::Single();
	if (!db.active_song)
		return;
	
	Story& o = db.active_song->story;
	
	this->meaning						.SetData(o.meaning);
	this->devices						.SetData(o.devices);
	this->emotion						.SetData(o.emotion);
	this->references					.SetData(o.references);
	this->structure						.SetData(o.structure);
	this->history						.SetData(o.history);
	this->storyline						.SetData(o.storyline);
	this->implications					.SetData(o.implications);
}

void StoryCtrl::SaveStory() {
	Database& db = Database::Single();
	if (!db.active_song)
		return;
	
	Story& o = db.active_song->story;
	
	o.meaning						= this->meaning.GetData();
	o.devices						= this->devices.GetData();
	o.emotion						= this->emotion.GetData();
	o.references					= this->references.GetData();
	o.structure						= this->structure.GetData();
	o.history						= this->history.GetData();
	o.storyline						= this->storyline.GetData();
	o.implications					= this->implications.GetData();
}
