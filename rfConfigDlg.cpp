// rfConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "rfConfig.h"
#include "rfConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRfConfigDlg dialog

CRfConfigDlg::CRfConfigDlg(CWnd* pParent /*=NULL*/)
: CDialog(CRfConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRfConfigDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRfConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRfConfigDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRfConfigDlg, CDialog)
//{{AFX_MSG_MAP(CRfConfigDlg)
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRfConfigDlg message handlers

BOOL CRfConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRfConfigDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRfConfigDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CRfConfigDlg::OnOK() 
{
	char *szProcess[] = {"leveldirectory","bitmapdirectory","actordirectory",
		"audiodirectory","audiostreamdirectory","videodirectory",
		"mididirectory","txlsearchpath","previewpath","headers",
		"objectsdir","projectdir","GPreviewPath",NULL};
	int n3DTFilesProcessed = 0;
	
	int nTemp = 0, nTemp2 = 0;
	char szRootDirectory[512] = {""};
	char szCurrentFileName[1024] = {""};
	char szWorkingFileName[1024] = {""};
	char szTemp[512] = {""};
	char szInputString[1024] = {""};
	char szOutputString[1024] = {""};
	char *szAtom = NULL;
	FILE *fdInput = NULL, *fdOutput = NULL;
	
	remove("rfconfig.exe");

	char *pTemp = __argv[1];
	strcpy(szRootDirectory, pTemp);
	
	//	This is where we grab the info from the dialog and PROCESS the various
	//	..files we need to change.
	
	//	Fine, let's parse through the various files.  This is what we do:
	//
	//	#1: Open up a <filename>.TEMP file
	//	#2: Copy/update, a line at a time, the files we're messing with
	//	#3: Close both files.
	//	#4: Delete any pre-existing .BAK file
	//	#5: Rename the original file to a .BAK file
	//	#6: Rename the .TEMP file to the name of the original file!
	//
	//	This preserves the existing files in case Something Really Bad Happens.

	//	Next on the hit list: rfEdit.ini
	
	strcpy(szCurrentFileName, szRootDirectory);
	strcat(szCurrentFileName, "\\tools\\rfEdit.ini");
	strcpy(szWorkingFileName, szCurrentFileName);
	strcat(szWorkingFileName, ".Temp");
	
	if((fdInput = fopen(szCurrentFileName, "rt")) == NULL)
	{
		MessageBox("ERROR - rfEdit.ini VANISHED! SERIOUS FATAL ERROR - EXITING!",
			"RealityFactory Configurator", MB_ICONSTOP | MB_OK);
		CDialog::OnOK();
		return;
	}
	
	if((fdOutput = fopen(szWorkingFileName, "wt")) == NULL)
	{
		MessageBox("ERROR - CAN'T CREATE WORKING FILE (B)! SERIOUS FATAL ERROR - EXITING!",
			"RealityFactory Configurator", MB_ICONSTOP | MB_OK);
		CDialog::OnOK();
		return;
	}
	
	while(fgets(szInputString, 512, fdInput) != NULL)
	{
		// If a comment, copy verbatim
		if(szInputString[0] == ';')
		{
			fputs(szInputString, fdOutput);
			continue;
		}
		// If too short for a command, copy verbatim
		if(strlen(szInputString) <= 5)
		{
			fputs(szInputString, fdOutput);
			continue;
		}
		// Attempt to parse out a commandname
		strcpy(szOutputString, szInputString);
		szAtom = strtok(szOutputString, "=");
		if(szAtom == NULL)
		{
			// No idea what this is, best leave it alone.
			fputs(szInputString, fdOutput);
			continue;
		}
		// Ok, we KNOW there's a command-format entry, let's see if it's something
		// ..we don't need to fool with, and if so, leave it alone
		for(nTemp = 0; szProcess[nTemp] != NULL; nTemp++)
		{
			if(!stricmp(szAtom, szProcess[nTemp]))
				break;
		}
		if(szProcess[nTemp] == NULL)
		{
			// Leave it alone, then
			fputs(szInputString, fdOutput);
			continue;
		}
		// Ok, we have to process whatever this is.
		switch(nTemp)
		{
		case 7:								// TxlSearchPath
			strcpy(szOutputString,"TxlSearchPath=");
			strcat(szOutputString, szRootDirectory);
			strcat(szOutputString, "\\media\\levels\n");
			fputs(szOutputString, fdOutput);
			break;
		case 8:								// PreviewPath
			strcpy(szOutputString,"PreviewPath=");
			strcat(szOutputString, szRootDirectory);
			strcat(szOutputString, "\\RealityFactory.exe\n");
			fputs(szOutputString, fdOutput);
			break;
		case 9:								// Headers
			strcpy(szOutputString,"Headers=");
			strcat(szOutputString, szRootDirectory);
			strcat(szOutputString, "\\source\n");
			fputs(szOutputString, fdOutput);
			break;
		case 10:								// ObjectsDir
			strcpy(szOutputString,"ObjectsDir=");
			strcat(szOutputString, szRootDirectory);
			strcat(szOutputString, "\\media\\objects\n");
			fputs(szOutputString, fdOutput);
			break;
		case 11:								// ProjectDir
			strcpy(szOutputString,"ProjectDir=");
			strcat(szOutputString, szRootDirectory);
			strcat(szOutputString, "\\media\\levels\n");
			fputs(szOutputString, fdOutput);
			break;
		case 12:								// GPreviewPath
			strcpy(szOutputString,"GPreviewPath=");
			strcat(szOutputString, szRootDirectory);
			strcat(szOutputString, "\\RealityFactory.ini\n");
			fputs(szOutputString, fdOutput);
			break;
		}
	}
	
	fclose(fdInput); fclose(fdOutput);			// Clean up a bit.
	
	remove(szCurrentFileName);	
	rename(szWorkingFileName, szCurrentFileName);		// Working file replaces original

	//	Next on the hit list: wrldedit.ini
	
	strcpy(szCurrentFileName, szRootDirectory);
	strcat(szCurrentFileName, "\\tools\\wrldedit.ini");
	strcpy(szWorkingFileName, szCurrentFileName);
	strcat(szWorkingFileName, ".Temp");
	
	if((fdInput = fopen(szCurrentFileName, "rt")) == NULL)
	{
		MessageBox("ERROR - wrldedit.ini VANISHED! SERIOUS FATAL ERROR - EXITING!",
			"RealityFactory Configurator", MB_ICONSTOP | MB_OK);
		CDialog::OnOK();
		return;
	}
	
	if((fdOutput = fopen(szWorkingFileName, "wt")) == NULL)
	{
		MessageBox("ERROR - CAN'T CREATE WORKING FILE (B)! SERIOUS FATAL ERROR - EXITING!",
			"RealityFactory Configurator", MB_ICONSTOP | MB_OK);
		CDialog::OnOK();
		return;
	}
	
	while(fgets(szInputString, 512, fdInput) != NULL)
	{
		// If a comment, copy verbatim
		if(szInputString[0] == ';')
		{
			fputs(szInputString, fdOutput);
			continue;
		}
		// If too short for a command, copy verbatim
		if(strlen(szInputString) <= 5)
		{
			fputs(szInputString, fdOutput);
			continue;
		}
		// Attempt to parse out a commandname
		strcpy(szOutputString, szInputString);
		szAtom = strtok(szOutputString, "=");
		if(szAtom == NULL)
		{
			// No idea what this is, best leave it alone.
			fputs(szInputString, fdOutput);
			continue;
		}
		// Ok, we KNOW there's a command-format entry, let's see if it's something
		// ..we don't need to fool with, and if so, leave it alone
		for(nTemp = 0; szProcess[nTemp] != NULL; nTemp++)
		{
			if(!stricmp(szAtom, szProcess[nTemp]))
				break;
		}
		if(szProcess[nTemp] == NULL)
		{
			// Leave it alone, then
			fputs(szInputString, fdOutput);
			continue;
		}
		// Ok, we have to process whatever this is.
		switch(nTemp)
		{
		case 7:								// TxlSearchPath
			strcpy(szOutputString,"TxlSearchPath=");
			strcat(szOutputString, szRootDirectory);
			strcat(szOutputString, "\\media\\levels\n");
			fputs(szOutputString, fdOutput);
			break;
		case 8:								// PreviewPath
			strcpy(szOutputString,"PreviewPath=");
			strcat(szOutputString, szRootDirectory);
			strcat(szOutputString, "\\RealityFactory.exe\n");
			fputs(szOutputString, fdOutput);
			break;
		case 9:								// Headers
			strcpy(szOutputString,"Headers=");
			strcat(szOutputString, szRootDirectory);
			strcat(szOutputString, "\\source\n");
			fputs(szOutputString, fdOutput);
			break;
		case 10:								// ObjectsDir
			strcpy(szOutputString,"ObjectsDir=");
			strcat(szOutputString, szRootDirectory);
			strcat(szOutputString, "\\media\\objects\n");
			fputs(szOutputString, fdOutput);
			break;
		case 11:								// ProjectDir
			strcpy(szOutputString,"ProjectDir=");
			strcat(szOutputString, szRootDirectory);
			strcat(szOutputString, "\\media\\levels\n");
			fputs(szOutputString, fdOutput);
			break;
		}
	}
	
	fclose(fdInput); fclose(fdOutput);			// Clean up a bit.
	
	remove(szCurrentFileName);	
	rename(szWorkingFileName, szCurrentFileName);		// Working file replaces original
	
	//	Ok, now we start processing .3DT files.
	
	strcpy(szTemp, szRootDirectory);
	strcat(szTemp,"\\media\\levels\\*.3dt");	// Our search pattern
	
	WIN32_FIND_DATA FindData;
	HANDLE fileHandle = FindFirstFile(szTemp, &FindData);
	if(fileHandle != NULL)
	{
		// We have some 3DT files to process.  Let's scan our way through them.
		for(;;)
		{
			n3DTFilesProcessed++;
			// Construct the fully qualified filename
			strcpy(szCurrentFileName, szRootDirectory);
			strcat(szCurrentFileName, "\\media\\levels\\");
			strcat(szCurrentFileName, FindData.cFileName);
			// Fine, now we need to look for TWO LINES: One that starts with
			// ..TextureLib and one that starts with HeadersDir.  Thankfully,
			// ..these are usually at the TOP of the file.
			strcpy(szWorkingFileName, szCurrentFileName);
			strcat(szWorkingFileName, ".Temp");
			if((fdInput = fopen(szCurrentFileName, "rt")) == NULL)
			{
				MessageBox("ERROR - Can't open existing 3DT! SERIOUS FATAL ERROR - EXITING!",
					"RealityFactory Configurator", MB_ICONSTOP | MB_OK);
				CDialog::OnOK();
				return;
			}
			if((fdOutput = fopen(szWorkingFileName, "wt")) == NULL)
			{
				MessageBox("ERROR - CAN'T CREATE WORKING FILE (C)! SERIOUS FATAL ERROR - EXITING!",
					"RealityFactory Configurator", MB_ICONSTOP | MB_OK);
				CDialog::OnOK();
				return;
			}
			while(fgets(szInputString, 512, fdInput) != NULL)
			{
				// Attempt to parse out a commandname
				strcpy(szOutputString, szInputString);
				szAtom = strtok(szOutputString, " ");
				if(szAtom == NULL)
				{
					// No idea what this is, best leave it alone.
					fputs(szInputString, fdOutput);
					continue;
				}
				if(!stricmp("TextureLib",szAtom))
				{
					// Fix up texture library reference
					char *pTemp = strrchr(szInputString,'\\');
					if(pTemp == NULL)
					{
						// What's there isn't fully qualified, leave it alone
						fputs(szInputString, fdOutput);
						continue;
					}
					else
					{
						char *szTerm = strrchr(szInputString,'\"');
						if(szTerm != NULL)
							*szTerm = 0;								// Truncate at double-quote
						// Pathname qualified, let's replace the path.
						strcpy(szTemp, pTemp+1);			// Filename + closing quote
						strcpy(szOutputString,"TextureLib \"");
						strcat(szOutputString, szRootDirectory);
						strcat(szOutputString, "\\media\\levels\\");
						strcat(szOutputString, szTemp);
						strcat(szOutputString, "\"\n");
						for(nTemp = nTemp2 = 0; szOutputString[nTemp] != 0; nTemp++)
						{
							if(szOutputString[nTemp] != '\\')
								szInputString[nTemp2++] = szOutputString[nTemp];
							else
							{
								szInputString[nTemp2++] = '\\';
								szInputString[nTemp2++] = '\\';
							}
						}
						szInputString[nTemp2] = 0;
						strcpy(szOutputString, szInputString);
						fputs(szOutputString, fdOutput);
					}
				}
				else if(!stricmp("HeadersDir",szAtom))
				{
					// Fix up headers directory reference
					strcpy(szOutputString,"HeadersDir \"");
					strcat(szOutputString, szRootDirectory);
					strcat(szOutputString, "\\source\\");
					strcat(szOutputString, "\"\n");
					for(nTemp = nTemp2 = 0; szOutputString[nTemp] != 0; nTemp++)
					{
						if(szOutputString[nTemp] != '\\')
							szInputString[nTemp2++] = szOutputString[nTemp];
						else
						{
							szInputString[nTemp2++] = '\\';
							szInputString[nTemp2++] = '\\';
						}
					}
					szInputString[nTemp2] = 0;
					strcpy(szOutputString, szInputString);
					fputs(szOutputString, fdOutput);
				}
				else
				{
					fputs(szInputString, fdOutput);
					continue;
				}
			}
			fclose(fdInput); fclose(fdOutput);			// Clean up a bit.
			
			remove(szCurrentFileName);	
			rename(szWorkingFileName, szCurrentFileName);		// Working file replaces original
			
			if(FindNextFile(fileHandle, &FindData) == 0)
				break;					// Out of files that match
			}
		}
		
		//	Ok, we did it SUCCESSFULLY!  Inform the user and then exit the
		//	..program.
		
		char szMsg[512];
		sprintf(szMsg,"Processing complete!  INI files updated, %d .3DT files updated.",
			n3DTFilesProcessed);
		MessageBox(szMsg, "RealityFactory Configurator", MB_OK);
		
		CDialog::OnOK();
}
