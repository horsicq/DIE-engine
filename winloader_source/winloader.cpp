// Copyright (c) 2018-2019 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#include <windows.h>

void GetWorkDirectory(char *pszModuleName,char *pszWorkDirectory)
{
    int nTotalNumberOfSlashes=0;
    char *pOffset=pszModuleName;
    
    while(*pOffset)
    {
        if(*pOffset=='\\')
        {
            nTotalNumberOfSlashes++;
        }
        pOffset++;
    }
    
    int nCurrentNumberOfSlashes=0;
    
    while(*pszModuleName)
    {
        if(*pszModuleName=='\\')
        {
            nCurrentNumberOfSlashes++;
            if(nCurrentNumberOfSlashes==nTotalNumberOfSlashes)
            {
                *pszWorkDirectory=0;
                break;
            }
        }
        *pszWorkDirectory=*pszModuleName;
        pszModuleName++;
        pszWorkDirectory++;
    }
}

void entry()
{
    char szModuleFileName[256];
    char szWorkDirectory[256];
    char szArgument[256];

    char *szFile="\\base\\die.exe";
    char *szStuff="\\base\\";

    char *pszCommandLine=GetCommandLineA();
    GetModuleFileName(0,szModuleFileName,sizeof(szModuleFileName)-1);
    
    int nFileNameLength=lstrlenA(szModuleFileName);
    
    if(*(pszCommandLine+nFileNameLength+2))
    {
        lstrcpyA(szArgument,pszCommandLine+nFileNameLength+3);
    }
    
    GetWorkDirectory(szModuleFileName,szWorkDirectory);
    lstrcpyA(szModuleFileName,szWorkDirectory);
    lstrcatA(szModuleFileName,szFile);
    lstrcatA(szWorkDirectory,szStuff);
    
    if(ShellExecuteA(0,0,szModuleFileName,szArgument,szWorkDirectory,SW_SHOW)<=(HINSTANCE)32)
    {
        MessageBoxA(0,szModuleFileName,"Cannot open",MB_ICONERROR);
    }

    ExitProcess(0);
}