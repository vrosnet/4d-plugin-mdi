/* --------------------------------------------------------------------------------
 #
 #	4DPlugin.cpp
 #	source generated by 4D Plugin Wizard
 #	Project : MDI
 #	author : miyako
 #	2015/07/10
 #
 # --------------------------------------------------------------------------------*/


#include "4DPluginAPI.h"
#include "4DPlugin.h"

#if VERSIONWIN
namespace MDI{
    HWND windowRef = NULL;
}
#endif

void PluginMain(PA_long32 selector, PA_PluginParameters params)
{
	try
	{
		PA_long32 pProcNum = selector;
		sLONG_PTR *pResult = (sLONG_PTR *)params->fResult;
		PackagePtr pParams = (PackagePtr)params->fParameters;

		CommandDispatcher(pProcNum, pResult, pParams); 
	}
	catch(...)
	{

	}
}

#if VERSIONWIN
HWND MDI_getHWND(){
    //the window class is the folder name of the application 
	HWND mdi = NULL;
    wchar_t path[_MAX_PATH] = {0};
    wchar_t * applicationPath = wcscpy(path, (const wchar_t *)PA_GetApplicationFullPath().fString);
	//remove file name (4D.exe)
    PathRemoveFileSpec(path);
	//check instance as well, to be sure
	HINSTANCE h = (HINSTANCE)PA_Get4DHInstance();
	do{
		mdi = FindWindowEx(NULL, mdi, (LPCTSTR)path, NULL); 
		if(mdi){
			if(h == (HINSTANCE)GetWindowLongPtr(mdi, GWLP_HINSTANCE)){
				break;
			}
		}
	}while(mdi);
    return mdi;
}
#endif

void CommandDispatcher (PA_long32 pProcNum, sLONG_PTR *pResult, PackagePtr pParams)
{
	switch(pProcNum)
	{
		case kInitPlugin :
		case kServerInitPlugin :  
#if VERSIONWIN              
			MDI::windowRef = MDI_getHWND();
#endif            
			break;		    
// --- MDI

		case 1 :
			MDI_Get_title(pResult, pParams);
			break;

		case 2 :
			MDI_SET_TITLE(pResult, pParams);
			break;

		case 3 :
			MDI_USE_ICON_FILE(pResult, pParams);
			break;

		case 4 :
			MDI_MAXIMIZE(pResult, pParams);
			break;

		case 5 :
			MDI_MINIMIZE(pResult, pParams);
			break;

		case 6 :
			MDI_SET_CLOSE_BOX_ENABLED(pResult, pParams);
			break;

		case 7 :
			MDI_Get_close_box_enabled(pResult, pParams);
			break;

		case 8 :
			MDI_RESTORE(pResult, pParams);
			break;

		case 9 :
			MDI_SET_POSITION(pResult, pParams);
			break;

		case 10 :
			MDI_GET_POSITION(pResult, pParams);
			break;

	}
}

// -------------------------------------- MDI -------------------------------------


void MDI_Get_title(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT returnValue;
#if VERSIONWIN
	size_t size = GetWindowTextLength(MDI::windowRef);
	size++;
	std::vector<wchar_t> buf(size);	
	size = GetWindowText(MDI::windowRef, (LPTSTR)&buf[0], size);
	
	returnValue.setUTF16String((const PA_Unichar *)&buf[0], size);
	returnValue.setReturn(pResult);
#endif 
	returnValue.setReturn(pResult);
}

void MDI_SET_TITLE(sLONG_PTR *pResult, PackagePtr pParams)
{
#if VERSIONWIN
	C_TEXT Param1;

	Param1.fromParamAtIndex(pParams, 1);

	SetWindowText(MDI::windowRef, (LPCTSTR)Param1.getUTF16StringPtr());
#endif    
}

void MDI_USE_ICON_FILE(sLONG_PTR *pResult, PackagePtr pParams)
{
#if VERSIONWIN
	C_TEXT Param1;

	Param1.fromParamAtIndex(pParams, 1);

	SHFILEINFO fileinfo;
	
	if(SHGetFileInfo((LPCTSTR)Param1.getUTF16StringPtr(), 
					 0,
					 &fileinfo, 
					 sizeof(fileinfo), 
					 SHGFI_SMALLICON|SHGFI_ICON)){
		
		HICON smallIcon = fileinfo.hIcon;
		SendMessage(MDI::windowRef, WM_SETICON, ICON_SMALL, (LPARAM)smallIcon);
		DestroyIcon(smallIcon);
        
        if(SHGetFileInfo((LPCTSTR)Param1.getUTF16StringPtr(), 
					 0,
					 &fileinfo, 
					 sizeof(fileinfo), 
					 SHGFI_LARGEICON|SHGFI_ICON)){
		
            HICON largeIcon = fileinfo.hIcon;
			SendMessage(MDI::windowRef, WM_SETICON, ICON_BIG, (LPARAM)largeIcon);
            DestroyIcon(largeIcon);
        
        }
	}
#endif    
}

void MDI_MAXIMIZE(sLONG_PTR *pResult, PackagePtr pParams)
{
#if VERSIONWIN
	ShowWindow(MDI::windowRef, SW_MAXIMIZE);
#endif    
}

void MDI_MINIMIZE(sLONG_PTR *pResult, PackagePtr pParams)
{
#if VERSIONWIN
	ShowWindow(MDI::windowRef, SW_MINIMIZE);
#endif    
}

void MDI_SET_CLOSE_BOX_ENABLED(sLONG_PTR *pResult, PackagePtr pParams)
{
#if VERSIONWIN
	C_LONGINT Param1;

	Param1.fromParamAtIndex(pParams, 1);
	if(Param1.getIntValue()){
		EnableMenuItem(GetSystemMenu(MDI::windowRef, FALSE), SC_CLOSE, MF_BYCOMMAND|MF_ENABLED);		
	}else{
        EnableMenuItem(GetSystemMenu(MDI::windowRef, FALSE), SC_CLOSE, MF_BYCOMMAND|MF_GRAYED);
	}
#endif    
}

void MDI_Get_close_box_enabled(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT returnValue;
#if VERSIONWIN
	UINT state = GetMenuState(GetSystemMenu(MDI::windowRef, FALSE), SC_CLOSE, MF_BYCOMMAND);
	
	returnValue.setIntValue(!(state & (MF_DISABLED|MF_GRAYED)));
#endif    
	returnValue.setReturn(pResult);
}

void MDI_RESTORE(sLONG_PTR *pResult, PackagePtr pParams)
{
#if VERSIONWIN
	ShowWindow(MDI::windowRef, SW_RESTORE);
#endif
}

void MDI_SET_POSITION(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT insertAfter;
	C_LONGINT x;
	C_LONGINT y;
	C_LONGINT w;
	C_LONGINT h;
	C_LONGINT flags;
#if VERSIONWIN	
	insertAfter.fromParamAtIndex(pParams, 1);
	x.fromParamAtIndex(pParams, 2);
	y.fromParamAtIndex(pParams, 3);
	w.fromParamAtIndex(pParams, 4);
	h.fromParamAtIndex(pParams, 5);
	flags.fromParamAtIndex(pParams, 6);
	
    HWND hWndInsertAfter = (HWND)insertAfter.getIntValue();
    
    if((hWndInsertAfter != HWND_BOTTOM)
    && (hWndInsertAfter != HWND_NOTOPMOST)
    && (hWndInsertAfter != HWND_TOP)
    && (hWndInsertAfter != HWND_TOPMOST)){
        //remember, since v1264bits/v14, 4D windowRef is not HWND
       hWndInsertAfter = (HWND)PA_GetHWND((PA_WindowRef)hWndInsertAfter); 
    }

	SetWindowPos(MDI::windowRef, 
				 hWndInsertAfter, 
				 x.getIntValue(), 
				 y.getIntValue(), 
				 w.getIntValue(), 
				 h.getIntValue(), 
				 flags.getIntValue());
#endif                 
}

void MDI_GET_POSITION(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT Param2;
	C_LONGINT Param3;
	C_LONGINT Param4;
#if VERSIONWIN
	RECT rect;
	
	if(GetWindowRect(MDI::windowRef, &rect))
	{
		Param1.setIntValue(rect.left);
		Param2.setIntValue(rect.top);
		Param3.setIntValue(rect.right - rect.left);
		Param4.setIntValue(rect.bottom - rect.top);		
	}
#endif
	Param1.toParamAtIndex(pParams, 1);
	Param2.toParamAtIndex(pParams, 2);
	Param3.toParamAtIndex(pParams, 3);
	Param4.toParamAtIndex(pParams, 4);
}

