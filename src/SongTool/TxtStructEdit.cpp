#include "SongTool.h"



TxtStructEdit::TxtStructEdit() {
	have_third_view = true;
	have_group_bad_better = true;
	main_key = "ENGLISH_UNPACKED_STRUCTURE";
	error_result_key = "ENGLISH_UNPACKED_STRUCTURE_ERRORS";
	main_natural_key = "ENGLISH_UNPACKED_STRUCTURE_TO_NATURAL";
	main_natural_native_key = "ENGLISH_UNPACKED_STRUCTURE_TO_NATURAL_NATIVE";
	audience_evaluation_key = "ENGLISH_UNPACKED_STRUCTURE_AUDIENCE";
}

void TxtStructEdit::Init() {
	EditorCtrl::Init();
	
	int w = 300;
	top_bar.Add(import_reference_struct.HCenterPos(w,-2*w).VSizePos(1,1));
	top_bar.Add(check_errors.HCenterPos(w,-w).VSizePos(1,1));
	top_bar.Add(convert_to_native.HCenterPos(w,0).VSizePos(1,1));
	top_bar.Add(evaluate_audience.HCenterPos(w,w).VSizePos(1,1));
	
	import_reference_struct.SetLabel(t_("Copy the struct of the reference song"));
	import_reference_struct << THISBACK(ImportReferenceStruct);
	
	check_errors.SetLabel(t_("Check for problems in the song structure"));
	check_errors << THISBACK(CheckErrors);
	
	convert_to_native.SetLabel(t_("Convert the structure to a native text"));
	convert_to_native << THISBACK(ConvertToNative);
	
	evaluate_audience.SetLabel(t_("Evaluate the english text with an AI audience"));
	evaluate_audience << THISBACK(EvaluateAudience);
	
}

void TxtStructEdit::ImportReferenceStruct() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist)
		return;
	Song& song = *p.song;
	
	if (!song.data.Get(main_key, "").IsEmpty()) {
		if (!PromptYesNo(DeQtf(t_("Do you want to overwrite existing text? This cannot be undone."))))
			return;
	}
	String txt = song.data.Get("REFERENCE_SONG__ENGLISH_UNPACKED_STRUCTURE", "");
	song.data.GetAdd(main_key) = txt;
	
	Data();
}

void TxtStructEdit::CheckErrors() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_key.IsEmpty())
		return;
	
	p.RealizePipe();
	
	{
		TaskMgr& m = *p.song->pipe;
		m.CheckSongStructureErrors(main_key, error_result_key, THISBACK(PostOnErrorsRecv));
	}
}

void TxtStructEdit::ConvertToNative() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_key.IsEmpty() || main_natural_key.IsEmpty())
		return;
	
	p.RealizePipe();
	
	p.song->data.GetAdd(main_natural_key).Clear();
	p.song->data.GetAdd(main_natural_native_key).Clear();
	
	{
		TaskMgr& m = *p.song->pipe;
		m.ConvertSongStructureToEnglish(main_key, main_natural_key, THISBACK(OnNaturalExportReady));
	}
}

void TxtStructEdit::OnNaturalExportReady() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_natural_key.IsEmpty() || main_natural_native_key.IsEmpty())
		return;
	
	p.RealizePipe();
	
	{
		String orig_lng = GetCurrentLanguageString().Left(5);
		String trans_lng = "EN-US";
		
		TaskMgr& m = *p.song->pipe;
		m.TranslateSongData(trans_lng, main_natural_key, orig_lng, main_natural_native_key, THISBACK(OnNaturalNativeExportReady));
	}
	
	PostCallback(THISBACK(UpdateExportData));
}

void TxtStructEdit::OnNaturalNativeExportReady() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_natural_native_key.IsEmpty())
		return;
	
	PostCallback(THISBACK(UpdateExportData));
}

void TxtStructEdit::UpdateExportData() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_natural_key.IsEmpty() || main_natural_native_key.IsEmpty())
		return;
	
	other.SetData(p.song->data.Get(main_natural_key, ""));
	third.SetData(p.song->data.Get(main_natural_native_key, ""));
}

void TxtStructEdit::EvaluateAudience() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || audience_evaluation_key.IsEmpty() || main_natural_key.IsEmpty())
		return;
	
	audience.Clear();
	audience_data.Clear();
	
	p.RealizePipe();
	
	p.song->data.GetAdd(audience_evaluation_key).Clear();
	
	{
		TaskMgr& m = *p.song->pipe;
		m.EvaluateSongAudience(main_natural_key, audience_evaluation_key, THISBACK(OnAudienceEvaluationReady));
	}
}

void TxtStructEdit::DoMainAction(int i) {
	if (i == 0) ImportReferenceStruct();
	if (i == 1) CheckErrors();
	if (i == 2) ConvertToNative();
	if (i == 3) EvaluateAudience();
}

void TxtStructEdit::OnErrorsRecv() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || error_result_key.IsEmpty())
		return;
	
	String results = p.song->pipe->data.Get(error_result_key, "");
	//LOG(results);
	
	{
		results.Replace("\r","");
		Vector<String> lines = Split(results, "\n", false);
		int i = -1;
		for(int i = 0; i < lines.GetCount(); i++) {
			String& l = lines[i];
			l = TrimBoth(l);
			
			if (l.IsEmpty() && (i == 0 || lines[i-1].IsEmpty())) {
				lines.Remove(i--);
				continue;
			}
			
			// Ensure that there is at least 1 empty line between parts
			if (i > 0 && IsDigit(l[0]) && !lines[i-1].IsEmpty()) {
				lines.Insert(i);
				continue;
			}
		}
		results = Join(lines, "\n", false);
	}
	
	Vector<String> parts = Split(results, "\n\n");
	
	Vector<String> beginnings;
	beginnings << "Lines with ";
	beginnings << "Lines that are ";
	beginnings << "Lines that ";
	beginnings << "Lines ";
	
	Vector<Point> errors;
	
	TextMatchFinder finder(main.GetData());
	
	Array<ProcMsg> msgs;
	for(String& part : parts) {
		int a = -1;
		for (String& b : beginnings) {
			a = part.Find(b);
			if (a >= 0) {
				a += b.GetCount();
				break;
			}
		}
		if (a < 0) {
			LOG("error: unexpected result");
			continue;
		}
		
		int b = part.Find(" (and improvements)");
		if (b < 0) {
			b = part.Find(" in \"Text B\"");
			if (b < 0) {
				LOG("error: unexpected result");
				continue;
			}
		}
		String key = part.Mid(a, b-a);
		LOG(key);
		LOG(part);
		
		bool all_has_lines = true;
		{
			Vector<String> part_lines = Split(part, "\n");
			for(int i = 1; i < part_lines.GetCount(); i++) {
				String& l = part_lines[i];
				if (l.GetCount() && l[0] != '-') {
					all_has_lines = false;
					break;
				}
			}
		}
		
		Vector<String> part_results;
		if (!all_has_lines) {
			part_results = Split(part, "\n-");
			for (String& part_result : part_results) {
				part_result.Replace("\n", " ");
			}
		}
		else {
			part_results = Split(part, "\n");
		}
		if (part_results.IsEmpty())
			continue;
		part_results.Remove(0);
		//DUMPC(part_results);
		
		for (String& part_result : part_results) {
		
			if (part_result.Left(1) == "-")
				part_result = TrimBoth(part_result.Mid(1)); // remove "-"
			
			String bad, better;
			int a = part_result.Find("\"");
			int b = part_result.Find("\" -> \"");
			if (a < 0) {
				//
			}
			else if (b >= 0) {
				bad = TrimBoth(part_result.Mid(a, b+1-a));
				bad = bad.Mid(1, bad.GetCount()-2);
				better = TrimBoth(part_result.Mid(b+5));
				better = better.Mid(1, better.GetCount()-2);
				
				ProcMsg& err = msgs.Add();
				err.severity = PROCMSG_ERROR;
				err.parts[0] = key;
				err.parts[1] = bad;
				err.parts[2] = better;
				
				Point pt;
				if (finder.Find(bad, pt)) {
					errors << pt;
					err.line = pt.y+1;
					err.col = pt.x+1;
				}
				
			}
			else {
				bad = TrimBoth(part_result.Mid(a));
				bad = bad.Mid(1, bad.GetCount()-2);
				
				ProcMsg& err = msgs.Add();
				err.severity = PROCMSG_ERROR;
				err.parts[0] = key;
				err.parts[1] = bad;
				err.parts[2] = "";
				
				Point pt;
				if (finder.Find(bad, pt)) {
					errors << pt;
					err.line = pt.y+1;
					err.col = pt.x+1;
				}
				
			}
		}
	}
	
	this->error.Clear();
	Sort(msgs, ProcMsg());
	for (ProcMsg& m : msgs)
		OnMessage(m);
	
	main.Errors(std::move(errors));
	
	
	SetBottom(BERRORS);
}

void TxtStructEdit::OnAudienceEvaluationReady() {
	audience_data.Clear();
	
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || audience_evaluation_key.IsEmpty())
		return;
	
	String results = p.song->data.Get(audience_evaluation_key, "");
	//LOG(results);
	
	{
		results.Replace("\r","");
		Vector<String> lines = Split(results, "\n", false);
		int i = -1;
		for(int i = 0; i < lines.GetCount(); i++) {
			String& l = lines[i];
			l = TrimBoth(l);
			
			if (l.IsEmpty() && (i == 0 || lines[i-1].IsEmpty())) {
				lines.Remove(i--);
				continue;
			}
			
			// Ensure that there is at least 1 empty line between parts
			if (i > 0 && IsDigit(l[0]) && !lines[i-1].IsEmpty()) {
				lines.Insert(i);
				continue;
			}
		}
		results = Join(lines, "\n", false);
	}
	
	Time now = GetSysTime();
	
	Vector<String> parts = Split(results, "\n\n");
	
	for (String& part : parts) {
		Vector<String> lines = Split(part, "\n");
		
		AudiencePerson& person = audience_data.Add();
		
		if (lines.GetCount() == 2) {
			String header = lines[0];
			String output = lines[1];
			
			{
				int a, b;
				a = header.Find("What ");
				if (a < 0) continue;
				a += 5;
				b = header.Find("(", a);
				if (b < 0) continue;
				person.name = header.Mid(a, b-a);
				
				a=b;
				a = header.Find("born ", a);
				if (a < 0) continue;
				a += 5;
				b = header.Find(",", a);
				if (b < 0) continue;
				person.born = StrInt(header.Mid(a, b-a));
				person.age = now.year - person.born;
				
				a=b;
				a = header.Find("likes ", a);
				if (a < 0) continue;
				a += 6;
				b = header.Find(")", a);
				if (b < 0) continue;
				person.likes = header.Mid(a, b-a);
			}
			
			{
				if (output.Left(1) == "-") output = TrimBoth(output.Mid(1));
				
				int punct = 0;
				WString line = output.ToWString();
				
				for(int i = 1; i < line.GetCount(); i++) {
					int chr = line[i];
					if (chr == '!' || chr == '.' || chr == '?') {
						punct = i+1;
						break;
					}
				}
				
				person.reaction = line.Left(punct).ToString();
				person.comment = TrimBoth(line.Mid(punct).ToString());
			}
		}
	}
	
	PostCallback(THISBACK(OnAudienceDataReady));
}

void TxtStructEdit::OnAudienceDataReady() {
	for(int i = 0; i < audience_data.GetCount(); i++) {
		AudiencePerson& person = audience_data[i];
		
		audience.Set(i, 0, person.name);
		audience.Set(i, 1, Format(t_("%d (age %d)"), person.born, person.age));
		audience.Set(i, 2, person.likes);
		audience.Set(i, 3, person.reaction);
		audience.Set(i, 4, person.comment);
	}
	audience.SetCount(audience_data.GetCount());
	
	SetBottom(BAUDIENCE);
}
