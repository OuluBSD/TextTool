#include "SimplifiedCtrl.h"


VocabularyCtrl::VocabularyCtrl() {
	Add(vsplit.SizePos());
	vsplit.Vert() << list << examples;
	
	list.AddColumn(t_("#"));
	list.AddColumn(t_("Example phrase"));
	list.AddColumn(t_("Artist's native phrase"));
	list.AddColumn(t_("Translated English artist's phrase"));
	list.ColumnWidths("1 6 6 6");
	list.WhenCursor = THISBACK(OnPhraseList);
	
	examples.AddColumn(t_("Artist"));
	examples.AddColumn(t_("Country"));
	examples.AddColumn(t_("Phrase"));
	examples.ColumnWidths("2 1 6");
	
	
}

void VocabularyCtrl::DisableAll() {
	list.Disable();
}

void VocabularyCtrl::EnableAll() {
	list.Enable();
}

void VocabularyCtrl::Data() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.artist) return;
	
	for(int i = 0; i < SIGNIFICANT_PHRASE_COUNT; i++) {
		list.Set(i, 0, i);
		list.Set(i, 1, SignificantPhrases[i]);
		list.Set(i, 2, Value());
		list.Set(i, 3, Value());
		
		EditString& ed = list.CreateCtrl<UPP::EditString>(i, 2);
		int j = p.artist->phrases_nat.Find(SignificantPhrases[i]);
		if (j >= 0) {
			ed.SetData(p.artist->phrases_nat[j]);
			//list.Set(i, 2, p.artist->phrases[j]);
		}
		ed.WhenAction = [&p,&ed,i]() {
			p.artist->phrases_nat.GetAdd(SignificantPhrases[i]) = ed.GetData();
		};
		
		j = p.artist->phrases_eng.Find(SignificantPhrases[i]);
		if (j >= 0)
			list.Set(i, 3, p.artist->phrases_eng[j]);
	}
}

void VocabularyCtrl::ToolMenu(Bar& bar) {
	Database& db = Database::Single();
	//bar.Add(t_("Get words"), AppImg::BlueRing(), THISBACK(GetWords)).Key(K_CTRL_Q);
	//bar.Add(t_("Get example phrases"), AppImg::BlueRing(), THISBACK(GetExamplePhrases)).Key(K_CTRL_W);
	//bar.Add(t_("Translate example phrases"), AppImg::BlueRing(), THISBACK(TranslateExamplePhrases)).Key(K_CTRL_E);
	bar.Add(t_("Translate artist's phrase"), AppImg::BlueRing(), THISBACK(TranslateArtistPhrases)).Key(K_CTRL_Q);
	bar.Add(t_("Copy example phrase"), AppImg::BlueRing(), THISBACK(CopyExamplePhrase)).Key(K_CTRL_W);
	bar.Add(t_("Clear artist's phrase"), AppImg::BlueRing(), THISBACK(ClearArtistPhrases)).Key(K_CTRL_E);
	bar.Separator();
	bar.Add(dl, 200);
	
	dl.Clear();
	for(int i = 0; i < db.artists.GetCount(); i++) {
		dl.Add(db.artists[i].native_name);
	}
	bar.Add(t_("Copy artist's phrases"), AppImg::RedRing(), THISBACK(CopyArtistPhrase)).Key(K_CTRL_R);
}

void VocabularyCtrl::GetWords() {
	
}

void VocabularyCtrl::GetExamplePhrases() {
	
}

void VocabularyCtrl::TranslateExamplePhrases() {
	
}

void VocabularyCtrl::TranslateArtistPhrases() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.artist || !p.song || !list.IsCursor()) {
		return;
	}
	Artist& a = *p.artist;
	int list_i = list.GetCursor();
	String eng_phrase = SignificantPhrases[list_i];
	String nat_phrase = a.phrases_nat.Get(eng_phrase, "");
	
	if (nat_phrase.IsEmpty())
		return;
	
	Song& s = *p.song;
	
	s.RealizePipe();
	
	{
		String orig_lng = GetCurrentLanguageString().Left(5);
		String trans_lng = "EN-US";
		TaskMgr& m = *s.pipe;
		m.Translate(orig_lng, nat_phrase, trans_lng, [this, &a, list_i](String result) {PostCallback(THISBACK3(OnTranslateResult, result, &a, list_i));});
	}
}

void VocabularyCtrl::OnTranslateResult(String result, Artist* a, int list_i) {
	result = TrimBoth(result);
	
	String eng_phrase = SignificantPhrases[list_i];
	a->phrases_eng.GetAdd(eng_phrase) = result;
	
	list.Set(list_i, 3, result);
}

void VocabularyCtrl::ClearArtistPhrases() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.artist || !p.song || !list.IsCursor()) {
		return;
	}
	Artist& a = *p.artist;
	int list_i = list.GetCursor();
	String eng_phrase = SignificantPhrases[list_i];
	a.phrases_nat.RemoveKey(eng_phrase);
	a.phrases_eng.RemoveKey(eng_phrase);
	
	list.Set(list_i, 2, Value());
	list.Set(list_i, 3, Value());
}

void VocabularyCtrl::CopyExamplePhrase() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.artist || !p.song || !list.IsCursor() || !examples.IsCursor()) {
		return;
	}
	Artist& a = *p.artist;
	int list_i = list.GetCursor();
	int examples_i = examples.GetCursor();
	String eng_phrase = SignificantPhrases[list_i];
	String user_phrase = examples.Get(examples_i, 2);
	a.phrases_eng.GetAdd(eng_phrase) = user_phrase;
	
	list.Set(list_i, 3, user_phrase);
}

void VocabularyCtrl::CopyArtistPhrase() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.artist) {
		return;
	}
	Artist& a = *p.artist;
	
	int idx = dl.GetIndex();
	if (idx < 0)
		return;
	
	const Artist& src_a = db.artists[idx];
	if (&src_a == &a)
		return;
	
	a.phrases_eng <<= src_a.phrases_eng;
	a.phrases_nat <<= src_a.phrases_nat;
	
	PostCallback(THISBACK(Data));
}

void VocabularyCtrl::OnPhraseList() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.artist || !p.song || !list.IsCursor()) {
		return;
	}
	Artist& a = *p.artist;
	int list_i = list.GetCursor();
	
	VocabularyArgs args;
	args.fn = 0;
	args.phrase = SignificantPhrases[list_i];
	
	Song& s = *p.song;
	
	s.RealizePipe();
	
	{
		TaskMgr& m = *s.pipe;
		m.GetVocabulary(args,  [this, &a, list_i](String result) {PostCallback(THISBACK3(OnVocabularyVariations, result, &a, list_i));});
	}
}

void VocabularyCtrl::OnVocabularyVariations(String result, Artist* a, int list_i) {
	result = TrimBoth(result);
	result.Replace("\r", "");
	
	String artist, country, line;
	Vector<String> lines = Split(result, "\n");
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		if (l.Left(1) == "-")
			l = TrimBoth(l.Mid(1));
		if (l.GetCount() && IsDigit(l[0])) {
			int a = l.Find(".");
			if (a < 5)
				l = TrimBoth(l.Mid(a+1));
		}
			
		
		int a = l.Find(":");
		if (a >= 0) {
			String begin = TrimBoth(l.Left(a));
			String end = TrimBoth(l.Mid(a+1));
			a = begin.Find("(");
			if (a >= 0) {
				int b = begin.Find(")");
				if (b >= 0) {
					country = begin.Mid(a+1, b-a-1);
					artist = TrimBoth(begin.Left(a));
					line = end;
				}
			}
			else {
				artist = begin;
				line = end;
			}
		}
		else {
			a = l.ReverseFind("-");
			if (a >= 0) {
				String begin = TrimBoth(l.Left(a));
				String end = TrimBoth(l.Mid(a+1));
				if (end.Left(1) == ">") end = TrimBoth(end.Mid(1));
				a = begin.Find("(");
				if (a >= 0) {
					int b = begin.Find(")");
					if (b >= 0) {
						country = begin.Mid(a+1, b-a-1);
						artist = TrimBoth(begin.Left(a));
						line = end;
					}
				}
				else {
					artist = begin;
					line = end;
				}
			}
			else {
				int a0 = l.ReverseFind(" by ");
				int a1 = l.Find("\"");
				int a2 = l.ReverseFind("\"");
				// e.g. "Rompere le catene" by Eros Ramazzotti (Italy)
				if (a0 >= 0 && a1 >= 0 && a2 >= 0 && a1 < a2 && a2 < a0) {
					line = l.Left(a0);
					String end = TrimBoth(l.Mid(a0+4));
					a = end.Find("(");
					if (a >= 0) {
						int b = end.Find(")");
						if (b >= 0) {
							country = end.Mid(a+1, b-a-1);
							artist = TrimBoth(end.Left(a));
						}
					}
					else {
						artist = end;
					}
				}
				// e.g. Sigrid (Norway) "hunting for a purpose"
				else if (a0 < 0 && a1 >= 0 && a2 >= 0 && a1 < a2) {
					a = a1;
					String begin = TrimBoth(l.Left(a));
					String end = TrimBoth(l.Mid(a+1));
					a = begin.Find("(");
					if (a >= 0) {
						int b = begin.Find(")");
						if (b >= 0) {
							country = begin.Mid(a+1, b-a-1);
							artist = TrimBoth(begin.Left(a));
							line = end;
						}
					}
					else {
						artist = begin;
						line = end;
					}
				}
				else {
					
				}
			}
		}
		
		RemoveQuotes(line);
		examples.Set(i, 0, artist);
		examples.Set(i, 1, country);
		examples.Set(i, 2, line);
	}
	examples.SetCount(lines.GetCount());
	
	/*String eng_phrase = SignificantPhrases[list_i];
	a->phrases_eng.GetAdd(eng_phrase) = result;
	
	GuiLock __;
	list.Set(list_i, 3, result);*/
}

