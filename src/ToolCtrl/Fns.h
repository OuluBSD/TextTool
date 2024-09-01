#ifndef _ToolCtrl_Fns_h_
#define _ToolCtrl_Fns_h_


void GuiStartup();

void SetCountForArray(ArrayCtrl& arr, int count);
void SetCountWithDefaultCursor(ArrayCtrl& arr, int count);
void SetCountWithDefaultCursor(ArrayCtrl& arr, int count, int sort_row, bool descending=false);

int FindArrayIndex(ArrayCtrl& arr, int idx);


#endif
