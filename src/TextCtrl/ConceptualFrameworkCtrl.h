#ifndef _TextCtrl_ConceptualFramework_h_
#define _TextCtrl_ConceptualFramework_h_

/*
- conceptual framework
	- tässä sais olla pieni formi nimelle ja uskomuksen valitsemiselle
	- on myös lista kaikista CF:stä
	- tärkein lista on se idea-lista, josta valitsemalla näkee yksityiskohtia
		- listassa on typeclass/content, nimi, pisteet, ja ne on pistejärjestyksessä
		- ehkä vois olla joku lyhyt koodi-stringi, missä on se rakenne, esim. exposition=E, reptition=R ja sitten koodi olis ECRECRASDF jne.
		- 20 parhaimmalle idealle vois hakea vielä värit ja attribuutitkin
	- idea-formi
		- näyttää lyhyesti sen rakenteen
		- näyttää pitkästi sen rakenteen ja ne tekstit per elementti
		- mahdollisesti näyttää jonkun tekoälyllä parannetun rakenteen, koska elementit ei sovi saumattomasti yhteen välttämättä
- itse prosessi
	- kuinka valita elementit yhteen?
		- niinkuin elementtilistassa näkyy: sekä valitsemalla "kaikki" listasta että yksittäisistä aihealueista
	- vaiheet
		- tehdä elemnttiryhmät ja pyytää ryhmästä 1-3 parasta ideaa
		- hakea pisteytys ideoille
- promptit
	- konseptit
	- niiden pisteet
	- konseptien värit
	- konseptien attribuutit
	- rakenteen parantamine
*/

BEGIN_TEXTLIB_NAMESPACE


class ToolEditor;


class ConceptualFrameworkCtrl : public ToolAppCtrl {
	Splitter cfsplit, vsplit, tsplit, bsplit;
	ArrayCtrl cfs;
	ArrayCtrl stories;
	WithConceptualFramework<Ctrl> cf;
	WithConceptualFrameworkStory<Ctrl> story;
	DocEdit story_struct, story_improved;
	
public:
	typedef ConceptualFrameworkCtrl CLASSNAME;
	ConceptualFrameworkCtrl();
	
	void Data() override;
	void DataFramework();
	void DataStory();
	void Clear();
	void OnValueChange();
	void ToolMenu(Bar& bar) override;
	void Do(int fn);
	
	ToolEditor* editor = 0;
	
};


END_TEXTLIB_NAMESPACE


#endif
