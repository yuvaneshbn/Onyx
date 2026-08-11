
//-----------------------------------------------------------
// When the status of audio endpoint devices change, the
// MMDevice module calls these methods to notify the client.
//-----------------------------------------------------------

#include <Mmdeviceapi.h>
#include <Dbt.h>
#include "global.h"

#define SAFE_RELEASE(punk)  \
	if ((punk) != NULL)  \
				{ (punk)->Release(); (punk) = NULL; }

class CMMNotificationClient : public IMMNotificationClient
{
	LONG _cRef;
	IMMDeviceEnumerator *_pEnumerator;
	bool hasCallback;
	public:
	CMMNotificationClient() :
		  _cRef(1),
		  _pEnumerator(NULL),
		 hasCallback(false)
	  {
		  HRESULT hr = S_OK;
		  CoInitialize(NULL);
		  if (_pEnumerator == NULL)
		  {
			  // Get enumerator for audio endpoint devices.
			  hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),
				  NULL, CLSCTX_INPROC_SERVER,
				  __uuidof(IMMDeviceEnumerator),
				  (void**)&_pEnumerator);
			  if (hr != S_OK) {
				  _pEnumerator = NULL;
			  } else {
				  if (_pEnumerator->RegisterEndpointNotificationCallback(this) == S_OK) {
					hasCallback = true;
				  }
			  }
		  }
	  }

	  ~CMMNotificationClient()
	  {
		  if (hasCallback) {
			  _pEnumerator->UnregisterEndpointNotificationCallback(this);
		  }
		  SAFE_RELEASE(_pEnumerator)
		  CoUninitialize();
	  }

	  // IUnknown methods -- AddRef, Release, and QueryInterface

	  ULONG STDMETHODCALLTYPE AddRef()
	  {
		  return InterlockedIncrement(&_cRef);
	  }

	  ULONG STDMETHODCALLTYPE Release()
	  {
		  ULONG ulRef = InterlockedDecrement(&_cRef);
		  if (0 == ulRef)
		  {
			  delete this;
		  }
		  return ulRef;
	  }

	  HRESULT STDMETHODCALLTYPE QueryInterface(
		  REFIID riid, VOID **ppvInterface)
	  {
		  if (IID_IUnknown == riid)
		  {
			  AddRef();
			  *ppvInterface = (IUnknown*)this;
		  }
		  else if (__uuidof(IMMNotificationClient) == riid)
		  {
			  AddRef();
			  *ppvInterface = (IMMNotificationClient*)this;
		  }
		  else
		  {
			  *ppvInterface = NULL;
			  return E_NOINTERFACE;
		  }
		  return S_OK;
	  }

	  // Callbacks
	  
	  HRESULT STDMETHODCALLTYPE OnDefaultDeviceChanged(
		  EDataFlow flow, ERole role,
		  LPCWSTR pwstrDeviceId)
	  {
		  OnDeviceStateChanged(pwstrDeviceId, 0);
		  return S_OK;
	  }

	  HRESULT STDMETHODCALLTYPE OnDeviceAdded(LPCWSTR pwstrDeviceId)
	  {
		  return S_OK;
	  };

	  HRESULT STDMETHODCALLTYPE OnDeviceRemoved(LPCWSTR pwstrDeviceId)
	  {
		  return S_OK;
	  }

	  HRESULT STDMETHODCALLTYPE OnDeviceStateChanged(
		  LPCWSTR pwstrDeviceId,
		  DWORD dwNewState)
	  {
		  // Post a WM_DEVICECHANGE notification to the main window via Win32 HWND.
		  // mainDlg is the global Qt main window; retrieve its HWND via winId().
		  extern class CmainDlg* mainDlg;
		  if (mainDlg) {
			  HWND hwnd = reinterpret_cast<HWND>(mainDlg->winId());
			  if (hwnd) {
				  ::PostMessage(hwnd, WM_DEVICECHANGE, DBT_DEVNODES_CHANGED, 1);
			  }
		  }
		  return S_OK;
	  }

	  HRESULT STDMETHODCALLTYPE OnPropertyValueChanged(
		  LPCWSTR pwstrDeviceId,
		  const PROPERTYKEY key)
	  {
		  return S_OK;
	  }
};
