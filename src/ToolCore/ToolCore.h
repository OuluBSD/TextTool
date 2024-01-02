#ifndef _ToolCore_ToolCore_h_
#define _ToolCore_ToolCore_h_

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
#define IMAGEFILE <ToolCore/App.iml>
#include <Draw/iml_header.h>

#define LAYOUTFILE <ToolCore/App.lay>
#include <CtrlCore/lay.h>


#include "OpenAI.h"

#include "Utils.h"
#include "Optimizer.h"

#include "Phoneme.h"
#include "Nana.h"
#include "Common.h"
#include "FileClasses.h"
#include "Tokenizer.h"


#endif
