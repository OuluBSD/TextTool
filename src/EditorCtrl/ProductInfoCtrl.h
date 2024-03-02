#ifndef _EditorCtrl_ProductInfoCtrl_h_
#define _EditorCtrl_ProductInfoCtrl_h_


class Editor;

class ProductInfoCtrl : public WithProductInfo<SongToolCtrl> {
	
	
public:
	typedef ProductInfoCtrl CLASSNAME;
	ProductInfoCtrl();
	
	void Data();
	void Clear();
	void OnValueChange();
	
	Editor* editor = 0;
	
};


#endif
