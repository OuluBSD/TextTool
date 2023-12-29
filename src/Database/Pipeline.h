#ifndef _SongTool_Pipeline_h_
#define _SongTool_Pipeline_h_

#if 0
struct Pipe :
	TaskMgr
{
	// Local
	/*VectorMap<String,String>	data;
	String						structure_str;
	Vector<String>				structure;
	
	MArr<String>					content;
	MArr<VectorMap<String,String>>	analysis;*/
	
	
	// Temp
	Song*						song = 0;
	PipePtrs					p;
	RWMutex						lock;
	
	
	
	
};


#endif
#endif
