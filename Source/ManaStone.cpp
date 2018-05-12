//*************************************************************************************
//Code merged from https://gitlab.com/fourk/gdlenhanced/commits/use_mana_stones_on_self
//*************************************************************************************

//Merged from GDLE2 Team https://gitlab.com/Scribble/gdlenhanced/commit/75132b15809a026a58d5ed5abefac26290b14461 //

#include "StdAfx.h"
#include "WeenieObject.h"
#include "ManaStone.h"
#include "Player.h"


CManaStoneWeenie::CManaStoneWeenie()
{
	SetName("ManaStone");
	m_Qualities.m_WeenieType = ManaStone_WeenieType;
}

CManaStoneWeenie::~CManaStoneWeenie()
{
}

void CManaStoneWeenie::ApplyQualityOverrides()
{
}

int CManaStoneWeenie::UseWith(CPlayerWeenie *player, CWeenieObject *with)
{
	CManaStoneUseEvent *useEvent = new CManaStoneUseEvent;
	useEvent->_target_id = with->GetID();
	useEvent->_tool_id = GetID();
	useEvent->_max_use_distance = 1.0; // todo: change to 0?
	player->ExecuteUseEvent(useEvent);

	return WERROR_NONE;
}

/**
TODO: move logic from player useex to here
int CManaStoneWeenie::DoUseWithResponse(CWeenieObject *player, CWeenieObject *with) {
}
**/

void CManaStoneUseEvent::OnReadyToUse()
{
	CWeenieObject *target = GetTarget();
	if (!target && _target_id)
	{
		Cancel(WERROR_OBJECT_GONE);
		return;
	}

	CWeenieObject *tool = GetTool();
	if (!tool && _tool_id)
	{
		Cancel(WERROR_OBJECT_GONE);
		return;
	}

	int error = WERROR_NONE;

	if (target)
	{
		error = tool->DoUseWithResponse(_weenie, target);
	}

	Done(error);
}