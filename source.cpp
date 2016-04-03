#include <Windows.h>
#include <wrl\client.h>
#include <d3d9.h>
#include <d3d10_1.h>
#include <d3d11.h>
#include <d3d12.h>

#pragma comment (lib, "d3d9.lib")

using namespace Microsoft::WRL;

typedef HRESULT(WINAPI* _D3D10CreateDevice1)(
	_In_ IDXGIAdapter *pAdapter,
	_In_ D3D10_DRIVER_TYPE DriverType,
	_In_ HMODULE Software,
	_In_ UINT Flags,
	_In_ D3D10_FEATURE_LEVEL1 HardwareLevel,
	_In_ UINT SDKVersion,
	_Out_ ID3D10Device1 **ppDevice);
_D3D10CreateDevice1 D3D10CreateDevice1_;

typedef HRESULT(WINAPI* _D3D11CreateDevice)(
	_In_opt_ IDXGIAdapter *pAdapter,
	D3D_DRIVER_TYPE DriverType,
	HMODULE Software,
	UINT Flags,
	_In_opt_ const D3D_FEATURE_LEVEL *pFeatureLevels,
	UINT FeatureLevels,
	UINT SDKVersion,
	_Out_opt_ ID3D11Device **ppDevice,
	_Out_opt_ D3D_FEATURE_LEVEL *pFeatureLevel,
	_Out_opt_ ID3D11DeviceContext **ppImmediateContext);
_D3D11CreateDevice D3D11CreateDevice_;

typedef HRESULT(WINAPI* _D3D12CreateDevice)(
	_In_opt_ IUnknown *pAdapter,
	D3D_FEATURE_LEVEL MinimumFeatureLevel,
	_In_ REFIID riid,
	_Out_opt_ void **ppDevice);
_D3D12CreateDevice D3D12CreateDevice_;

ComPtr<IDirect3D9> d3d;
ComPtr<ID3D10Device1> device_10_1_0;
ComPtr<ID3D10Device1> device_10_1_1;
ComPtr<ID3D11Device> device_11_0;
ComPtr<ID3D11Device> device_11_0_1;
ComPtr<ID3D12Device> device_12_0;
ComPtr<ID3D12Device> device_12_0_1;
ComPtr<ID3D11DeviceContext> context_11_0;
ComPtr<ID3D11DeviceContext> context_11_0_1;

ComPtr<ID3D11RenderTargetView> renderTarget;

HMODULE hMod_10_1 = 0;
HMODULE hMod_11_0 = 0;
HMODULE hMod_12_0 = 0;

HRESULT hr_10_1_0;
HRESULT hr_10_1_1;
HRESULT hr_11_0;
HRESULT hr_11_0_1;
HRESULT hr_12_0;
HRESULT hr_12_0_1;

void Clear()
{
	device_10_1_0.ReleaseAndGetAddressOf();
	device_10_1_1.ReleaseAndGetAddressOf();
	device_11_0.ReleaseAndGetAddressOf();
	device_11_0_1.ReleaseAndGetAddressOf();
	device_12_0_1.ReleaseAndGetAddressOf();
	context_11_0.ReleaseAndGetAddressOf();
	context_11_0_1.ReleaseAndGetAddressOf();
	renderTarget.ReleaseAndGetAddressOf();
}

void Init(HWND hWnd)
{
	hMod_10_1 = LoadLibrary("d3d10_1.dll");
	hMod_11_0 = LoadLibrary("d3d11.dll");
	hMod_12_0 = LoadLibrary("d3d12.dll");

	if (hMod_12_0 == NULL)
	{
		D3D10CreateDevice1_ = (_D3D10CreateDevice1)GetProcAddress(hMod_10_1, "D3D10CreateDevice1");
		D3D11CreateDevice_ = (_D3D11CreateDevice)GetProcAddress(hMod_11_0, "D3D11CreateDevice");

		D3D10_FEATURE_LEVEL1 feat_10_1_0 = D3D10_FEATURE_LEVEL_10_0;
		hr_10_1_0 = D3D10CreateDevice1_(nullptr, D3D10_DRIVER_TYPE_HARDWARE, nullptr,
			D3D10_CREATE_DEVICE_SINGLETHREADED, feat_10_1_0, D3D10_1_SDK_VERSION, &device_10_1_0);

		D3D10_FEATURE_LEVEL1 feat_10_1_1 = D3D10_FEATURE_LEVEL_10_1;
		hr_10_1_1 = D3D10CreateDevice1_(nullptr, D3D10_DRIVER_TYPE_HARDWARE, nullptr,
			D3D10_CREATE_DEVICE_SINGLETHREADED, feat_10_1_1, D3D10_1_SDK_VERSION, &device_10_1_1);

		D3D_FEATURE_LEVEL feat_11_0 = D3D_FEATURE_LEVEL_11_0;
		hr_11_0 = D3D11CreateDevice_(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			D3D11_CREATE_DEVICE_SINGLETHREADED, &feat_11_0, 1, D3D11_SDK_VERSION,
			&device_11_0, nullptr, &context_11_0);

		D3D_FEATURE_LEVEL feat_11_1 = D3D_FEATURE_LEVEL_11_1;
		hr_11_0_1 = D3D11CreateDevice_(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			D3D11_CREATE_DEVICE_SINGLETHREADED, &feat_11_1, 1, D3D11_SDK_VERSION,
			&device_11_0_1, nullptr, &context_11_0_1);
	}

	else
	{
		D3D10CreateDevice1_ = (_D3D10CreateDevice1)GetProcAddress(hMod_10_1, "D3D10CreateDevice1");
		D3D11CreateDevice_ = (_D3D11CreateDevice)GetProcAddress(hMod_11_0, "D3D11CreateDevice");
		D3D12CreateDevice_ = (_D3D12CreateDevice)GetProcAddress(hMod_12_0, "D3D12CreateDevice");

		D3D10_FEATURE_LEVEL1 feat_10_1_0 = D3D10_FEATURE_LEVEL_10_0;
		hr_10_1_0 = D3D10CreateDevice1_(nullptr, D3D10_DRIVER_TYPE_HARDWARE, nullptr,
			D3D10_CREATE_DEVICE_SINGLETHREADED, feat_10_1_0, D3D10_1_SDK_VERSION, &device_10_1_0);

		D3D10_FEATURE_LEVEL1 feat_10_1_1 = D3D10_FEATURE_LEVEL_10_1;
		hr_10_1_1 = D3D10CreateDevice1_(nullptr, D3D10_DRIVER_TYPE_HARDWARE, nullptr,
			D3D10_CREATE_DEVICE_SINGLETHREADED, feat_10_1_1, D3D10_1_SDK_VERSION, &device_10_1_1);

		D3D_FEATURE_LEVEL feat_11_0 = D3D_FEATURE_LEVEL_11_0;
		hr_11_0 = D3D11CreateDevice_(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			D3D11_CREATE_DEVICE_SINGLETHREADED, &feat_11_0, 1, D3D11_SDK_VERSION,
			&device_11_0, nullptr, &context_11_0);

		D3D_FEATURE_LEVEL feat_11_1 = D3D_FEATURE_LEVEL_11_1;
		hr_11_0_1 = D3D11CreateDevice_(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			D3D11_CREATE_DEVICE_SINGLETHREADED, &feat_11_1, 1, D3D11_SDK_VERSION,
			&device_11_0_1, nullptr, &context_11_0_1);

		hr_12_0 = D3D12CreateDevice_(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device_12_0));
		device_12_0.ReleaseAndGetAddressOf();

		hr_12_0_1 = D3D12CreateDevice_(nullptr, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&device_12_0_1));
	}
}

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	HWND hWnd = CreateWindowEx(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, hInstance, 0);
	Init(hWnd);

	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	D3DADAPTER_IDENTIFIER9 GPUName;
	d3d->GetAdapterIdentifier(0, 0, &GPUName);
	LPCSTR GPU_Name = GPUName.Description;

	if (hMod_12_0 == NULL)
	{
		if (SUCCEEDED(hr_10_1_0) && SUCCEEDED(hr_10_1_1) &&
			SUCCEEDED(hr_11_0) && SUCCEEDED(hr_11_0_1))
		{
			MessageBox(NULL,
				"DirectX 10 - Support\nDirectX 10.1 - Support\nDirectX 11 - Support\nDirectX 12 - Support\n\n"
				"D3D10_FEATURE_LEVEL_10_0 - Support\nD3D10_FEATURE_LEVEL_10_1 - Support\n\n"
				"D3D_FEATURE_LEVEL_11_0 - Support\nD3D_FEATURE_LEVEL_11_1 - Support\n\n"
				"D3D_FEATURE_LEVEL_12_0 - Unknown (need Windows 10)\nD3D_FEATURE_LEVEL_12_1 - Unknown (need Windows 10)",
				GPU_Name, NULL);
		}

		else if (SUCCEEDED(hr_10_1_0) && SUCCEEDED(hr_10_1_1) &&
			SUCCEEDED(hr_11_0) && FAILED(hr_11_0_1))
		{
			MessageBox(NULL,
				"DirectX 10 - Support\nDirectX 10.1 - Support\nDirectX 11 - Support\nDirectX 12 - Support\n\n"
				"D3D10_FEATURE_LEVEL_10_0 - Support\nD3D10_FEATURE_LEVEL_10_1 - Support\n\n"
				"D3D_FEATURE_LEVEL_11_0 - Support\nD3D_FEATURE_LEVEL_11_1 - Not support\n\n"
				"D3D_FEATURE_LEVEL_12_0 - Not support\nD3D_FEATURE_LEVEL_12_1 - Not support",
				GPU_Name, NULL);
		}

		else if (SUCCEEDED(hr_10_1_0) && SUCCEEDED(hr_10_1_1) &&
			FAILED(hr_11_0) && FAILED(hr_11_0_1))
		{
			MessageBox(NULL,
				"DirectX 10 - Support\nDirectX 10.1 - Support\nDirectX 11 - Not support\nDirectX 12 - Not support\n\n"
				"D3D10_FEATURE_LEVEL_10_0 - Support\nD3D10_FEATURE_LEVEL_10_1 - Support\n\n"
				"D3D_FEATURE_LEVEL_11_0 - Not support\nD3D_FEATURE_LEVEL_11_1 - Not support\n\n"
				"D3D_FEATURE_LEVEL_12_0 - Not support\nD3D_FEATURE_LEVEL_12_1 - Not support",
				GPU_Name, NULL);
		}

		else if (SUCCEEDED(hr_10_1_0) && FAILED(hr_10_1_1) &&
			FAILED(hr_11_0) && FAILED(hr_11_0_1))
		{
			MessageBox(NULL,
				"DirectX 10 - Support\nDirectX 10.1 - Not support\nDirectX 11 - Not support\nDirectX 12 - Not support\n\n"
				"D3D10_FEATURE_LEVEL_10_0 - Support\nD3D10_FEATURE_LEVEL_10_1 - Not support\n\n"
				"D3D_FEATURE_LEVEL_11_0 - Not support\nD3D_FEATURE_LEVEL_11_1 - Not support\n\n"
				"D3D_FEATURE_LEVEL_12_0 - Not support\nD3D_FEATURE_LEVEL_12_1 - Not support",
				GPU_Name, NULL);
		}

		else
		{
			MessageBox(NULL, "Something went wrong", GPU_Name, NULL);
		}

		FreeLibrary(hMod_10_1);
		FreeLibrary(hMod_11_0);
	}

	else
	{
		if (SUCCEEDED(hr_10_1_0) && SUCCEEDED(hr_10_1_1) &&
			SUCCEEDED(hr_11_0) && SUCCEEDED(hr_11_0_1) &&
			SUCCEEDED(hr_12_0) && SUCCEEDED(hr_12_0_1))
		{
			MessageBox(NULL,
				"DirectX 10 - Support\nDirectX 10.1 - Support\nDirectX 11 - Support\nDirectX 12 - Support\n\n"
				"D3D10_FEATURE_LEVEL_10_0 - Support\nD3D10_FEATURE_LEVEL_10_1 - Support\n\n"
				"D3D_FEATURE_LEVEL_11_0 - Support\nD3D_FEATURE_LEVEL_11_1 - Support\n\n"
				"D3D_FEATURE_LEVEL_12_0 - Support\nD3D_FEATURE_LEVEL_12_1 - Support",
				GPU_Name, NULL);
		}

		else if (SUCCEEDED(hr_10_1_0) && SUCCEEDED(hr_10_1_1) &&
			SUCCEEDED(hr_11_0) && SUCCEEDED(hr_11_0_1) &&
			SUCCEEDED(hr_12_0) && FAILED(hr_12_0_1))
		{
			MessageBox(NULL,
				"DirectX 10 - Support\nDirectX 10.1 - Support\nDirectX 11 - Support\nDirectX 12 - Support\n\n"
				"D3D10_FEATURE_LEVEL_10_0 - Support\nD3D10_FEATURE_LEVEL_10_1 - Support\n\n"
				"D3D_FEATURE_LEVEL_11_0 - Support\nD3D_FEATURE_LEVEL_11_1 - Support\n\n"
				"D3D_FEATURE_LEVEL_12_0 - Support\nD3D_FEATURE_LEVEL_12_1 - Not support",
				GPU_Name, NULL);
		}

		else if (SUCCEEDED(hr_10_1_0) && SUCCEEDED(hr_10_1_1) &&
			SUCCEEDED(hr_11_0) && SUCCEEDED(hr_11_0_1) &&
			FAILED(hr_12_0) && FAILED(hr_12_0_1))
		{
			MessageBox(NULL,
				"DirectX 10 - Support\nDirectX 10.1 - Support\nDirectX 11 - Support\nDirectX 12 - Support\n\n"
				"D3D10_FEATURE_LEVEL_10_0 - Support\nD3D10_FEATURE_LEVEL_10_1 - Support\n\n"
				"D3D_FEATURE_LEVEL_11_0 - Support\nD3D_FEATURE_LEVEL_11_1 - Support\n\n"
				"D3D_FEATURE_LEVEL_12_0 - Not support\nD3D_FEATURE_LEVEL_12_1 - Not support",
				GPU_Name, NULL);
		}

		else if (SUCCEEDED(hr_10_1_0) && SUCCEEDED(hr_10_1_1) &&
			SUCCEEDED(hr_11_0) && FAILED(hr_11_0_1) &&
			FAILED(hr_12_0) && FAILED(hr_12_0_1))
		{
			MessageBox(NULL,
				"DirectX 10 - Support\nDirectX 10.1 - Support\nDirectX 11 - Support\nDirectX 12 - Support\n\n"
				"D3D10_FEATURE_LEVEL_10_0 - Support\nD3D10_FEATURE_LEVEL_10_1 - Support\n\n"
				"D3D_FEATURE_LEVEL_11_0 - Support\nD3D_FEATURE_LEVEL_11_1 - Not support\n\n"
				"D3D_FEATURE_LEVEL_12_0 - Not support\nD3D_FEATURE_LEVEL_12_1 - Not support",
				GPU_Name, NULL);
		}

		else if (SUCCEEDED(hr_10_1_0) && SUCCEEDED(hr_10_1_1) &&
			FAILED(hr_11_0) && FAILED(hr_11_0_1) &&
			FAILED(hr_12_0) && FAILED(hr_12_0_1))
		{
			MessageBox(NULL,
				"DirectX 10 - Support\nDirectX 10.1 - Support\nDirectX 11 - Not support\nDirectX 12 - Not support\n\n"
				"D3D10_FEATURE_LEVEL_10_0 - Support\nD3D10_FEATURE_LEVEL_10_1 - Support\n\n"
				"D3D_FEATURE_LEVEL_11_0 - Not support\nD3D_FEATURE_LEVEL_11_1 - Not support\n\n"
				"D3D_FEATURE_LEVEL_12_0 - Not support\nD3D_FEATURE_LEVEL_12_1 - Not support",
				GPU_Name, NULL);
		}

		else if (SUCCEEDED(hr_10_1_0) && FAILED(hr_10_1_1) &&
			FAILED(hr_11_0) && FAILED(hr_11_0_1) &&
			FAILED(hr_12_0) && FAILED(hr_12_0_1))
		{
			MessageBox(NULL,
				"DirectX 10 - Support\nDirectX 10.1 - Not support\nDirectX 11 - Not support\nDirectX 12 - Not support\n\n"
				"D3D10_FEATURE_LEVEL_10_0 - Support\nD3D10_FEATURE_LEVEL_10_1 - Not support\n\n"
				"D3D_FEATURE_LEVEL_11_0 - Not support\nD3D_FEATURE_LEVEL_11_1 - Not support\n\n"
				"D3D_FEATURE_LEVEL_12_0 - Not support\nD3D_FEATURE_LEVEL_12_1 - Not support",
				GPU_Name, NULL);
		}

		else
		{
			MessageBox(NULL, "Something went wrong", GPU_Name, NULL);
		}
	}

	Clear();
	FreeLibrary(hMod_10_1);
	FreeLibrary(hMod_11_0);
	FreeLibrary(hMod_12_0);
	return 0;
}
