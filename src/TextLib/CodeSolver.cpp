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
		
		db.GetAddPlatform("Desktop: Intel 8088 DOS compatible");
		db.GetAddPlatform("Desktop: Intel 386 DOS compatible");
		db.GetAddPlatform("Desktop: Intel 486 Windows 3.11 compatible");
		db.GetAddPlatform("Desktop: Amiga 500 m68k");
		db.GetAddPlatform("Embedded: FPGA");
		db.GetAddPlatform("Integrated circuit: Structured VLSI");
		db.GetAddPlatform("Integrated circuit: Hardware description language");
		db.GetAddPlatform("Low level abstraction: SDL");
		db.GetAddPlatform("Low level abstraction: DirectX");
		db.GetAddPlatform("Low level abstraction: Apple Metal");
		db.GetAddPlatform("Low level abstraction: Android NDK");
		db.GetAddPlatform("Low level abstraction: Unreal Engine");
		db.GetAddPlatform("Low level abstraction: Godot Engine");
		db.GetAddPlatform("Low level abstraction: Amazon Lumberyard");
		db.GetAddPlatform("Low level abstraction: Open 3D Engine (O3DE)");
		
		if (skip_ready && db.platforms.GetCount()) {
			NextPhase();
			return;
		}
		
		SetWaiting(1);
		m.GetCode(args, [this](String res) {
			TextDatabase& db = GetDatabase();
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			
			db.platforms.Clear();
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
			
			plat.os.Clear();
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
			
			plat.langs.Clear();
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
			
			plat.paradigms.Clear();
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
		auto& plat_lang = plat.langs[sub_batch];
		if (skip_ready && plat_lang.ides.GetCount()) {
			NextSubBatch();
			return;
		}
		
		SoftLanguage& lang = db.langs[plat_lang.lng];
		
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
				SoftIDE& ide = db.GetAddIDE(name, &j);
				
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
		
		if (batch >= db.platforms.GetCount()) {
			NextPhase();
			return;
		}
		HardPlatform& plat = db.platforms[batch];
		
		if (sub_batch >= plat.langs.GetCount()) {
			NextBatch();
			return;
		}
		const auto& lng = plat.langs[sub_batch];
		if (skip_ready && lng.libs.GetCount()) {
			NextSubBatch();
			return;
		}
		SoftLanguage& lang = db.langs[lng.lng];
		
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
		if (sub_batch >= plat.langs.GetCount()) {
			NextBatch();
			return;
		}
		auto& lng = plat.langs[sub_batch];
		
		if (lng.project_types.GetCount() && lng.project_types[0] >= db.project_types.GetCount())
			lng.project_types.Clear();
		
		if (skip_ready && lng.project_types.GetCount()) {
			NextSubBatch();
			return;
		}
		SoftLanguage& l = db.langs[lng.lng];
		
		args.platform = plat.name;
		args.lang = l.name;
		
		SetWaiting(1);
		m.GetCode(args, [this](String res) {
			TextDatabase& db = GetDatabase();
			HardPlatform& plat = db.platforms[batch];
			auto& lng = plat.langs[sub_batch];
			
			lng.project_types.Clear();
			
			RemoveEmptyLines3(res);
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			
			for (String& s : lines) {
				int idx = -1;
				SoftProjectType& t = db.GetAddProjectType(s, &idx);
				lng.project_types << idx;
			}
			
			NextSubBatch();
			SetWaiting(0);
		});
	}
	else if (phase == PHASE_GET_BUILD_TOOLS) {
		TaskMgr& m = TaskMgr::Single();
		CodeArgs args;
		args.fn = CodeArgs::GET_BUILD_TOOLS;
		
		if (batch >= db.platforms.GetCount()) {
			NextPhase();
			return;
		}
		
		HardPlatform& plat = db.platforms[batch];
		if (sub_batch >= plat.langs.GetCount()) {
			NextBatch();
			return;
		}
		auto& lng = plat.langs[sub_batch];
		
		if (lng.build_tools.GetCount() && lng.build_tools[0] >= db.build_tools.GetCount())
			lng.build_tools.Clear();
		
		if (skip_ready && lng.build_tools.GetCount()) {
			NextSubBatch();
			return;
		}
		SoftLanguage& l = db.langs[lng.lng];
		
		args.platform = plat.name;
		args.lang = l.name;
		
		SetWaiting(1);
		m.GetCode(args, [this](String res) {
			TextDatabase& db = GetDatabase();
			HardPlatform& plat = db.platforms[batch];
			auto& lng = plat.langs[sub_batch];
			
			lng.build_tools.Clear();
			
			RemoveEmptyLines3(res);
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			
			for (String& s : lines) {
				int idx = -1;
				SoftBuildTool& t = db.GetAddBuildTool(s, &idx);
				lng.build_tools << idx;
			}
			
			NextSubBatch();
			SetWaiting(0);
		});
	}
	else if (phase == PHASE_GET_TESTING_FRAMEWORKS) {
		TaskMgr& m = TaskMgr::Single();
		CodeArgs args;
		args.fn = CodeArgs::GET_TESTING_FRAMEWORKS;
		
		if (batch >= db.platforms.GetCount()) {
			NextPhase();
			return;
		}
		
		HardPlatform& plat = db.platforms[batch];
		if (sub_batch >= plat.langs.GetCount()) {
			NextBatch();
			return;
		}
		auto& lng = plat.langs[sub_batch];
		
		if (lng.tests.GetCount() && lng.tests[0] >= db.tests.GetCount())
			lng.tests.Clear();
		
		if (skip_ready && lng.tests.GetCount()) {
			NextSubBatch();
			return;
		}
		SoftLanguage& l = db.langs[lng.lng];
		
		args.platform = plat.name;
		args.lang = l.name;
		
		SetWaiting(1);
		m.GetCode(args, [this](String res) {
			TextDatabase& db = GetDatabase();
			HardPlatform& plat = db.platforms[batch];
			auto& lng = plat.langs[sub_batch];
			
			lng.tests.Clear();
			
			RemoveEmptyLines3(res);
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			
			for (String& s : lines) {
				int idx = -1;
				SoftTestingFramework& t = db.GetAddTestingFramework(s, &idx);
				lng.tests << idx;
			}
			
			NextSubBatch();
			SetWaiting(0);
		});
	}
	else if (phase == PHASE_GET_DATABASE) {
		TaskMgr& m = TaskMgr::Single();
		CodeArgs args;
		args.fn = CodeArgs::GET_DATABASE;
		
		if (batch >= db.platforms.GetCount()) {
			NextPhase();
			return;
		}
		
		HardPlatform& plat = db.platforms[batch];
		if (sub_batch >= plat.langs.GetCount()) {
			NextBatch();
			return;
		}
		auto& lng = plat.langs[sub_batch];
		
		if (lng.dbs.GetCount() && lng.dbs[0] >= db.dbs.GetCount())
			lng.dbs.Clear();
		
		if (skip_ready && lng.dbs.GetCount()) {
			NextSubBatch();
			return;
		}
		SoftLanguage& l = db.langs[lng.lng];
		
		args.platform = plat.name;
		args.lang = l.name;
		
		SetWaiting(1);
		m.GetCode(args, [this](String res) {
			TextDatabase& db = GetDatabase();
			HardPlatform& plat = db.platforms[batch];
			auto& lng = plat.langs[sub_batch];
			
			lng.dbs.Clear();
			
			RemoveEmptyLines3(res);
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			
			for (String& s : lines) {
				int idx = -1;
				SoftDatabase& t = db.GetAddDatabase(s, &idx);
				lng.dbs << idx;
			}
			
			NextSubBatch();
			SetWaiting(0);
		});
	}
	else if (phase == PHASE_GET_USER_INTERFACE) {
		TaskMgr& m = TaskMgr::Single();
		CodeArgs args;
		args.fn = CodeArgs::GET_USER_INTERFACE;
		
		if (batch >= db.platforms.GetCount()) {
			NextPhase();
			return;
		}
		
		HardPlatform& plat = db.platforms[batch];
		if (sub_batch >= plat.langs.GetCount()) {
			NextBatch();
			return;
		}
		auto& lng = plat.langs[sub_batch];
		
		if (lng.uis.GetCount() && lng.uis[0] >= db.uis.GetCount())
			lng.uis.Clear();
		
		if (skip_ready && lng.uis.GetCount()) {
			NextSubBatch();
			return;
		}
		SoftLanguage& l = db.langs[lng.lng];
		
		args.platform = plat.name;
		args.lang = l.name;
		
		SetWaiting(1);
		m.GetCode(args, [this](String res) {
			TextDatabase& db = GetDatabase();
			HardPlatform& plat = db.platforms[batch];
			auto& lng = plat.langs[sub_batch];
			
			lng.uis.Clear();
			
			RemoveEmptyLines3(res);
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			
			for (String& s : lines) {
				int idx = -1;
				SoftUserInterface& t = db.GetAddUserInterface(s, &idx);
				lng.uis << idx;
			}
			
			NextSubBatch();
			SetWaiting(0);
		});
	}
	else if (phase == PHASE_GET_SECURITY_FRAMEWORKS) {
		TaskMgr& m = TaskMgr::Single();
		CodeArgs args;
		args.fn = CodeArgs::GET_SECURITY_FRAMEWORKS;
		
		if (batch >= db.platforms.GetCount()) {
			NextPhase();
			return;
		}
		
		HardPlatform& plat = db.platforms[batch];
		if (sub_batch >= plat.langs.GetCount()) {
			NextBatch();
			return;
		}
		auto& lng = plat.langs[sub_batch];
		
		if (lng.secs.GetCount() && lng.secs[0] >= db.secs.GetCount())
			lng.secs.Clear();
		
		if (skip_ready && lng.secs.GetCount()) {
			NextSubBatch();
			return;
		}
		SoftLanguage& l = db.langs[lng.lng];
		
		args.platform = plat.name;
		args.lang = l.name;
		
		SetWaiting(1);
		m.GetCode(args, [this](String res) {
			TextDatabase& db = GetDatabase();
			HardPlatform& plat = db.platforms[batch];
			auto& lng = plat.langs[sub_batch];
			
			lng.secs.Clear();
			
			RemoveEmptyLines3(res);
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			
			for (String& s : lines) {
				int idx = -1;
				SoftSecurityFramework& t = db.GetAddSecurityFramework(s, &idx);
				lng.secs << idx;
			}
			
			NextSubBatch();
			SetWaiting(0);
		});
	}
	else if (phase == PHASE_GET_DATA_FORMATS) {
		TaskMgr& m = TaskMgr::Single();
		CodeArgs args;
		args.fn = CodeArgs::GET_DATA_FORMATS;
		
		if (batch >= db.langs.GetCount()) {
			NextPhase();
			return;
		}
		
		SoftLanguage& lang = db.langs[batch];
		
		if (lang.formats.GetCount() && lang.formats[0] >= db.formats.GetCount())
			lang.formats.Clear();
		
		if (skip_ready && lang.formats.GetCount()) {
			NextBatch();
			return;
		}
		
		args.lang = lang.name;
		
		SetWaiting(1);
		m.GetCode(args, [this](String res) {
			TextDatabase& db = GetDatabase();
			SoftLanguage& lang = db.langs[batch];
			
			lang.formats.Clear();
			
			RemoveEmptyLines3(res);
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			
			for (String& s : lines) {
				int idx = -1;
				SoftDataFormat& t = db.GetAddDataFormat(s, &idx);
				lang.formats << idx;
			}
			
			NextBatch();
			SetWaiting(0);
		});
	}
	else if (phase == PHASE_GET_ALGORITHMS) {
		TaskMgr& m = TaskMgr::Single();
		CodeArgs args;
		args.fn = CodeArgs::GET_ALGORITHMS;
		
		if (batch >= db.langs.GetCount()) {
			NextPhase();
			return;
		}
		
		SoftLanguage& lang = db.langs[batch];
		
		if (lang.algos.GetCount() && lang.algos[0] >= db.algos.GetCount())
			lang.algos.Clear();
		
		if (skip_ready && lang.algos.GetCount()) {
			NextBatch();
			return;
		}
		
		args.lang = lang.name;
		
		SetWaiting(1);
		m.GetCode(args, [this](String res) {
			TextDatabase& db = GetDatabase();
			SoftLanguage& lang = db.langs[batch];
			
			lang.algos.Clear();
			
			RemoveEmptyLines3(res);
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			
			for (String& s : lines) {
				int idx = -1;
				SoftAlgorithm& t = db.GetAddAlgorithm(s, &idx);
				lang.algos << idx;
			}
			
			NextBatch();
			SetWaiting(0);
		});
	}
	else if (phase == PHASE_GET_DATA_PROCESSING) {
		TaskMgr& m = TaskMgr::Single();
		CodeArgs args;
		args.fn = CodeArgs::GET_DATA_PROCESSING;
		
		if (batch >= db.langs.GetCount()) {
			NextPhase();
			return;
		}
		
		SoftLanguage& lang = db.langs[batch];
		
		if (lang.data_procs.GetCount() && lang.data_procs[0] >= db.data_procs.GetCount())
			lang.data_procs.Clear();
		
		if (skip_ready && lang.data_procs.GetCount()) {
			NextBatch();
			return;
		}
		
		args.lang = lang.name;
		
		SetWaiting(1);
		m.GetCode(args, [this](String res) {
			TextDatabase& db = GetDatabase();
			SoftLanguage& lang = db.langs[batch];
			
			lang.data_procs.Clear();
			
			RemoveEmptyLines3(res);
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			
			for (String& s : lines) {
				int idx = -1;
				SoftDataProcessing& t = db.GetAddDataProcessing(s, &idx);
				lang.data_procs << idx;
			}
			
			NextBatch();
			SetWaiting(0);
		});
	}
	else if (phase == PHASE_GET_DEPLOYMENT_ENVS) {
		TaskMgr& m = TaskMgr::Single();
		CodeArgs args;
		args.fn = CodeArgs::GET_DEPLOYMENT_ENVS;
		
		if (batch >= db.platforms.GetCount()) {
			NextPhase();
			return;
		}
		
		HardPlatform& plat = db.platforms[batch];
		
		if (plat.deployments.GetCount() && plat.deployments[0] >= db.deployments.GetCount())
			plat.deployments.Clear();
		
		if (skip_ready && plat.deployments.GetCount()) {
			NextBatch();
			return;
		}
		
		args.platform = plat.name;
		
		SetWaiting(1);
		m.GetCode(args, [this](String res) {
			TextDatabase& db = GetDatabase();
			HardPlatform& plat = db.platforms[batch];
			
			plat.deployments.Clear();
			
			RemoveEmptyLines3(res);
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			
			for (String& s : lines) {
				int idx = -1;
				SoftDeployment& t = db.GetAddDeployment(s, &idx);
				plat.deployments << idx;
			}
			
			NextBatch();
			SetWaiting(0);
		});
	}
	else if (phase == PHASE_GET_INTEGRATION_APIS) {
		TaskMgr& m = TaskMgr::Single();
		CodeArgs args;
		args.fn = CodeArgs::GET_INTEGRATION_APIS;
		
		if (skip_ready && db.apis.GetCount()) {
			NextPhase();
			return;
		}
		
		SetWaiting(1);
		m.GetCode(args, [this](String res) {
			TextDatabase& db = GetDatabase();
			
			db.apis.Clear();
			
			RemoveEmptyLines3(res);
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			
			for (String& s : lines) {
				int idx = -1;
				SoftIntegrationApi& t = db.apis.Add();
				t.name = s;
			}
			
			NextPhase();
			SetWaiting(0);
		});
	}
	else if (phase == PHASE_GET_COLLABORATION_TOOLS) {
		TaskMgr& m = TaskMgr::Single();
		CodeArgs args;
		args.fn = CodeArgs::GET_COLLABORATION_TOOLS;
		
		if (skip_ready && db.collab_tools.GetCount()) {
			NextPhase();
			return;
		}
		
		SetWaiting(1);
		m.GetCode(args, [this](String res) {
			TextDatabase& db = GetDatabase();
			
			db.collab_tools.Clear();
			
			RemoveEmptyLines3(res);
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			
			for (String& s : lines) {
				int idx = -1;
				SoftCollaborationTool& t = db.collab_tools.Add();
				t.name = s;
			}
			
			NextPhase();
			SetWaiting(0);
		});
	}
	else if (phase == PHASE_GET_PROJECT_MANAGEMENTS) {
		TaskMgr& m = TaskMgr::Single();
		CodeArgs args;
		args.fn = CodeArgs::GET_PROJECT_MANAGEMENTS;
		
		if (skip_ready && db.prj_manags.GetCount()) {
			NextPhase();
			return;
		}
		
		SetWaiting(1);
		m.GetCode(args, [this](String res) {
			TextDatabase& db = GetDatabase();
			
			db.prj_manags.Clear();
			
			RemoveEmptyLines3(res);
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			
			for (String& s : lines) {
				int idx = -1;
				SoftProjectManagement& t = db.prj_manags.Add();
				t.name = s;
			}
			
			NextPhase();
			SetWaiting(0);
		});
	}
	else if (phase == PHASE_GET_MARKET_TARGETS) {
		const auto& tgts = GetSoftMarketTargets();
		
		if (db.market_targets.IsEmpty()) {
			for(String s : tgts) {
				auto& tgt = db.market_targets.Add();
				
				int a = s.Find(": ");
				if (a >= 0) {
					tgt.name = s.Left(a);
					tgt.description = s.Mid(a+2);
				}
				else tgt.name = s;
			}
		}
		
		NextPhase();
	}
	else if (phase == PHASE_GET_MARKET_TRENDS) {
		if (batch >= db.market_targets.GetCount()) {
			NextPhase();
			return;
		}
		SoftMarketTarget& mt = db.market_targets[batch];
		
		if (skip_ready && mt.trends.GetCount()) {
			NextBatch();
			return;
		}
		
		CodeArgs args;
		args.fn = CodeArgs::GET_MARKET_TRENDS;
		
		args.market_target = mt.name + ": " + mt.description;
		
		SetWaiting(1);
		TaskMgr& m = TaskMgr::Single();
		m.GetCode(args, [this](String res) {
			TextDatabase& db = GetDatabase();
			SoftMarketTarget& mt = db.market_targets[batch];
			
			mt.trends.Clear();
			
			RemoveEmptyLines3(res);
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			for (String& s : lines) {
				int idx = -1;
				String name, description;
				int a = s.Find(": ");
				if (a >= 0) {
					name = s.Left(a);
					description = s.Mid(a+2);
				}
				else name = s;
				
				
				auto& t = db.GetAddMarketTrend(name, &idx);
				if (t.description.IsEmpty() && description.GetCount())
					t.description = description;
				
				mt.trends.Add().idx = idx;
			}
			
			NextBatch();
			SetWaiting(0);
		});
	}
	else if (phase == PHASE_GET_GENRES) {
		if (batch >= db.market_targets.GetCount()) {
			NextPhase();
			return;
		}
		SoftMarketTarget& mt = db.market_targets[batch];
		if (sub_batch >= mt.trends.GetCount()){
			NextBatch();
			return;
		}
		auto& mt_trend = mt.trends[sub_batch];
		SoftMarketTrend& trend = db.market_trends[mt_trend.idx];
		
		if (skip_ready && mt_trend.genres.GetCount()) {
			NextSubBatch();
			return;
		}
		
		CodeArgs args;
		args.fn = CodeArgs::GET_GENRES;
		
		args.market_target = mt.name + ": " + mt.description;
		args.market_trend = trend.name + ": " + trend.description;
		
		SetWaiting(1);
		TaskMgr& m = TaskMgr::Single();
		m.GetCode(args, [this](String res) {
			TextDatabase& db = GetDatabase();
			SoftMarketTarget& mt = db.market_targets[batch];
			auto& mt_trend = mt.trends[sub_batch];
			SoftMarketTrend& trend = db.market_trends[mt_trend.idx];
			
			mt_trend.genres.Clear();
			
			RemoveEmptyLines3(res);
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			for (String& s : lines) {
				int idx = -1;
				auto& t = db.GetAddGenre(s, &idx);
				mt_trend.genres << idx;
			}
			
			NextSubBatch();
			SetWaiting(0);
		});
	}
	else if (phase == PHASE_GET_USER_NEEDS) {
		if (batch >= db.market_targets.GetCount()) {
			NextPhase();
			return;
		}
		SoftMarketTarget& mt = db.market_targets[batch];
		if (sub_batch >= mt.trends.GetCount()){
			NextBatch();
			return;
		}
		auto& mt_trend = mt.trends[sub_batch];
		SoftMarketTrend& trend = db.market_trends[mt_trend.idx];
		
		if (skip_ready && mt_trend.user_needs.GetCount()) {
			NextSubBatch();
			return;
		}
		
		CodeArgs args;
		args.fn = CodeArgs::GET_USER_NEEDS;
		
		args.market_target = mt.name + ": " + mt.description;
		args.market_trend = trend.name + ": " + trend.description;
		
		SetWaiting(1);
		TaskMgr& m = TaskMgr::Single();
		m.GetCode(args, [this](String res) {
			TextDatabase& db = GetDatabase();
			SoftMarketTarget& mt = db.market_targets[batch];
			auto& mt_trend = mt.trends[sub_batch];
			SoftMarketTrend& trend = db.market_trends[mt_trend.idx];
			
			mt_trend.user_needs.Clear();
			
			RemoveEmptyLines3(res);
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			for (String& s : lines) {
				int idx = -1;
				auto& t = db.GetAddUserNeed(s, &idx);
				mt_trend.user_needs << idx;
			}
			
			NextSubBatch();
			SetWaiting(0);
		});
	}
	else if (phase == PHASE_GET_PRODUCTS) {
		if (batch >= db.market_targets.GetCount()) {
			NextPhase();
			return;
		}
		SoftMarketTarget& mt = db.market_targets[batch];
		if (sub_batch >= mt.trends.GetCount()){
			NextBatch();
			return;
		}
		auto& mt_trend = mt.trends[sub_batch];
		SoftMarketTrend& trend = db.market_trends[mt_trend.idx];
		
		if (skip_ready && mt_trend.products.GetCount()) {
			NextSubBatch();
			return;
		}
		
		CodeArgs args;
		args.fn = CodeArgs::GET_PRODUCTS;
		
		args.market_target = mt.name + ": " + mt.description;
		args.market_trend = trend.name + ": " + trend.description;
		
		SetWaiting(1);
		TaskMgr& m = TaskMgr::Single();
		m.GetCode(args, [this](String res) {
			TextDatabase& db = GetDatabase();
			SoftMarketTarget& mt = db.market_targets[batch];
			auto& mt_trend = mt.trends[sub_batch];
			SoftMarketTrend& trend = db.market_trends[mt_trend.idx];
			
			mt_trend.products.Clear();
			
			RemoveEmptyLines3(res);
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			for (String& s : lines) {
				int idx = -1;
				auto& t = db.GetAddProduct(s, &idx);
				mt_trend.products << idx;
			}
			
			NextSubBatch();
			SetWaiting(0);
		});
	}
	else if (phase == PHASE_GET_USER_FEEDBACKS) {
		if (batch >= db.market_targets.GetCount()) {
			NextPhase();
			return;
		}
		SoftMarketTarget& mt = db.market_targets[batch];
		if (sub_batch >= mt.trends.GetCount()){
			NextBatch();
			return;
		}
		auto& mt_trend = mt.trends[sub_batch];
		SoftMarketTrend& trend = db.market_trends[mt_trend.idx];
		
		if (skip_ready && mt_trend.feedbacks.GetCount()) {
			NextSubBatch();
			return;
		}
		
		CodeArgs args;
		args.fn = CodeArgs::GET_USER_FEEDBACKS;
		
		args.market_target = mt.name + ": " + mt.description;
		args.market_trend = trend.name + ": " + trend.description;
		
		SetWaiting(1);
		TaskMgr& m = TaskMgr::Single();
		m.GetCode(args, [this](String res) {
			TextDatabase& db = GetDatabase();
			SoftMarketTarget& mt = db.market_targets[batch];
			auto& mt_trend = mt.trends[sub_batch];
			SoftMarketTrend& trend = db.market_trends[mt_trend.idx];
			
			mt_trend.feedbacks.Clear();
			
			RemoveEmptyLines3(res);
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			for (String& s : lines) {
				int idx = -1;
				auto& t = db.GetAddFeedback(s, &idx);
				mt_trend.feedbacks << idx;
			}
			
			NextSubBatch();
			SetWaiting(0);
		});
	}
	else if (phase == PHASE_GET_MARKETING_STRATEGIES) {
		if (batch >= db.market_targets.GetCount()) {
			NextPhase();
			return;
		}
		SoftMarketTarget& mt = db.market_targets[batch];
		if (sub_batch >= mt.trends.GetCount()){
			NextBatch();
			return;
		}
		auto& mt_trend = mt.trends[sub_batch];
		SoftMarketTrend& trend = db.market_trends[mt_trend.idx];
		
		if (skip_ready && mt_trend.marketing_strategies.GetCount()) {
			NextSubBatch();
			return;
		}
		
		CodeArgs args;
		args.fn = CodeArgs::GET_MARKETING_STRATEGIES;
		
		args.market_target = mt.name + ": " + mt.description;
		args.market_trend = trend.name + ": " + trend.description;
		
		SetWaiting(1);
		TaskMgr& m = TaskMgr::Single();
		m.GetCode(args, [this](String res) {
			TextDatabase& db = GetDatabase();
			SoftMarketTarget& mt = db.market_targets[batch];
			auto& mt_trend = mt.trends[sub_batch];
			SoftMarketTrend& trend = db.market_trends[mt_trend.idx];
			
			mt_trend.marketing_strategies.Clear();
			
			RemoveEmptyLines3(res);
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			for (String& s : lines) {
				int idx = -1;
				auto& t = db.GetAddMarketingStrategy(s, &idx);
				mt_trend.marketing_strategies << idx;
			}
			
			NextSubBatch();
			SetWaiting(0);
		});
	}
	else if (phase == PHASE_GET_COLLABORATORS) {
		if (batch >= db.market_targets.GetCount()) {
			NextPhase();
			return;
		}
		SoftMarketTarget& mt = db.market_targets[batch];
		if (sub_batch >= mt.trends.GetCount()){
			NextBatch();
			return;
		}
		auto& mt_trend = mt.trends[sub_batch];
		SoftMarketTrend& trend = db.market_trends[mt_trend.idx];
		
		if (skip_ready && mt_trend.collaborators.GetCount()) {
			NextSubBatch();
			return;
		}
		
		CodeArgs args;
		args.fn = CodeArgs::GET_COLLABORATORS;
		
		args.market_target = mt.name + ": " + mt.description;
		args.market_trend = trend.name + ": " + trend.description;
		
		SetWaiting(1);
		TaskMgr& m = TaskMgr::Single();
		m.GetCode(args, [this](String res) {
			TextDatabase& db = GetDatabase();
			SoftMarketTarget& mt = db.market_targets[batch];
			auto& mt_trend = mt.trends[sub_batch];
			SoftMarketTrend& trend = db.market_trends[mt_trend.idx];
			
			mt_trend.collaborators.Clear();
			
			RemoveEmptyLines3(res);
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			for (String& s : lines) {
				int idx = -1;
				auto& t = db.GetAddCollaborator(s, &idx);
				mt_trend.collaborators << idx;
			}
			
			NextSubBatch();
			SetWaiting(0);
		});
	}
	else if (phase == PHASE_GET_REQUIREMENTS) {
		if (batch >= db.market_targets.GetCount()) {
			NextPhase();
			return;
		}
		SoftMarketTarget& mt = db.market_targets[batch];
		if (sub_batch >= mt.trends.GetCount()){
			NextBatch();
			return;
		}
		auto& mt_trend = mt.trends[sub_batch];
		SoftMarketTrend& trend = db.market_trends[mt_trend.idx];
		
		if (skip_ready && mt_trend.requirements.GetCount()) {
			NextSubBatch();
			return;
		}
		
		CodeArgs args;
		args.fn = CodeArgs::GET_REQUIREMENTS;
		
		args.market_target = mt.name + ": " + mt.description;
		args.market_trend = trend.name + ": " + trend.description;
		
		SetWaiting(1);
		TaskMgr& m = TaskMgr::Single();
		m.GetCode(args, [this](String res) {
			TextDatabase& db = GetDatabase();
			SoftMarketTarget& mt = db.market_targets[batch];
			auto& mt_trend = mt.trends[sub_batch];
			SoftMarketTrend& trend = db.market_trends[mt_trend.idx];
			
			mt_trend.requirements.Clear();
			
			RemoveEmptyLines3(res);
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			for (String& s : lines) {
				int idx = -1;
				auto& t = db.GetAddRequirement(s, &idx);
				mt_trend.requirements << idx;
			}
			
			NextSubBatch();
			SetWaiting(0);
		});
	}
	else if (phase == PHASE_GET_PRICING) {
		if (batch >= db.market_targets.GetCount()) {
			NextPhase();
			return;
		}
		SoftMarketTarget& mt = db.market_targets[batch];
		if (sub_batch >= mt.trends.GetCount()){
			NextBatch();
			return;
		}
		auto& mt_trend = mt.trends[sub_batch];
		SoftMarketTrend& trend = db.market_trends[mt_trend.idx];
		
		if (skip_ready && mt_trend.pricing.GetCount()) {
			NextSubBatch();
			return;
		}
		
		CodeArgs args;
		args.fn = CodeArgs::GET_PRICING;
		
		args.market_target = mt.name + ": " + mt.description;
		args.market_trend = trend.name + ": " + trend.description;
		
		SetWaiting(1);
		TaskMgr& m = TaskMgr::Single();
		m.GetCode(args, [this](String res) {
			TextDatabase& db = GetDatabase();
			SoftMarketTarget& mt = db.market_targets[batch];
			auto& mt_trend = mt.trends[sub_batch];
			SoftMarketTrend& trend = db.market_trends[mt_trend.idx];
			
			mt_trend.pricing.Clear();
			
			RemoveEmptyLines3(res);
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			for (String& s : lines) {
				mt_trend.pricing << s;
			}
			
			NextSubBatch();
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

