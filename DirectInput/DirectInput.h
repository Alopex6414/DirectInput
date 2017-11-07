/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2017, Alopex/Helium
*     All rights reserved.
*
* @file		DirectInput.h
* @brief	This Program is DirectInput DLL Project.
* @author	Alopex/Helium
* @version	v1.00a
* @date		2017-10-27
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
#define KEYBOARD_ARRAYSIZE	256	//KeyBoard Array Size
#define JOYSTICK_ARRAYSIZE	80	//JoyStick Array Size

enum DirectInputDevice
{
	DirectInputDevice_KeyBoard = 0,	//KeyBoard Device
	DirectInputDevice_Mouse = 1,	//Mouse Device
	DirectInputDevice_JoyStick = 2, //JoyStick Device
};

enum DirectInputMulDevice
{
	DirectInputDevice_KeyBoard_Mouse = 0,		//KeyBoard&Mouse
	DirectInputDevice_KeyBoard_JoyStick = 1,	//KeyBoard&JoyStick
	DirectInputDevice_Mouse_JoyStick = 2,		//Mouse&JoyStick
};

enum DirectInputMouseState
{
	DIMouse_LeftButton = 0,		//MouseButton:Left
	DIMouse_RightButton = 1,	//MouseButton:Right
	DIMouse_MiddleButton = 2,	//MouseButton:Middle
};

//CallBack Function
typedef void(CALLBACK*KeyDownProc)(void);
typedef void(CALLBACK*MouseDownProc)(void);

//Class
class DIRECTINPUT_API DirectInput
{
private:
	LPDIRECTINPUT8 m_pDirectInput;	//IDirectInput8 Interface Pointer
	LPDIRECTINPUTDEVICE8 m_pDirectInputDeviceKeyBoard;	//IDirectInputDevice8 Interface Pointer(KeyBoard Device)
	LPDIRECTINPUTDEVICE8 m_pDirectInputDeviceMouse;		//IDirectInputDevice8 Interface Pointer(Mouse Device)
	LPDIRECTINPUTDEVICE8 m_pDirectInputDeviceJoyStick;	//IDirectInputDevice8 Interface Pointer(JoyStick Device)

	DIMOUSESTATE m_DIMouseState;	//Mouse State Struct
	DIJOYSTATE m_DIJoyStickState;	//JoyStick State Struct
	char m_cKeyBoradBuffer[KEYBOARD_ARRAYSIZE]; //KeyBoard State Array

public:
	DirectInput();	//DirectInput Constructor Function
	~DirectInput();	//DirectInput Destructor Function

	HRESULT WINAPI DirectInputInit(HWND hWnd, HINSTANCE hInstance);	//DirectInput Initialize(KeyBoard&Mouse)(dwDeviceCoopFlags:DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)
	HRESULT WINAPI DirectInputInit(HWND hWnd, HINSTANCE hInstance, DirectInputDevice eDirectInputDevice_X, DWORD dwDeviceCoopFlags);	//DirectInput Single Device Initialize
	HRESULT WINAPI DirectInputInit(HWND hWnd, HINSTANCE hInstance, DirectInputMulDevice eDirectInputMulDevice_X, DWORD dwDeviceCoopFlags1, DWORD dwDeviceCoopFlags2); //DirectInput Multiple Device Initialize
	HRESULT WINAPI DirectInputKeyBoardInit(HWND hWnd, HINSTANCE hInstance);	//DirectInput Initialize(KeyBoard)(dwDeviceCoopFlags:DISCL_FOREGROUND | DISCL_EXCLUSIVE)
	HRESULT WINAPI DirectInputKeyBoardInit(HWND hWnd, HINSTANCE hInstance, DWORD dwDeviceCoopFlags);	//DirectInput Initialize(KeyBoard)
	HRESULT WINAPI DirectInputMouseInit(HWND hWnd, HINSTANCE hInstance);		//DirectInput Initialize(Mouse)(dwDeviceCoopFlags:DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)
	HRESULT WINAPI DirectInputMouseInit(HWND hWnd, HINSTANCE hInstance, DWORD dwDeviceCoopFlags);	//DirectInput Initialize(Mouse)
	void WINAPI DirectInputGetDeviceState(void) const;	//DirectInput Get Device State(KeyBoard&Mouse)
	void WINAPI DirectInputGetDeviceState(DirectInputDevice eDirectInputDevice_X) const;	//DirectInput Get Device State
	bool WINAPI DIKeyBoardIsDown(int nKeyValue) const;	//DirectInput KeyBoard Is KeyDown
	bool WINAPI DIKeyBoardIsUp(int nKeyValue) const;		//DirectInput KeyBoard Is KeyUp
	bool WINAPI DIMouseIsDown(DirectInputMouseState eDIMouse_XButton) const;	//DirectInput Mouse Is KeyDown
	bool WINAPI DIMouseIsUp(DirectInputMouseState eDIMouse_XButton) const;	//DirectInput Mouse Is KeyUp
	float WINAPI DIMouseGetX(void) const;	//DirectInput Mouse Get X Axis Value
	float WINAPI DIMouseGetY(void) const;	//DirectInput Mouse Get Y Axis Value
	float WINAPI DIMouseGetZ(void) const;	//DirectInput Mouse Get Z Axis Value(Mouse Wheel)
	void CALLBACK DIKeyDownProc(int nKeyValue, KeyDownProc);	//DirectInput KeyDown Proc
	void CALLBACK DIMouseDownProc(DirectInputMouseState eDIMouse_XButton, MouseDownProc);	//DirectInput MouseDown Proc
};

#endif