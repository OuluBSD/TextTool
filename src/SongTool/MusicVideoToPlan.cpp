#include "SongTool.h"



MusicVideoToPlan::MusicVideoToPlan() {
	english_screenplay_auto = "ENGLISH_SCREENPLAY_AUTOMATIC";
	
}

void MusicVideoToPlan::Init() {
	ImagePlayerBase::Init();
	
}

void MusicVideoToPlan::ToolMenu(Bar& bar) {
	bar.Add(t_("Import the english screenplay"), AppImg::Part(), THISBACK(ImportScreenplay)).Key(K_F5);
	bar.Add(t_("Make single image"), AppImg::Part(), THISBACK(MakeSingleImage)).Key(K_F6);
	bar.Add(t_("Make all images"), AppImg::Part(), THISBACK(MakeAllImages)).Key(K_F7);
	
}

String MusicVideoToPlan::GetStatusText() {
	return "";
}

void MusicVideoToPlan::ImportScreenplay() {
	if (!items.IsEmpty()) {
		if (!PromptYesNo(DeQtf(t_("Are you sure you want to replace everything in the list?"))))
			return;
	}
	
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || english_screenplay_auto.IsEmpty())
		return;
	
	p.RealizePipe();
	
	String txt = p.song->data.Get(english_screenplay_auto, "");
	txt.Replace("\r", "");
	txt.Replace("\n \n", "\n\n");
	Vector<String> parts = Split(txt, "\n\n");
	
	items.Clear();
	for (const String& part : parts) {
		Vector<String> lines = Split(part, "\n");
		String part_name = TrimBoth(lines[0]);
		if (part_name.Right(1) == ":")
			part_name = part_name.Left(part_name.GetCount()-1);
		
		for(int i = 1; i < lines.GetCount(); i++) {
			const String& line = lines[i];
			
			Item& item = items.Add();
			item.time = -1;
			item.part = part_name;
			item.prompt = lines[i];
			int j = item.prompt.Find(": ");
			if (j >= 0) {
				item.sort_value = item.prompt.Left(j);
				item.prompt = item.prompt.Mid(j+1);
				
				j = item.sort_value.Find(" ");
				if (j >= 0) {
					String time_str = item.sort_value.Left(j);
					Vector<String> time_parts = Split(time_str, ":");
					if (time_parts.GetCount() == 2) {
						int min = StrInt(time_parts[0]);
						int sec = StrInt(time_parts[1]);
						int total_sec = min * 60 + sec;
						item.time = total_sec;
					}
				}
			}
		}
	}
	
	DataList();
}

void MusicVideoToPlan::MakeSingleImage() {
	ImagePlayerBase::MakeSingleImage();
}

void MusicVideoToPlan::MakeAllImages() {
	ImagePlayerBase::MakeAllImages();
}
