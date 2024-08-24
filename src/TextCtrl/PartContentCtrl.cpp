#include "TextCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


PartContentCtrl::PartContentCtrl(ScriptReferenceMakerCtrl& o) : o(o) {
	AddFrame(scroll);
	
	scroll.WhenScroll << THISBACK(Layout);
}

void PartContentCtrl::Paint(Draw& d) {
	
}

void PartContentCtrl::Layout() {
	
}

void PartContentCtrl::Data() {
	const EditorPtrs& p = o.GetPointers();
	if (!p.script || !o.parts.IsCursor())
		return;
	
	Script& s = *p.script;
	int part_i = o.parts.GetCursor();
	const DynPart& dp = s.parts[part_i];
	
	for (auto& e : sub_splitters)
		RemoveChild(&e);
	for (auto& e : sub_elements)
		RemoveChild(&e);
	sub_splitters.Clear();
	sub_elements.Clear();
	sub_ids.Clear();
	int y = 0;
	int lh = 28;
	int lw = GetSize().cx;
	int indent = 30;
	
	{
		sub_ids << -1;
		sub_el_ids << 0;
		DropList& el_list = sub_elements.Add();
		AddElements(el_list);
		el_list.SetIndex(FindElement(dp.element)+1);
		Add(el_list.TopPos(y, lh).HSizePos(0,0));
		y += lh;
		el_list <<= THISBACK3(OnElementChange, -1, 0, &el_list);
	}
	
	for(int i = 0; i < dp.sub.GetCount(); i++) {
		const DynSub& ds = dp.sub[i];
		int w = (lw - indent) / 2;
		Splitter& split = sub_splitters.Add();
		split.Horz();
		Add(split.TopPos(y, lh).HSizePos(indent, 0));
		y += lh;
		
		{
			sub_ids << i;
			sub_el_ids << 0;
			DropList& el_list = sub_elements.Add();
			AddElements(el_list);
			el_list.SetIndex(FindElement(ds.element0)+1);
			el_list <<= THISBACK3(OnElementChange, i, 0, &el_list);
			split << el_list;
		}
		{
			sub_ids << i;
			sub_el_ids << 1;
			DropList& el_list = sub_elements.Add();
			AddElements(el_list);
			el_list.SetIndex(FindElement(ds.element1)+1);
			el_list <<= THISBACK3(OnElementChange, i, 1, &el_list);
			split << el_list;
		}
	}
	
}

void PartContentCtrl::AddElements(DropList& dl) {
	#if 0
	const Vector<String>& el = GetElements();
	#else
	if (element_keys.IsEmpty()) {
		TextDatabase& db = o.GetDatabase();
		SourceData& sd = db.src_data;
		SourceDataAnalysis& sda = db.src_data.a;
		DatasetAnalysis& da = sda.dataset;
		const auto& el = da.element_keys.GetKeys();
		element_keys <<= el;
		Sort(element_keys, StdLess<String>());
	}
	const auto& el = element_keys;
	#endif
	
	dl.Add("");
	for (const auto& e : el)
		dl.Add(Capitalize(e));
}

int PartContentCtrl::FindElement(const String& s) {
	const Vector<String>& el = element_keys;// GetElements();
	String e = ToLower(s);
	for(int i = 0; i < el.GetCount(); i++) {
		if (el[i] == e)
			return i;
	}
	return -1;
}

void PartContentCtrl::OnElementChange(int sub_i, int el_i, DropList* dl) {
	const EditorPtrs& p = o.GetPointers();
	if (!p.script || !o.parts.IsCursor())
		return;
	Script& s = *p.script;
	int part_i = o.parts.GetCursor();
	DynPart& dp = s.parts[part_i];
	
	if (sub_i < 0) {
		dp.element = ToLower((String)dl->GetKey(dl->GetIndex()));
	}
	else {
		DynSub& ds = dp.sub[sub_i];
		if (el_i == 0)
			ds.element0 = ToLower((String)dl->GetKey(dl->GetIndex()));
		if (el_i == 1)
			ds.element1 = ToLower((String)dl->GetKey(dl->GetIndex()));
	}
}


END_TEXTLIB_NAMESPACE

