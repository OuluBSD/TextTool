#include "TextLib.h"

BEGIN_TEXTLIB_NAMESPACE


CodeSolver::CodeSolver() {
	
}

int CodeSolver::GetPhaseCount() const {
	return PHASE_COUNT;
}

void CodeSolver::DoPhase() {
	TextDatabase& db = GetDatabase();
	
	if (phase == PHASE_GET_PLATFORMS) {
		TaskMgr& m = TaskMgr::Single();
		CodeArgs args;
		args.fn = CodeArgs::GET_PLATFORMS;
		
		if (skip_ready && db.platforms.GetCount()) {
			NextPhase();
			return;
		}
		
		SetWaiting(1);
		m.GetCode(args, [this](String res) {
			TextDatabase& db = GetDatabase();
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			
			db.platforms.SetCount(0);
			for(int i = 0; i < lines.GetCount(); i++) {
				HardPlatform& plat = db.platforms.Add();
				plat.name = TrimBoth(lines[i]);
			}
			
			NextBatch();
			SetWaiting(0);
		});
	}
	else if (phase == PHASE_GET_OPERATING_SYSTEMS) {
		TaskMgr& m = TaskMgr::Single();
		CodeArgs args;
		args.fn = CodeArgs::GET_OPERATING_SYSTEMS;
		
		if (batch >= db.platforms.GetCount()) {
			NextPhase();
			return;
		}
		
		HardPlatform& plat = db.platforms[batch];
		if (skip_ready && plat.os.GetCount()) {
			NextBatch();
			return;
		}
		args.platform = plat.name;
		
		SetWaiting(1);
		m.GetCode(args, [this](String res) {
			TextDatabase& db = GetDatabase();
			HardPlatform& plat = db.platforms[batch];
			
			RemoveEmptyLines3(res);
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			
			plat.os.SetCount(0);
			for(int i = 0; i < lines.GetCount(); i++) {
				String s = TrimBoth(lines[i]);
				String name, desc;
				int a = s.Find(":");
				if (a < 0)
					name = s;
				else {
					name = s.Left(a);
					desc = TrimBoth(s.Mid(a+1));
				}
				int idx = -1;
				SoftPlatform& os = db.GetAddOS(name, &idx);
				if (desc.GetCount() && os.description.IsEmpty())
					os.description = desc;
				
				plat.os << idx;
			}
			
			NextBatch();
			SetWaiting(0);
		});
	}
	else if (phase == PHASE_GET_SW_LANGUAGES) {
		TaskMgr& m = TaskMgr::Single();
		CodeArgs args;
		args.fn = CodeArgs::GET_SW_LANGUAGES;
		
		if (batch >= db.platforms.GetCount()) {
			NextPhase();
			return;
		}
		
		HardPlatform& plat = db.platforms[batch];
		if (skip_ready && plat.langs.GetCount()) {
			NextBatch();
			return;
		}
		
		args.platform = plat.name;
		
		SetWaiting(1);
		m.GetCode(args, [this](String res) {
			TextDatabase& db = GetDatabase();
			HardPlatform& plat = db.platforms[batch];
			
			RemoveEmptyLines3(res);
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			
			plat.langs.SetCount(0);
			for(int i = 0; i < lines.GetCount(); i++) {
				String name = TrimBoth(lines[i]);
				String desc;
				
				int a = name.Find(":");
				if (a >= 0) {
					desc = TrimBoth(name.Mid(a+1));
					name = name.Left(a);
				}
				else {
					a = name.Find("(");
					if (a >= 0) {
						desc = TrimBoth(name.Mid(a+1));
						desc = desc.Left(desc.GetCount()-1);
						name = TrimBoth(name.Left(a));
					}
				}
				
				int lng_i = -1;
				SoftLanguage& soft = db.GetAddLanguage(name, &lng_i);
				
				plat.langs.Add().lng = lng_i;
			}
			
			NextBatch();
			SetWaiting(0);
		});
	}
	else if (phase == PHASE_GET_SW_PARADIGMS) {
		TaskMgr& m = TaskMgr::Single();
		CodeArgs args;
		args.fn = CodeArgs::GET_SW_PARADIGMS;
		
		if (batch >= db.platforms.GetCount()) {
			NextPhase();
			return;
		}
		
		HardPlatform& plat = db.platforms[batch];
		if (skip_ready && plat.paradigms.GetCount()) {
			NextPhase();
			return;
		}
		
		args.platform = plat.name;
		
		SetWaiting(1);
		m.GetCode(args, [this](String res) {
			TextDatabase& db = GetDatabase();
			HardPlatform& plat = db.platforms[batch];
			
			RemoveEmptyLines3(res);
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			
			plat.paradigms.SetCount(0);
			for(int i = 0; i < lines.GetCount(); i++) {
				String name = TrimBoth(lines[i]);
				String desc;
				
				int a = name.Find(":");
				if (a >= 0) {
					desc = TrimBoth(name.Mid(a+1));
					name = name.Left(a);
				}
				else {
					a = name.Find("(");
					if (a >= 0) {
						desc = TrimBoth(name.Mid(a+1));
						desc = desc.Left(desc.GetCount()-1);
						name = TrimBoth(name.Left(a));
					}
				}
				
				int j = -1;
				SoftParadigm& soft = db.GetAddParadigm(name, &j);
				
				plat.paradigms << j;
			}
			
			NextBatch();
			SetWaiting(0);
		});
	}
	else if (phase == PHASE_GET_IDE) {
		TaskMgr& m = TaskMgr::Single();
		CodeArgs args;
		args.fn = CodeArgs::GET_IDE;
		
		if (batch >= db.platforms.GetCount()) {
			NextPhase();
			return;
		}
		
		HardPlatform& plat = db.platforms[batch];
		if (sub_batch >= plat.langs.GetCount()) {
			NextBatch();
			return;
		}
		
		SoftLanguage& lang = db.langs[plat.langs[sub_batch].lng];
		
		args.platform = plat.name;
		args.lang = lang.name;
		
		SetWaiting(1);
		m.GetCode(args, [this](String res) {
			TextDatabase& db = GetDatabase();
			HardPlatform& plat = db.platforms[batch];
			auto& plat_lang = plat.langs[sub_batch];
			plat_lang.ides.Clear();
			
			RemoveEmptyLines3(res);
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			
			for(int i = 0; i < lines.GetCount(); i++) {
				String name = TrimBoth(lines[i]);
				String desc;
				
				int a = name.Find(":");
				if (a >= 0) {
					desc = TrimBoth(name.Mid(a+1));
					name = name.Left(a);
				}
				else {
					a = name.Find("(");
					if (a >= 0) {
						desc = TrimBoth(name.Mid(a+1));
						desc = desc.Left(desc.GetCount()-1);
						name = TrimBoth(name.Left(a));
					}
					else {
						a = name.Find(" - ");
						if (a >= 0) {
							desc = TrimBoth(name.Mid(a+3));
							name = TrimBoth(name.Left(a));
						}
					}
				}
				
				if (name == "Note") continue;
				
				int j = -1;
				PropertyIDE& ide = db.GetAddIDE(name, &j);
				
				plat_lang.ides << j;
			}
			
			NextSubBatch();
			SetWaiting(0);
		});
	}
	else if (phase == PHASE_GET_LIBRARIES) {
		TaskMgr& m = TaskMgr::Single();
		CodeArgs args;
		args.fn = CodeArgs::GET_LIBRARIES;
		
		#if 0
		if (batch == 0 && sub_batch == 0) {
			tmp_map.Clear();
			for(int i = 0; i < db.platforms.GetCount(); i++) {
				const HardPlatform& plat = db.platforms[i];
				for(int j = 0; j < plat.os.GetCount(); j++) {
					int os_i = plat.os[j];
					const SoftPlatform& os = db.os[os_i];
					for(int k = 0; k < plat.langs.GetCount(); k++) {
						const auto& lng = plat.langs[k];
						tmp_map.GetAdd(os_i).FindAdd(lng.lng);
					}
				}
			}
		}
		if (batch >= tmp_map.GetCount()) {
			NextPhase();
			return;
		}
		
		int os_i = tmp_map.GetKey(batch);
		SoftPlatform& os = db.os[os_i];
		const auto& langs = tmp_map[batch];
		if (sub_batch >= langs.GetCount()) {
			NextBatch();
			return;
		}
		int lng_i = langs[sub_batch];
		
		SoftLanguage& lang = db.langs[lng_i];
		#else
		if (batch >= db.platforms.GetCount()) {
			NextPhase();
			return;
		}
		HardPlatform& plat = db.platforms[batch];
		
		if (sub_batch >= plat.langs.GetCount()) {
			NextBatch();
			return;
		}
		SoftLanguage& lang = db.langs[plat.langs[sub_batch].lng];
		#endif
		
		args.platform = plat.name;
		args.lang = lang.name;
		
		SetWaiting(1);
		m.GetCode(args, [this](String res) {
			TextDatabase& db = GetDatabase();
			//SoftPlatform& os = db.os[os_i];
			//SoftLanguage& lang = db.langs[lng_i];
			HardPlatform& plat = db.platforms[batch];
			auto& lng = plat.langs[sub_batch];
			SoftLanguage& lang = db.langs[lng.lng];
			res.Replace("–", "-");
			RemoveEmptyLines3(res);
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			lng.libs.Clear();
			
			for(int i = 0; i < lines.GetCount(); i++) {
				String name = TrimBoth(lines[i]);
				String desc;
				
				int a = name.Find(":");
				if (a >= 0) {
					desc = TrimBoth(name.Mid(a+1));
					name = name.Left(a);
				}
				else {
					a = name.Find("(");
					if (a >= 0) {
						desc = TrimBoth(name.Mid(a+1));
						desc = desc.Left(desc.GetCount()-1);
						name = TrimBoth(name.Left(a));
					}
					else {
						a = name.Find(" - ");
						if (a >= 0) {
							desc = TrimBoth(name.Mid(a+3));
							name = TrimBoth(name.Left(a));
						}
					}
				}
				if (name == "Note") continue;
				
				int j = -1;
				SoftLibrary& lib = db.GetAddLibrary(name, &j);
				if (lib.description.IsEmpty() && desc.GetCount()) lib.description = desc;
				lib.langs.FindAdd(lng.lng);
				lng.libs.Add(j);
			}
			
			NextSubBatch();
			SetWaiting(0);
		});
	}
	else if (phase == PHASE_GET_PROJECT_TYPES) {
		TaskMgr& m = TaskMgr::Single();
		CodeArgs args;
		args.fn = CodeArgs::GET_PROJECT_TYPES;
		
		if (batch >= db.platforms.GetCount()) {
			NextPhase();
			return;
		}
		
		HardPlatform& plat = db.platforms[batch];
		if (skip_ready && plat.project_types.GetCount()) {
			NextBatch();
			return;
		}
		
		args.platform = plat.name;
		
		SetWaiting(1);
		m.GetCode(args, [this](String res) {
			TextDatabase& db = GetDatabase();
			HardPlatform& plat = db.platforms[batch];
			
			plat.project_types.Clear();
			
			RemoveEmptyLines3(res);
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			
			for (String& s : lines) {
				int idx = -1;
				SoftProjectType& t = db.GetAddProjectType(s);
				plat.project_types << idx;
			}
			
			NextBatch();
			SetWaiting(0);
		});
	}
	
}

CodeSolver& CodeSolver::Get(int appmode) {
	static CodeSolver s[DB_COUNT];
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	s[appmode].appmode = appmode;
	return s[appmode];
}


END_TEXTLIB_NAMESPACE

