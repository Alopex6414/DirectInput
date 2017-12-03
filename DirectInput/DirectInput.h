/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2017, Team Shanghai Dream Equinox
*     All rights reserved.
*
* @file		DirectInput.h
* @brief	This Program is DirectInput DLL Project.
* @author	Alopex/Helium
* @version	v1.01a
* @date		2017-10-27	v1.00a	alopex	Create Project
* @date		2017-12-3	v1.01a	alopex	Add Enum & Modify CallBack Function
*/
#pragma once

#ifndef __DIRECTINPUT_H_
#define __DIRECTINPUT_H_

//Macro Definition
#ifdef  DIRECTINPUT_EXPORTS
#define DIRECTINPUT_API	__declspec(dllexport)
#else
#define DIRECTINPUT_API	__declspec(dllimport)
#endif

//Class Definition
#define KEYBOARD_ARRAYSIZE	256	//KeyBoard Array Size(��׼���������С256��)
#define JOYSTICK_ARRAYSIZE	80	//JoyStick Array Size(JoyStick��Ϸ�������С80��)

//DirectInput�����豸
enum DirectInputDevice
{
	DirectInputDevice_KeyBoard = 0,	//KeyBoard Device(����)
	DirectInputDevice_Mouse = 1,	//Mouse Device(���)
	DirectInputDevice_JoyStick = 2, //JoyStick Device(��Ϸ��)
};

//DirectInput�������豸
enum DirectInputMulDevice
{
	DirectInputDevice_KeyBoard_Mouse = 0,		//KeyBoard&Mouse(����&���)
	DirectInputDevice_KeyBoard_JoyStick = 1,	//KeyBoard&JoyStick(����&��Ϸ��)
	DirectInputDevice_Mouse_JoyStick = 2,		//Mouse&JoyStick(���&��Ϸ��)
};

//DirectInput�豸Э������
enum DirectInputDeviceCoopFlags
{
	DirectInputCoopFlags_ForeGround_Exclusive = 0,		//ForeGround_Exclusive(ǰ̨��ռģʽ)
	DirectInputCoopFlags_ForeGround_NoExclusive = 1,	//ForeGround_NoExclusive(ǰ̨�Ƕ�ռģʽ)
	DirectInputCoopFlags_BackGround_NoExclusive = 2,	//BackGround_NoExclusive(��̨�Ƕ�ռģʽ)
};

//DirectInput����ֵ״̬
enum DirectInputMouseState
{
	DIMouse_LeftButton = 0,		//MouseButton:Left(���)
	DIMouse_RightButton = 1,	//MouseButton:Right(�Ҽ�)
	DIMouse_MiddleButton = 2,	//MouseButton:Middle(�м�)
};

//CallBack Function
typedef	void	(CALLBACK*LPCALLBACKKEYDOWNPROCFUNC)(void);				//������Ӧ�ص�����
typedef void	(CALLBACK*LPCALLBACKMOUSEDOWNPROCFUNC)(void);			//�����Ӧ�ص�����

//Class
class DIRECTINPUT_API DirectInput
{
private:
	LPDIRECTINPUT8 m_pDirectInput;	//IDirectInput8 Interface Pointer(IDirectInput8�ӿ�ָ���Ա)
	LPDIRECTINPUTDEVICE8 m_pDirectInputDeviceKeyBoard;	//IDirectInputDevice8 Interface Pointer(KeyBoard Device)(IDirectInputDevice8�ӿ��豸ָ��:����)
	LPDIRECTINPUTDEVICE8 m_pDirectInputDeviceMouse;		//IDirectInputDevice8 Interface Pointer(Mouse Device)(IDirectInputDevice8�ӿ��豸ָ��:���)
	LPDIRECTINPUTDEVICE8 m_pDirectInputDeviceJoyStick;	//IDirectInputDevice8 Interface Pointer(JoyStick Device)(IDirectInputDevice8�ӿ��豸ָ��:��Ϸ��)

	DIMOUSESTATE m_DIMouseState;	//Mouse State Struct(���״̬)
	DIJOYSTATE m_DIJoyStickState;	//JoyStick State Struct(��Ϸ��״̬)
	char m_cKeyBoradBuffer[KEYBOARD_ARRAYSIZE]; //KeyBoard State Array(����״̬)

public:
	DirectInput();	//DirectInput Constructor Function(���캯��)
	~DirectInput();	//DirectInput Destructor Function(��������)

	//DirectInput��ʼ��
	HRESULT WINAPI DirectInputInit(HWND hWnd, HINSTANCE hInstance);	//DirectInput Initialize(KeyBoard&Mouse)(dwDeviceCoopFlags:DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)
	HRESULT WINAPI DirectInputInit(HWND hWnd, HINSTANCE hInstance, DirectInputDevice eDirectInputDevice_X, DWORD dwDeviceCoopFlags);									//DirectInput Single Device Initialize
	HRESULT WINAPI DirectInputInit(HWND hWnd, HINSTANCE hInstance, DirectInputMulDevice eDirectInputMulDevice_X, DWORD dwDeviceCoopFlags1, DWORD dwDeviceCoopFlags2);	//DirectInput Multiple Device Initialize
	HRESULT WINAPI DirectInputInit(HWND hWnd, HINSTANCE hInstance, DirectInputDevice eDirectInputDevice_X, DirectInputDeviceCoopFlags eDirectInputDeviceCoopFlags);		//DirectInput Single Device Initialize
	HRESULT WINAPI DirectInputInit(HWND hWnd, HINSTANCE hInstance, DirectInputMulDevice eDirectInputMulDevice_X, DirectInputDeviceCoopFlags eDirectInputDeviceCoopFlags1, DirectInputDeviceCoopFlags eDirectInputDeviceCoopFlags2);	//DirectInput Multiple Device Initialize
	HRESULT WINAPI DirectInputKeyBoardInit(HWND hWnd, HINSTANCE hInstance);	//DirectInput Initialize(KeyBoard)(dwDeviceCoopFlags:DISCL_FOREGROUND | DISCL_EXCLUSIVE)
	HRESULT WINAPI DirectInputKeyBoardInit(HWND hWnd, HINSTANCE hInstance, DWORD dwDeviceCoopFlags);						//DirectInput Initialize(KeyBoard)
	HRESULT WINAPI DirectInputKeyBoardInit(HWND hWnd, HINSTANCE hInstance, DirectInputDeviceCoopFlags eDeviceCoopFlags);	//DirectInput Initialize(KeyBoard)
	HRESULT WINAPI DirectInputMouseInit(HWND hWnd, HINSTANCE hInstance);													//DirectInput Initialize(Mouse)(dwDeviceCoopFlags:DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)
	HRESULT WINAPI DirectInputMouseInit(HWND hWnd, HINSTANCE hInstance, DWORD dwDeviceCoopFlags);							//DirectInput Initialize(Mouse)
	HRESULT WINAPI DirectInputMouseInit(HWND hWnd, HINSTANCE hInstance, DirectInputDeviceCoopFlags eDeviceCoopFlags);		//DirectInput Initialize(Mouse)
	
	//DirectInput��ȡ�豸״̬
	void WINAPI DirectInputGetDeviceState(void) const;	//DirectInput Get Device State(KeyBoard&Mouse)
	void WINAPI DirectInputGetDeviceState(DirectInputDevice eDirectInputDevice_X) const;	//DirectInput Get Device State
	
	//DirectInput���(����/���)����״̬
	bool WINAPI DIKeyBoardIsDown(int nKeyValue) const;							//DirectInput KeyBoard Is KeyDown(���̰�������)
	bool WINAPI DIKeyBoardIsUp(int nKeyValue) const;							//DirectInput KeyBoard Is KeyUp(���̰����ͷ�)
	bool WINAPI DIMouseIsDown(DirectInputMouseState eDIMouse_XButton) const;	//DirectInput Mouse Is KeyDown(��갴������)
	bool WINAPI DIMouseIsUp(DirectInputMouseState eDIMouse_XButton) const;		//DirectInput Mouse Is KeyUp(��갴���ͷ�)
	
	//DirectInput��ȡ�������
	float WINAPI DIMouseGetX(void) const;										//DirectInput Mouse Get X Axis Value(��ȡ���X������)
	float WINAPI DIMouseGetY(void) const;										//DirectInput Mouse Get Y Axis Value(��ȡ���Y������)
	float WINAPI DIMouseGetZ(void) const;										//DirectInput Mouse Get Z Axis Value(Mouse Wheel)(��ȡ���Z��/��������)
	
	//DirectInput(����/���)�ص�����
	void CALLBACK DIKeyDownProc(int nKeyValue, LPCALLBACKKEYDOWNPROCFUNC pCallBackKeyDownProc);									//DirectInput KeyDown Proc(���̻ص�����)
	void CALLBACK DIMouseDownProc(DirectInputMouseState eDIMouse_XButton, LPCALLBACKMOUSEDOWNPROCFUNC pCallBackMouseDownProc);	//DirectInput MouseDown Proc(���ص�����)
};

#endif