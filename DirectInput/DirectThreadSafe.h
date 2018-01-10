/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2018, Team Shanghai Dream Equinox
*     All rights reserved.
*
* @file		DirectThreadSafe.h
* @brief	This Program is DirectThreadSafe Header File.
* @author	Alopex/Helium
* @version	v1.00a
* @date		2018-1-10	v1.00a	alopex	Create This File.
*/
#pragma once

#ifndef __DIRECTTHREADSAFE_H_
#define __DIRECTTHREADSAFE_H_

//Include DirectCommon Header File
#include "DirectCommon.h"

//Macro Definition
#ifdef  DIRECTINPUT_EXPORTS
#define DIRECTTHREADSAFE_API	__declspec(dllexport)
#else
#define DIRECTTHREADSAFE_API	__declspec(dllimport)
#endif

//Class
class DIRECTTHREADSAFE_API DirectThreadSafe
{
private:
	CRITICAL_SECTION* m_pCriticalSection;
	bool m_bThreadSafe;

public:
	DirectThreadSafe(const CRITICAL_SECTION* pCriticalSection, const bool bThreadSafe);
	~DirectThreadSafe();
};

//DirectThreadSafe构造函数
DirectThreadSafe::DirectThreadSafe(const CRITICAL_SECTION* pCriticalSection, const bool bThreadSafe)
{
	m_pCriticalSection = (CRITICAL_SECTION*)pCriticalSection;
	m_bThreadSafe = bThreadSafe;

	if (m_bThreadSafe) EnterCriticalSection(m_pCriticalSection);
}

//DirectThreadSafe析构函数
DirectThreadSafe::~DirectThreadSafe()
{
	if (m_bThreadSafe) LeaveCriticalSection(m_pCriticalSection);
}

#endif