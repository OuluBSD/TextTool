#include "OrgCtrl.h"


BEGIN_TEXTLIB_NAMESPACE



template <class T> void DebugCtrl::SubItem2(T& o) {}
template <> void DebugCtrl::SubItem2(HardPlatform& o) {
	
}


template <class T> void DebugCtrl::SubItem1(T& o) {}
template <> void DebugCtrl::SubItem1(HardPlatform::Lang& o) {
	TextDatabase& db = view->org->GetDatabase();
	if (o.lng >= 0) {
		SoftLanguage& l = db.langs[o.lng];
		subitems2.Add("Language", l.name);
	}
	else
		subitems2.Add("Language");
	
	subitems2.Add("IDEs", o.ides.GetCount());
	subitems2.Add("Libraries", o.libs.GetCount());
	subitems2.Add("Project types", o.project_types.GetCount());
	subitems2.Add("Build tools", o.build_tools.GetCount());
	subitems2.Add("Tests", o.tests.GetCount());
	subitems2.Add("Databases", o.dbs.GetCount());
	subitems2.Add("User interfaces", o.uis.GetCount());
	subitems2.Add("Security frameworks", o.secs.GetCount());
	subitem2_cbs.Add([this,&o](){});
	#define ADD(x) \
	subitem2_cbs.Add([this,&o,&db](){ \
		for(int i = 0; i < o.x.GetCount(); i++) { \
			int j = o.x[i]; \
			if (j >= db.x.GetCount()) {o.x.Remove(i--); continue;} \
			auto& l = db.x[j]; \
			subitems3.Add(l.name, l.description); \
		} \
	});
	ADD(ides);
	ADD(libs);
	ADD(project_types);
	ADD(build_tools);
	ADD(tests);
	ADD(dbs);
	ADD(uis);
	ADD(secs);

}


template <class T> void DebugCtrl::SubItem0(T& o) {}
template <> void DebugCtrl::SubItem0(HardPlatform& o) {
	subitems0.Add("Name", o.name);
	subitems0.Add("Description", o.description);
	subitems0.Add("Operating System", o.os.GetCount());
	subitems0.Add("Languages", o.langs.GetCount());
	subitems0.Add("Paradigms", o.paradigms.GetCount());
	subitems0.Add("Deployments", o.deployments.GetCount());
	
	subitem0_cbs.Clear();
	subitem0_cbs.Add([this,&o](){});
	subitem0_cbs.Add([this,&o](){});
	subitem0_cbs.Add([this,&o](){
		ASSERT(subitem1_cbs.IsEmpty());
		TextDatabase& db = view->org->GetDatabase();
		for(int i = 0; i < o.os.GetCount(); i++) {
			const auto& sub = db.os[o.os[i]];
			subitems1.Add(sub.name, sub.description);
		}
	});
	subitem0_cbs.Add([this,&o](){
		ASSERT(subitem1_cbs.IsEmpty());
		TextDatabase& db = view->org->GetDatabase();
		for(int i = 0; i < o.langs.GetCount(); i++) {
			auto& l = o.langs[i];
			auto& sub = db.langs[l.lng];
			subitems1.Add(sub.name, sub.description);
			subitem1_cbs.Add([this,&l](){SubItem1(l);});
		}
	});
	subitem0_cbs.Add([this,&o](){
		ASSERT(subitem1_cbs.IsEmpty());
		TextDatabase& db = view->org->GetDatabase();
		for(int i = 0; i < o.paradigms.GetCount(); i++) {
			auto& sub = db.paradigms[o.paradigms[i]];
			subitems1.Add(sub.name, sub.description);
			subitem1_cbs.Add([this,&sub](){SubItem1(sub);});
		}
	});
	subitem0_cbs.Add([this,&o](){
		ASSERT(subitem1_cbs.IsEmpty());
		TextDatabase& db = view->org->GetDatabase();
		for(int i = 0; i < o.deployments.GetCount(); i++) {
			int j = o.deployments[i];
			if (j >= db.deployments.GetCount()) {o.deployments.Remove(i--); continue;}
			auto& sub = db.deployments[j];
			subitems1.Add(sub.name, sub.description);
			subitem1_cbs.Add([this,&sub](){SubItem1(sub);});
		}
	});
}


template <class T> void DebugCtrl::RegisterType(Array<T>& array, String name) {
	type_cbs.Add(name, [this,&array](){
		item_cbs.Clear();
		for(int i = 0; i < array.GetCount(); i++) {
			T& o = array[i];
			items.Set(i, 0, o.name);
			items.Set(i, 1, o.description);
			items.Set(i, "IDX", i);
			
			item_cbs.Add([this,&o](){SubItem0<T>(o);});
		}
		INHIBIT_CURSOR(items);
		items.SetCount(array.GetCount());
		if (!items.IsCursor() && items.GetCount())
			items.SetCursor(0);
	});
	count_cbs.Add([this,&array](int& i) {i = array.GetCount();});
}

void DebugCtrl::RegisterTypes() {
	if (type_cbs.IsEmpty()) {
		TextDatabase& db = view->org->GetDatabase();
		RegisterType(db.platforms, "HardPlatform");
		RegisterType(db.os, "Platform");
		RegisterType(db.langs, "Language");
		RegisterType(db.paradigms, "Paradigm");
		RegisterType(db.ides, "IDE");
		RegisterType(db.libs, "Library");
		RegisterType(db.project_types, "Project type");
		RegisterType(db.build_tools, "Build tool");
		RegisterType(db.tests, "Testing framework");
		RegisterType(db.dbs, "Database");
		RegisterType(db.uis, "User interface");
		RegisterType(db.secs, "Security framework");
		RegisterType(db.formats, "Data format");
		RegisterType(db.algos, "Algorithm");
		RegisterType(db.data_procs, "Data processing");
		RegisterType(db.deployments, "Deployment");
		RegisterType(db.apis, "Integration api");
		RegisterType(db.collab_tools, "Collaboration tool");
		RegisterType(db.prj_manags, "Project management");
		RegisterType(db.market_targets, "Market target");
		RegisterType(db.market_trends, "Market trend");
		RegisterType(db.genres, "Genre");
		RegisterType(db.products, "Product");
		RegisterType(db.user_needs, "User need");
		RegisterType(db.marketing_strategies, "Marketing strategy");
		RegisterType(db.feedbacks, "Feedback");
		RegisterType(db.collaborators, "Collaborator");
		RegisterType(db.requirements, "Requirement");
	}
}


END_TEXTLIB_NAMESPACE
