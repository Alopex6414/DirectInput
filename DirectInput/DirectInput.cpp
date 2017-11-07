/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2017, Alopex/Helium
*     All rights reserved.
*
* @file		DirectInput.cpp
* @brief	This Program is DirectInput DLL Project.
* @author	Alopex/Helium
* @version	v1.00a
* @date		2017-10-27
*/
#include "DirectCommon.h"
#include "DirectInput.h"

GUID g_JoyStickGUID;	//JoyStick GUID
char g_cJoyStickName[JOYSTICK_ARRAYSIZE]; //JoyStick Name Array

//------------------------------------------------------------------
// @Function:	 DirectInput()
// @Purpose: DirectInput���캯��
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
DirectInput::DirectInput()
{
	m_pDirectInput = NULL;					//IDirectInput8�ӿڶ���ָ���ʼ��(NULL)
	m_pDirectInputDeviceKeyBoard = NULL;	//IDirectInputDevice8 KeyBoard�����豸ָ���ʼ��(NULL)
	m_pDirectInputDeviceMouse = NULL;		//IDirectInputDevice8 Mouse����豸ָ���ʼ��(NULL)
	m_pDirectInputDeviceJoyStick = NULL;	//IDirectInputDevice8 JoyStick��Ϸ���豸ָ���ʼ��(NULL)
	ZeroMemory(&m_DIMouseState, sizeof(m_DIMouseState));//���MouseState�ڴ�
	ZeroMemory(&m_DIJoyStickState, sizeof(m_DIJoyStickState));//���JoyStick�ڴ�
	ZeroMemory(&m_cKeyBoradBuffer, sizeof(char) * KEYBOARD_ARRAYSIZE);//���KeyBoradBuffer�ڴ�
}

//------------------------------------------------------------------
// @Function:	 ~DirectInput()
// @Purpose: DirectInput��������
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
DirectInput::~DirectInput()
{
	if (m_pDirectInputDeviceMouse != NULL)//Mouse����豸ָ�벻ΪNULL
	{
		m_pDirectInputDeviceMouse->Unacquire();//Mouse����豸�ͷſ���Ȩ
	}
	if (m_pDirectInputDeviceKeyBoard != NULL)//KeyBoard�����豸ָ�벻ΪNULL
	{
		m_pDirectInputDeviceKeyBoard->Unacquire();//KeyBoard�����豸�ͷſ���Ȩ
	}
	if (m_pDirectInputDeviceJoyStick != NULL)//JoyStick��Ϸ���豸ָ�벻ΪNULL
	{
		m_pDirectInputDeviceJoyStick->Unacquire();//JoyStick��Ϸ���豸�ͷſ���Ȩ
	}

	SAFE_RELEASE(m_pDirectInputDeviceMouse);//Mouse����豸�ͷ�
	SAFE_RELEASE(m_pDirectInputDeviceKeyBoard);//KeyBoard�����豸�ͷ�
	SAFE_RELEASE(m_pDirectInputDeviceJoyStick);//JoyStick��Ϸ���豸�ͷ�
	SAFE_RELEASE(m_pDirectInput);//DirectInput�ͷ�
}

//----------------------------------------------------------------------------------------------------
// @Function:	 DirectInputEnumJoySticks(LPCDIDEVICEINSTANCE pDirectDeviceInstance, LPVOID pGUID_Ptr)
// @Purpose: DirectInputö����Ϸ���豸����
// @Since: v1.00a
// @Para: LPCDIDEVICEINSTANCE pDirectDeviceInstance(DirectInput�豸���ָ��)
// @Para: LPVOID pGUID_Ptr(DirectInput�豸ȫ��Ψһ��ʶ���ַ�ָ��)
// @Return: DIENUM_STOP(ֹͣö��)
//----------------------------------------------------------------------------------------------------
BOOL WINAPI DirectInputEnumJoySticks(LPCDIDEVICEINSTANCE pDirectDeviceInstance, LPVOID pGUID_Ptr)
{
	*(GUID*)pGUID_Ptr = pDirectDeviceInstance->guidInstance;
	strcpy_s(g_cJoyStickName, (char*)pDirectDeviceInstance->tszProductName);
	return (DIENUM_STOP);
}

//------------------------------------------------------------------------
// @Function:	DirectInputInit(HWND hWnd, HINSTANCE hInstance)
// @Purpose: DirectInput ��ʼ�������豸(����/���)
// @Since: v1.00a
// @Para: HWND hWnd(���ھ��)
// @Para: HINSTANCE hInstance(����ʵ�����)
// @Return: HRESULT(��ʼ��״̬:�ɹ�:S_OK,ʧ��:E_FAIL)
//------------------------------------------------------------------------
HRESULT WINAPI DirectInput::DirectInputInit(HWND hWnd, HINSTANCE hInstance)
{
	//����IDirectInput8�ӿڶ���
	VERIFY(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL));

	//����KeyBoard�����豸
	VERIFY(m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pDirectInputDeviceKeyBoard, NULL));//����IDirectInputDevice8�ӿڶ���(KeyBoard�豸����)
	VERIFY(m_pDirectInputDeviceKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE));//DirectInputDevice����Э������
	VERIFY(m_pDirectInputDeviceKeyBoard->SetDataFormat(&c_dfDIKeyboard));//DirectInputDevice�������ݸ�ʽ(��׼���̽ṹ)
	VERIFY(m_pDirectInputDeviceKeyBoard->Acquire());//DirectInputDevice��ȡ�豸����Ȩ
	VERIFY(m_pDirectInputDeviceKeyBoard->Poll());//DirectInputDevice��ѯ�豸

	//Mouse����豸
	VERIFY(m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pDirectInputDeviceMouse, NULL));//����IDirectInputDevice8�ӿڶ���(Mouse����豸����)
	VERIFY(m_pDirectInputDeviceMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE));//DirectInputDevice����Э������
	VERIFY(m_pDirectInputDeviceMouse->SetDataFormat(&c_dfDIMouse));//DirectInputDevice�������ݸ�ʽ(��׼���ṹ)
	VERIFY(m_pDirectInputDeviceMouse->Acquire());//DirectInputDevice��ȡ�豸����Ȩ
	VERIFY(m_pDirectInputDeviceMouse->Poll());//DirectInputDevice��ѯ�豸

	return S_OK;//OK
}

//-----------------------------------------------------------------------------------------------------------------------------------------
// @Function:	DirectInputInit(HWND hWnd, HINSTANCE hInstance, DirectInputDevice eDirectInputDevice_X, DWORD dwDeviceCoopFlags)
// @Purpose: DirectInput ��ʼ�������豸(����/���/��Ϸ��)
// @Since: v1.00a
// @Para: HWND hWnd(���ھ��)
// @Para: HINSTANCE hInstance(����ʵ�����)
// @Para: DirectInputDevice eDirectInputDevice_X(ö������:����/���/��Ϸ��)
// @Para: DWORD dwDeviceCoopFlags(�豸Э������)
// @Return: HRESULT(��ʼ��״̬:�ɹ�:S_OK,ʧ��:E_FAIL)
//------------------------------------------------------------------------------------------------------------------------------------------
HRESULT WINAPI DirectInput::DirectInputInit(HWND hWnd, HINSTANCE hInstance, DirectInputDevice eDirectInputDevice_X, DWORD dwDeviceCoopFlags)
{
	//����IDirectInput8�ӿڶ���
	VERIFY(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL));

	//����IDirectInputDevice8�豸�ӿڶ���
	switch (eDirectInputDevice_X)
	{
	case DirectInputDevice_KeyBoard:
		//KeyBoard�����豸
		VERIFY(m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pDirectInputDeviceKeyBoard, NULL));//����IDirectInputDevice8�ӿڶ���(KeyBoard�豸����)
		VERIFY(m_pDirectInputDeviceKeyBoard->SetCooperativeLevel(hWnd, dwDeviceCoopFlags));//DirectInputDevice����Э������
		VERIFY(m_pDirectInputDeviceKeyBoard->SetDataFormat(&c_dfDIKeyboard));//DirectInputDevice�������ݸ�ʽ(��׼���̽ṹ)
		VERIFY(m_pDirectInputDeviceKeyBoard->Acquire());//DirectInputDevice��ȡ�豸����Ȩ
		VERIFY(m_pDirectInputDeviceKeyBoard->Poll());//DirectInputDevice��ѯ�豸
		break;
	case DirectInputDevice_Mouse:
		//Mouse����豸
		VERIFY(m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pDirectInputDeviceMouse, NULL));//����IDirectInputDevice8�ӿڶ���(Mouse����豸����)
		VERIFY(m_pDirectInputDeviceMouse->SetCooperativeLevel(hWnd, dwDeviceCoopFlags));//DirectInputDevice����Э������
		VERIFY(m_pDirectInputDeviceMouse->SetDataFormat(&c_dfDIMouse));//DirectInputDevice�������ݸ�ʽ(��׼���ṹ)
		VERIFY(m_pDirectInputDeviceMouse->Acquire());//DirectInputDevice��ȡ�豸����Ȩ
		VERIFY(m_pDirectInputDeviceMouse->Poll());//DirectInputDevice��ѯ�豸
		break;
	case DirectInputDevice_JoyStick:
		//JoyStick��Ϸ���豸
		VERIFY(m_pDirectInput->EnumDevices(DI8DEVTYPE_JOYSTICK, DirectInputEnumJoySticks, &g_JoyStickGUID, DIEDFL_ATTACHEDONLY));//ö��JoyStick��Ϸ���豸GUID
		VERIFY(m_pDirectInput->CreateDevice(g_JoyStickGUID, &m_pDirectInputDeviceJoyStick, NULL));//����IDirectInputDevice8�ӿڶ���(JoyStick��Ϸ���豸����)
		VERIFY(m_pDirectInputDeviceJoyStick->SetCooperativeLevel(hWnd, dwDeviceCoopFlags));//DirectInputDevice����Э������
		VERIFY(m_pDirectInputDeviceJoyStick->SetDataFormat(&c_dfDIJoystick));//DirectInputDevice�������ݸ�ʽ(��׼��Ϸ�˽ṹ)
		VERIFY(m_pDirectInputDeviceJoyStick->Acquire());//DirectInputDevice��ȡ�豸����Ȩ
		VERIFY(m_pDirectInputDeviceJoyStick->Poll());//DirectInputDevice��ѯ�豸
		break;
	default:
		break;
	}

	return S_OK;//OK
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------
// @Function:	DirectInputInit(HWND hWnd, HINSTANCE hInstance, DirectInputMulDevice eDirectInputMulDevice_X, DWORD dwDeviceCoopFlags1, DWORD dwDeviceCoopFlags2)
// @Purpose: DirectInput ��ʼ�������豸(����/���/��Ϸ��)
// @Since: v1.00a
// @Para: HWND hWnd(���ھ��)
// @Para: HINSTANCE hInstance(����ʵ�����)
// @Para: DirectInputMulDevice eDirectInputMulDevice_X(ö������:����/���/��Ϸ��)
// @Para: DWORD dwDeviceCoopFlags1(�豸1Э������)
// @Para: DWORD dwDeviceCoopFlags2(�豸2Э������)
// @Return: HRESULT(��ʼ��״̬:�ɹ�:S_OK,ʧ��:E_FAIL)
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT WINAPI DirectInput::DirectInputInit(HWND hWnd, HINSTANCE hInstance, DirectInputMulDevice eDirectInputMulDevice_X, DWORD dwDeviceCoopFlags1, DWORD dwDeviceCoopFlags2)
{
	//����IDirectInput8�ӿڶ���
	VERIFY(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL));

	//����IDirectInputDevice8�豸�ӿڶ���
	switch (eDirectInputMulDevice_X)
	{
	case DirectInputDevice_KeyBoard_Mouse:
		//KeyBoard�����豸
		VERIFY(m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pDirectInputDeviceKeyBoard, NULL));//����IDirectInputDevice8�ӿڶ���(KeyBoard�豸����)
		VERIFY(m_pDirectInputDeviceKeyBoard->SetCooperativeLevel(hWnd, dwDeviceCoopFlags1));//DirectInputDevice����Э������
		VERIFY(m_pDirectInputDeviceKeyBoard->SetDataFormat(&c_dfDIKeyboard));//DirectInputDevice�������ݸ�ʽ(��׼���̽ṹ)
		VERIFY(m_pDirectInputDeviceKeyBoard->Acquire());//DirectInputDevice��ȡ�豸����Ȩ
		VERIFY(m_pDirectInputDeviceKeyBoard->Poll());//DirectInputDevice��ѯ�豸
		//Mouse����豸
		VERIFY(m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pDirectInputDeviceMouse, NULL));//����IDirectInputDevice8�ӿڶ���(Mouse����豸����)
		VERIFY(m_pDirectInputDeviceMouse->SetCooperativeLevel(hWnd, dwDeviceCoopFlags2));//DirectInputDevice����Э������
		VERIFY(m_pDirectInputDeviceMouse->SetDataFormat(&c_dfDIMouse));//DirectInputDevice�������ݸ�ʽ(��׼���ṹ)
		VERIFY(m_pDirectInputDeviceMouse->Acquire());//DirectInputDevice��ȡ�豸����Ȩ
		VERIFY(m_pDirectInputDeviceMouse->Poll());//DirectInputDevice��ѯ�豸
		break;
	case DirectInputDevice_KeyBoard_JoyStick:
		//KeyBoard�����豸
		VERIFY(m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pDirectInputDeviceKeyBoard, NULL));//����IDirectInputDevice8�ӿڶ���(KeyBoard�豸����)
		VERIFY(m_pDirectInputDeviceKeyBoard->SetCooperativeLevel(hWnd, dwDeviceCoopFlags1));//DirectInputDevice����Э������
		VERIFY(m_pDirectInputDeviceKeyBoard->SetDataFormat(&c_dfDIKeyboard));//DirectInputDevice�������ݸ�ʽ(��׼���̽ṹ)
		VERIFY(m_pDirectInputDeviceKeyBoard->Acquire());//DirectInputDevice��ȡ�豸����Ȩ
		VERIFY(m_pDirectInputDeviceKeyBoard->Poll());//DirectInputDevice��ѯ�豸
		//JoyStick��Ϸ���豸
		VERIFY(m_pDirectInput->EnumDevices(DI8DEVTYPE_JOYSTICK, DirectInputEnumJoySticks, &g_JoyStickGUID, DIEDFL_ATTACHEDONLY));//ö��JoyStick��Ϸ���豸GUID
		VERIFY(m_pDirectInput->CreateDevice(g_JoyStickGUID, &m_pDirectInputDeviceJoyStick, NULL));//����IDirectInputDevice8�ӿڶ���(JoyStick��Ϸ���豸����)
		VERIFY(m_pDirectInputDeviceJoyStick->SetCooperativeLevel(hWnd, dwDeviceCoopFlags2));//DirectInputDevice����Э������
		VERIFY(m_pDirectInputDeviceJoyStick->SetDataFormat(&c_dfDIJoystick));//DirectInputDevice�������ݸ�ʽ(��׼��Ϸ�˽ṹ)
		VERIFY(m_pDirectInputDeviceJoyStick->Acquire());//DirectInputDevice��ȡ�豸����Ȩ
		VERIFY(m_pDirectInputDeviceJoyStick->Poll());//DirectInputDevice��ѯ�豸
		break;
	case DirectInputDevice_Mouse_JoyStick:
		//Mouse����豸
		VERIFY(m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pDirectInputDeviceMouse, NULL));//����IDirectInputDevice8�ӿڶ���(Mouse����豸����)
		VERIFY(m_pDirectInputDeviceMouse->SetCooperativeLevel(hWnd, dwDeviceCoopFlags1));//DirectInputDevice����Э������
		VERIFY(m_pDirectInputDeviceMouse->SetDataFormat(&c_dfDIMouse));//DirectInputDevice�������ݸ�ʽ(��׼���ṹ)
		VERIFY(m_pDirectInputDeviceMouse->Acquire());//DirectInputDevice��ȡ�豸����Ȩ
		VERIFY(m_pDirectInputDeviceMouse->Poll());//DirectInputDevice��ѯ�豸
		//JoyStick��Ϸ���豸
		VERIFY(m_pDirectInput->EnumDevices(DI8DEVTYPE_JOYSTICK, DirectInputEnumJoySticks, &g_JoyStickGUID, DIEDFL_ATTACHEDONLY));//ö��JoyStick��Ϸ���豸GUID
		VERIFY(m_pDirectInput->CreateDevice(g_JoyStickGUID, &m_pDirectInputDeviceJoyStick, NULL));//����IDirectInputDevice8�ӿڶ���(JoyStick��Ϸ���豸����)
		VERIFY(m_pDirectInputDeviceJoyStick->SetCooperativeLevel(hWnd, dwDeviceCoopFlags2));//DirectInputDevice����Э������
		VERIFY(m_pDirectInputDeviceJoyStick->SetDataFormat(&c_dfDIJoystick));//DirectInputDevice�������ݸ�ʽ(��׼��Ϸ�˽ṹ)
		VERIFY(m_pDirectInputDeviceJoyStick->Acquire());//DirectInputDevice��ȡ�豸����Ȩ
		VERIFY(m_pDirectInputDeviceJoyStick->Poll());//DirectInputDevice��ѯ�豸
		break;
	default:
		break;
	}

	return S_OK;//OK
}

//------------------------------------------------------------------------
// @Function:	DirectInputKeyBoardInit(HWND hWnd, HINSTANCE hInstance)
// @Purpose: DirectInput ��ʼ�������豸(����)
// @Since: v1.00a
// @Para: HWND hWnd(���ھ��)
// @Para: HINSTANCE hInstance(����ʵ�����)
// @Return: HRESULT(��ʼ��״̬:�ɹ�:S_OK,ʧ��:E_FAIL)
//------------------------------------------------------------------------
HRESULT WINAPI DirectInput::DirectInputKeyBoardInit(HWND hWnd, HINSTANCE hInstance)
{
	//����IDirectInput8�ӿڶ���
	VERIFY(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL));

	//����KeyBoard�����豸
	VERIFY(m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pDirectInputDeviceKeyBoard, NULL));//����IDirectInputDevice8�ӿڶ���(KeyBoard�豸����)
	VERIFY(m_pDirectInputDeviceKeyBoard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE));//DirectInputDevice����Э������
	VERIFY(m_pDirectInputDeviceKeyBoard->SetDataFormat(&c_dfDIKeyboard));//DirectInputDevice�������ݸ�ʽ(��׼���̽ṹ)
	VERIFY(m_pDirectInputDeviceKeyBoard->Acquire());//DirectInputDevice��ȡ�豸����Ȩ
	VERIFY(m_pDirectInputDeviceKeyBoard->Poll());//DirectInputDevice��ѯ�豸

	return S_OK;//OK
}

//----------------------------------------------------------------------------------------------
// @Function:	DirectInputKeyBoardInit(HWND hWnd, HINSTANCE hInstance, DWORD dwDeviceCoopFlags)
// @Purpose: DirectInput ��ʼ�������豸(����)
// @Since: v1.00a
// @Para: HWND hWnd(���ھ��)
// @Para: HINSTANCE hInstance(����ʵ�����)
// @Para: DWORD dwDeviceCoopFlags(�豸Э������)
// @Return: HRESULT(��ʼ��״̬:�ɹ�:S_OK,ʧ��:E_FAIL)
//----------------------------------------------------------------------------------------------
HRESULT WINAPI DirectInput::DirectInputKeyBoardInit(HWND hWnd, HINSTANCE hInstance, DWORD dwDeviceCoopFlags)
{
	//����IDirectInput8�ӿڶ���
	VERIFY(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL));

	//����KeyBoard�����豸
	VERIFY(m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pDirectInputDeviceKeyBoard, NULL));//����IDirectInputDevice8�ӿڶ���(KeyBoard�豸����)
	VERIFY(m_pDirectInputDeviceKeyBoard->SetCooperativeLevel(hWnd, dwDeviceCoopFlags));//DirectInputDevice����Э������
	VERIFY(m_pDirectInputDeviceKeyBoard->SetDataFormat(&c_dfDIKeyboard));//DirectInputDevice�������ݸ�ʽ(��׼���̽ṹ)
	VERIFY(m_pDirectInputDeviceKeyBoard->Acquire());//DirectInputDevice��ȡ�豸����Ȩ
	VERIFY(m_pDirectInputDeviceKeyBoard->Poll());//DirectInputDevice��ѯ�豸

	return S_OK;//OK
}

//------------------------------------------------------------------------
// @Function:	DirectInputMouseInit(HWND hWnd, HINSTANCE hInstance)
// @Purpose: DirectInput ��ʼ�������豸(���)
// @Since: v1.00a
// @Para: HWND hWnd(���ھ��)
// @Para: HINSTANCE hInstance(����ʵ�����)
// @Return: HRESULT(��ʼ��״̬:�ɹ�:S_OK,ʧ��:E_FAIL)
//------------------------------------------------------------------------
HRESULT WINAPI DirectInput::DirectInputMouseInit(HWND hWnd, HINSTANCE hInstance)
{
	//����IDirectInput8�ӿڶ���
	VERIFY(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL));

	//Mouse����豸
	VERIFY(m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pDirectInputDeviceMouse, NULL));//����IDirectInputDevice8�ӿڶ���(Mouse����豸����)
	VERIFY(m_pDirectInputDeviceMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE));//DirectInputDevice����Э������
	VERIFY(m_pDirectInputDeviceMouse->SetDataFormat(&c_dfDIMouse));//DirectInputDevice�������ݸ�ʽ(��׼���ṹ)
	VERIFY(m_pDirectInputDeviceMouse->Acquire());//DirectInputDevice��ȡ�豸����Ȩ
	VERIFY(m_pDirectInputDeviceMouse->Poll());//DirectInputDevice��ѯ�豸

	return S_OK;//OK
}

//----------------------------------------------------------------------------------------------
// @Function:	DirectInputMouseInit(HWND hWnd, HINSTANCE hInstance, DWORD dwDeviceCoopFlags)
// @Purpose: DirectInput ��ʼ�������豸(���)
// @Since: v1.00a
// @Para: HWND hWnd(���ھ��)
// @Para: HINSTANCE hInstance(����ʵ�����)
// @Para: DWORD dwDeviceCoopFlags(�豸Э������)
// @Return: HRESULT(��ʼ��״̬:�ɹ�:S_OK,ʧ��:E_FAIL)
//----------------------------------------------------------------------------------------------
HRESULT WINAPI DirectInput::DirectInputMouseInit(HWND hWnd, HINSTANCE hInstance, DWORD dwDeviceCoopFlags)
{
	//����IDirectInput8�ӿڶ���
	VERIFY(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL));

	//Mouse����豸
	VERIFY(m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pDirectInputDeviceMouse, NULL));//����IDirectInputDevice8�ӿڶ���(Mouse����豸����)
	VERIFY(m_pDirectInputDeviceMouse->SetCooperativeLevel(hWnd, dwDeviceCoopFlags));//DirectInputDevice����Э������
	VERIFY(m_pDirectInputDeviceMouse->SetDataFormat(&c_dfDIMouse));//DirectInputDevice�������ݸ�ʽ(��׼���ṹ)
	VERIFY(m_pDirectInputDeviceMouse->Acquire());//DirectInputDevice��ȡ�豸����Ȩ
	VERIFY(m_pDirectInputDeviceMouse->Poll());//DirectInputDevice��ѯ�豸

	return S_OK;//OK
}

//-------------------------------------------------------------------------------
// @Function:	DirectInputGetDeviceState(void) const
// @Purpose: DirectInput ��ȡ�豸״̬(����&���)
// @Since: v1.00a
// @Para: None
// @Return: None
//-------------------------------------------------------------------------------
void WINAPI DirectInput::DirectInputGetDeviceState(void) const
{
	HRESULT hr;

	//��ȡKeyBoard�����豸״̬
	hr = m_pDirectInputDeviceKeyBoard->GetDeviceState(sizeof(m_cKeyBoradBuffer), (LPVOID)m_cKeyBoradBuffer);//��ȡKeyBoard�����豸״̬
	if (hr)
	{
		m_pDirectInputDeviceKeyBoard->Poll();//��ѯ�豸
		m_pDirectInputDeviceKeyBoard->Acquire();//��ȡ����Ȩ
		hr = m_pDirectInputDeviceKeyBoard->GetDeviceState(sizeof(m_cKeyBoradBuffer), (LPVOID)m_cKeyBoradBuffer);//��ȡKeyBoard�����豸״̬
	}

	//��ȡMouse����豸״̬
	hr = m_pDirectInputDeviceMouse->GetDeviceState(sizeof(m_DIMouseState), (LPVOID)&m_DIMouseState);//��ȡMouse����豸״̬
	if (hr)
	{
		m_pDirectInputDeviceMouse->Poll();//��ѯ�豸
		m_pDirectInputDeviceMouse->Acquire();//��ȡ����Ȩ
		hr = m_pDirectInputDeviceMouse->GetDeviceState(sizeof(m_DIMouseState), (LPVOID)&m_DIMouseState);//��ȡMouse����豸״̬
	}
}

//-------------------------------------------------------------------------------
// @Function:	DirectInputGetDeviceState(DirectInputDevice eDirectInputDevice_X) const
// @Purpose: DirectInput ��ȡ�豸״̬(����/���)
// @Since: v1.00a
// @Para: DirectInputDevice eDirectInputDevice_X(ö������:����/���/��Ϸ��)
// @Return: None
//-------------------------------------------------------------------------------
void WINAPI DirectInput::DirectInputGetDeviceState(DirectInputDevice eDirectInputDevice_X) const
{
	HRESULT hr;

	switch (eDirectInputDevice_X)
	{
	case DirectInputDevice_KeyBoard:
		//��ȡKeyBoard�����豸״̬
		hr = m_pDirectInputDeviceKeyBoard->GetDeviceState(sizeof(m_cKeyBoradBuffer), (LPVOID)m_cKeyBoradBuffer);//��ȡKeyBoard�����豸״̬
		if (hr)
		{
			m_pDirectInputDeviceKeyBoard->Poll();//��ѯ�豸
			m_pDirectInputDeviceKeyBoard->Acquire();//��ȡ����Ȩ
			hr = m_pDirectInputDeviceKeyBoard->GetDeviceState(sizeof(m_cKeyBoradBuffer), (LPVOID)m_cKeyBoradBuffer);//��ȡKeyBoard�����豸״̬
		}
		break;
	case DirectInputDevice_Mouse:
		//��ȡMouse����豸״̬
		hr = m_pDirectInputDeviceMouse->GetDeviceState(sizeof(m_DIMouseState), (LPVOID)&m_DIMouseState);//��ȡMouse����豸״̬
		if (hr)
		{
			m_pDirectInputDeviceMouse->Poll();//��ѯ�豸
			m_pDirectInputDeviceMouse->Acquire();//��ȡ����Ȩ
			hr = m_pDirectInputDeviceMouse->GetDeviceState(sizeof(m_DIMouseState), (LPVOID)&m_DIMouseState);//��ȡMouse����豸״̬
		}
		break;
	case DirectInputDevice_JoyStick:
		//��ȡJoyStick��Ϸ���豸״̬
		hr = m_pDirectInputDeviceJoyStick->GetDeviceState(sizeof(m_DIJoyStickState), (LPVOID)&m_DIJoyStickState);//��ȡJoyStick��Ϸ���豸״̬
		if (hr)
		{
			m_pDirectInputDeviceJoyStick->Poll();//��ѯ�豸
			m_pDirectInputDeviceJoyStick->Acquire();//��ȡ����Ȩ
			hr = m_pDirectInputDeviceJoyStick->GetDeviceState(sizeof(m_DIJoyStickState), (LPVOID)&m_DIJoyStickState);//��ȡJoyStick��Ϸ���豸״̬
		}
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
// @Function:	DIKeyBoardIsDown(int nKeyValue) const
// @Purpose: DirectInput ��ȡKeyBoard�����豸����״̬
// @Since: v1.00a
// @Para: int nKeyValue(KeyBoard��ֵ)(eg:DIK_A)
// @Return: bool(true:KeyDown,false:KeyUp)
//--------------------------------------------------------------
bool WINAPI DirectInput::DIKeyBoardIsDown(int nKeyValue) const
{
	return (m_cKeyBoradBuffer[nKeyValue] & 0x80) ? true : false;
}

//--------------------------------------------------------------
// @Function:	DIKeyBoardIsUp(int nKeyValue) const
// @Purpose: DirectInput ��ȡKeyBoard�����豸����״̬
// @Since: v1.00a
// @Para: int nKeyValue(KeyBoard��ֵ)(eg:DIK_A)
// @Return: bool(true:KeyUp,false:KeyDown)
//--------------------------------------------------------------
bool WINAPI DirectInput::DIKeyBoardIsUp(int nKeyValue) const
{
	return (m_cKeyBoradBuffer[nKeyValue] & 0x80) ? false : true;
}

//-------------------------------------------------------------------------
// @Function:	DIMouseIsDown(DirectInputMouseState eDIMouse_XButton) const
// @Purpose: DirectInput ��ȡMouse����豸����״̬
// @Since: v1.00a
// @Para: DirectInputMouseState eDIMouse_XButton(����ֵ)
// @Return: bool(true:MouseDown,false:MouseUp)
//-------------------------------------------------------------------------
bool WINAPI DirectInput::DIMouseIsDown(DirectInputMouseState eDIMouse_XButton) const
{
	return (m_DIMouseState.rgbButtons[eDIMouse_XButton] & 0x80) ? true : false;
}

//-------------------------------------------------------------------------
// @Function:	DIMouseIsUp(DirectInputMouseState eDIMouse_XButton) const
// @Purpose: DirectInput ��ȡMouse����豸����״̬
// @Since: v1.00a
// @Para: DirectInputMouseState eDIMouse_XButton(����ֵ)
// @Return: bool(true:MouseUp,false:MouseDown)
//-------------------------------------------------------------------------
bool WINAPI DirectInput::DIMouseIsUp(DirectInputMouseState eDIMouse_XButton) const
{
	return (m_DIMouseState.rgbButtons[eDIMouse_XButton] & 0x80) ? false : true;
}

//-------------------------------------------------------------------------
// @Function:	DIMouseGetX(void) const
// @Purpose: DirectInput ��ȡMouse���X������
// @Since: v1.00a
// @Para: None
// @Return: float(Mouse���X������)
//-------------------------------------------------------------------------
float WINAPI DirectInput::DIMouseGetX(void) const
{
	return (float)m_DIMouseState.lX;
}

//-------------------------------------------------------------------------
// @Function:	DIMouseGetY(void) const
// @Purpose: DirectInput ��ȡMouse���Y������
// @Since: v1.00a
// @Para: None
// @Return: float(Mouse���Y������)
//-------------------------------------------------------------------------
float WINAPI DirectInput::DIMouseGetY(void) const
{
	return (float)m_DIMouseState.lY;
}

//-------------------------------------------------------------------------
// @Function:	DIMouseGetZ(void) const
// @Purpose: DirectInput ��ȡMouse���Z������
// @Since: v1.00a
// @Para: None
// @Return: float(Mouse���Z������)
//-------------------------------------------------------------------------
float WINAPI DirectInput::DIMouseGetZ(void) const
{
	return (float)m_DIMouseState.lZ;
}

//--------------------------------------------------------------
// @Function:	DIKeyDownProc(int nKeyValue, KeyDownProc)
// @Purpose: DirectInput ��ȡKeyBoard�����豸����״̬(��������Ӧ)
// @Since: v1.00a
// @Para: int nKeyValue(KeyBoard��ֵ)(eg:DIK_A)
// @Para: KeyDownProc(KeyBoard�����ص�����)
// @Return: None
//--------------------------------------------------------------
void CALLBACK DirectInput::DIKeyDownProc(int nKeyValue, KeyDownProc)
{
	if (m_cKeyBoradBuffer[nKeyValue] & 0x80)
	{
		KeyDownProc();//KeyProc
	}
}

//--------------------------------------------------------------
// @Function:	DIMouseDownProc(DirectInputMouseState eDIMouse_XButton, MouseDownProc)
// @Purpose: DirectInput ��ȡMouse����豸����״̬(��������Ӧ)
// @Since: v1.00a
// @Para: DirectInputMouseState eDIMouse_XButton(����ֵ)
// @Para: MouseDownProc(Mouse�����ص�����)
// @Return: None
//--------------------------------------------------------------
void CALLBACK DirectInput::DIMouseDownProc(DirectInputMouseState eDIMouse_XButton, MouseDownProc)
{
	if (m_DIMouseState.rgbButtons[eDIMouse_XButton] & 0x80)
	{
		MouseDownProc();//MouseProc
	}
}