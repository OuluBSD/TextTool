#ifndef _SongTool_SongTool_h_
#define _SongTool_SongTool_h_

#include <CtrlLib/CtrlLib.h>
#include <CodeEditor/CodeEditor.h>
#include <Esc/Esc.h>
#include <plugin/jpg/jpg.h>
#define TFILE <SongTool/SongTool.t>
#include <Core/t.h>
#include <BlueBar/BlueBar.h>
#include <RedBar/RedBar.h>
using namespace Upp;

#define IMAGECLASS AppImg
#define IMAGEFILE <SongTool/App.iml>
#include <Draw/iml_header.h>

#define LAYOUTFILE <SongTool/App.lay>
#include <CtrlCore/lay.h>

#define LAYOUTFILE <SongTool/Simplified.lay>
#include <CtrlCore/lay.h>

#include "OpenAI.h"

#include "Utils.h"
#include "Optimizer.h"

#include "Common.h"
#include "Calendar.h"
#include "Task.h"
#include "Social.h"
#include "Recruiment.h"
#include "Rhymes.h"
#include "Production.h"
#include "Analysis.h"
#include "PatternMask.h"
#include "Pattern.h"
#include "Scoring.h"
#include "Composition.h"
#include "Break.h"
#include "Line.h"
#include "Part.h"
#include "Block.h"
#include "Track.h"
#include "Attributes.h"

#include "TaskCommon.h"
#include "TaskAbstraction.h"
#include "Cache.h"
#include "Tasks.h"
#include "TaskMgr.h"

#include "Pipeline.h"
#include "Song.h"
#include "Release.h"
#include "Artist.h"
#include "Database.h"

#include "TranslatorToolBase.h"
#include "ChecklistBase.h"
#include "EditorBottom.h"
#include "EditorCtrl.h"
#include "ImagePlayerBase.h"

#include "Checklists.h"
#include "AlbumBriefing.h"
#include "AlbumIdeas.h"
#include "CoverImage.h"
#include "Pitching.h"
#include "SongBriefing.h"
#include "TxtInTranslate.h"
#include "TxtStructEdit.h"
#include "TxtEditNative.h"
#include "TxtEditEnglish.h"
#include "MusicVideoImport.h"
#include "MusicVideoEdit.h"
#include "MusicVideoToPlan.h"
#include "ImageGenTool.h"

#include "AttrDataCtrl.h"
#include "BasicInfoCtrl.h"
#include "RecruimentCtrl.h"
#include "SocialCtrl.h"
#include "CalMgrCtrl.h"
#include "TaskMgrCtrl.h"
#include "PatternMaskCtrl.h"
#include "AttrCtrl.h"
#include "PatternCtrl.h"
#include "CompositionCtrl.h"
#include "AnalysisCtrl.h"
#include "ProductionCtrl.h"
#include "RhymesCtrl.h"
#include "AttrScoreCtrl.h"
#include "ReverseImpactCtrl.h"
#include "ReverseCtrl.h"
#include "Plotter.h"
#include "ImpactScoringCtrl.h"
#include "MaskScoringCtrl.h"
#include "PatternScoringCtrl.h"
#include "ImpactCtrl.h"
#include "StoryCtrl.h"
#include "StructureCtrl.h"
#include "ImportCtrl.h"
#include "LyricsCtrl.h"

#include "LineEditCtrl.h"
#include "Sequencer.h"
#include "SeqCtrl.h"

#include "FrontPage.h"
#include "CalendarPage.h"
#include "Editor.h"
#include "TaskCtrl.h"

#include "App.h"



#endif
