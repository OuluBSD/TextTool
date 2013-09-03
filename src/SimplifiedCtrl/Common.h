#ifndef _SimplifiedCtrl_Common_h_
#define _SimplifiedCtrl_Common_h_


class LineListCtrl : public SongToolCtrl {
	
protected:
	ArrayCtrl list;
	
public:
	LineListCtrl();
	void Init();
	typedef LineListCtrl CLASSNAME;
	
	// VocabularyIdea
	Vector<String> GetLineVisualIdea(int row);
	Vector<String> GetLineCharactersIdea(int row);
	Vector<String> GetLineDialogueIdea1(int row);
	Vector<String> GetLineDialogueIdea2(int row);
	Vector<Vector<String>> GetLinePhraseIdea(int row, bool colors);
	Vector<String> GetPartData(int row, String key);
	Vector<Color> GetLineColorIdea(int row);
	Vector<String> FindLineData(int row, Vector<String> lines);
	
	// WordSaladIdea
	Vector<Vector<String>> GetVocabulary(int row);
	Vector<Vector<Color>> GetColors(int row);
	Vector<Color> GetListenerColors(int row);
	Vector<Vector<Color>> GetPreviousListenerColors(int row);
	void GetPossibleParts(Song& s, StaticPart& part, Index<String>& possible_previous, Index<String>& possible_next);
	
	// TextIdea
	Vector<String> GetLineWordSalads(int row);
	Vector<Vector<String>> GetPartWordSalads(int row);
	Vector<String> GetPartVisualIdea(int row);
	Vector<String> GetPartDialogueIdea1(int row);
	Vector<String> GetPartDialogueIdea2(int row);
	
};



#endif
