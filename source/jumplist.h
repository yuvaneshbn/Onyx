// jumplist.h
#ifndef jumplist_h__
#define jumplist_h__

#include <QString>

#undef _WIN32_WINNT
#define _WIN32_WINNT_OLD _WIN32_WINNT
#define _WIN32_WINNT _WIN32_WINNT_WIN7
#include <shobjidl.h>
#include <propkey.h>
#include <propvarutil.h>
#undef _WIN32_WINNT 
#define _WIN32_WINNT _WIN32_WINNT_OLD
#undef _WIN32_WINNT_OLD 

class JumpList
{
public:
	JumpList(const QString& AppID);
	~JumpList();
	bool DeleteJumpList();
	void AddTasks();

private:
	HRESULT _CreateShellLink(PCWSTR pszArguments, PCWSTR pszTitle, IShellLinkW **ppsl, int iconindex = -1);
	ICustomDestinationList *pcdl;
};

#endif // jumplist_h__

/*
#ifndef jumplist_h__
#define jumplist_h__

#include <QString>
#include <objectarray.h>
#include <shobjidl.h>
#include <propkey.h>
#include <propvarutil.h>
#include <knownfolders.h>
#include <shlobj.h>
#include <string>
#include <map>

class JumpList
{
public:
	JumpList(const QString& AppID);
	~JumpList();

	HRESULT _CreateShellLink(PCWSTR pszArguments, PCWSTR pszTitle, IShellLink **ppsl, int iconindex, bool WA);
	bool _IsItemInArray(const QString& path, IObjectArray *poaRemoved);
	HRESULT _AddTasksToList();
	HRESULT _AddCategoryToList();
	HRESULT _AddCategoryToList2();
	bool CreateJumpList(const QString& pluginpath, const QString& pref, const QString& fromstart, 
		const QString& resume, const QString& openfile, const QString& bookmarks, const QString& pltext, bool recent, 
		bool frequent, bool tasks, bool addbm, bool playlist, const QString& bms);
	bool DeleteJumpList();
    bool CleanJumpList();

private:
    bool CleanJL(IApplicationDocumentLists *padl, APPDOCLISTTYPE type);

	ICustomDestinationList *pcdl;
	IObjectCollection *poc;
	HRESULT m_hr;
	QString path;
    QString m_AppID;
    QString s1;
    QString s2;
    QString s3;
    QString s4;
    QString s5;
    QString s6;

    const int max_items_jumplist;
};

#endif // jumplist_h__
*/