
//*************************************************************************************
//Code merged from https://gitlab.com/fourk/gdlenhanced/commits/use_mana_stones_on_self
//*************************************************************************************

//Merged from GDLE2 Team https://gitlab.com/Scribble/gdlenhanced/commit/75132b15809a026a58d5ed5abefac26290b14461 //

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
#pragma once
