//*************************************************************************************
//Code merged from https://gitlab.com/fourk/gdlenhanced/commits/use_mana_stones_on_self
//*************************************************************************************

#pragma once

#include "WeenieObject.h"
#include "UseManager.h"

class CManaStoneUseEvent : public CUseEventData
{
public:
	virtual void OnReadyToUse() override;
};

class CManaStoneWeenie : public CWeenieObject // CWeenieObject
{
public:
	CManaStoneWeenie();
	virtual ~CManaStoneWeenie() override;

	virtual class CManaStoneWeenie *AsManaStone() { return this; }

	virtual void ApplyQualityOverrides() override;
	virtual int UseWith(CPlayerWeenie *player, CWeenieObject *with) override;

protected:
};

