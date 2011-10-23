/*
 * WEmu Scripts for WEmu MMORPG Server
 * Copyright (C) 2008 WEmu Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "StdAfx.h"
#include "Setup.h"
#include "../Common/EasyFunctions.h"

#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, plr); \ Menu->SendTo(plr);



class SpiritScreeches : public GossipScript
{
public:
	void GossipHello(ObjectPointer pObject, PlayerPointer plr, bool AutoSend)
	{
		if(!plr)
			return;

		GossipMenu *Menu;
		CreaturePointer spirit = TO_CREATURE(pObject);
		if (spirit == NULL)
			return;

		if(plr->GetQuestLogForEntry(3520))
		{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2039, plr);
			Menu->AddItem( 0, "Goodbye", 1);

			if(AutoSend)
				Menu->SendTo(plr);
		}



	}

	void GossipSelectOption(ObjectPointer pObject, PlayerPointer plr, uint32 Id, uint32 IntId, const char * EnteredCode)
	{
		if(!plr)
			return;

		CreaturePointer spirit = TO_CREATURE(pObject);
		if (spirit == NULL)
			return;

		switch (IntId)
		{
			case 0:
				GossipHello(pObject, plr, true);
				break;

			case 1:
				{
					QuestLogEntry *en = plr->GetQuestLogForEntry(3520);
					if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
					{
						en->SetMobCount(0, en->GetMobCount(0) + 1);
						en->SendUpdateAddKill(0);
						en->UpdatePlayerFields();
					}
					if(!spirit)
						return;

					spirit->Despawn(1, 0);
					return;

				}
		}
	}

	void Destroy()
	{
		delete this;
	}
};

class ScreecherSpirit : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ScreecherSpirit);

	ScreecherSpirit(CreaturePointer pCreature) : CreatureAIScript(pCreature) {}

	void OnLoad()
	{
		if(!_unit)
			return

		_unit->Despawn(60*1000, 0);
		CreaturePointer cialo = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 5307);
		if(!cialo)
			return;

		if(!cialo->isAlive())
			cialo->Despawn(1, 6*60*1000);
	}
};

class StewardOfTime : public GossipScript
{
public:
	void GossipHello(ObjectPointer pObject, PlayerPointer plr, bool AutoSend)
	{
		GossipMenu *Menu;
		if(plr->GetQuestLogForEntry(10279) || plr->HasFinishedQuest(10279))
		{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9978, plr);
			Menu->AddItem( 0, "Please take me to the master's lair", 1);
			Menu->SendTo(plr);
		}
	}

	void GossipSelectOption(ObjectPointer pObject, PlayerPointer plr, uint32 Id, uint32 IntId, const char * Code)
	{
		CreaturePointer creat = TO_CREATURE(pObject);
		switch(IntId)
		{
		case 1:
			creat->CastSpell(plr, dbcSpell.LookupEntry(34891), true);
			break;
		}
	}

	void Destroy()
	{
		delete this;
	};
};

void SetupTanaris(ScriptMgr * mgr)
{
	GossipScript * Screeches = (GossipScript*) new SpiritScreeches();
	mgr->register_gossip_script(8612, Screeches);

	mgr->register_creature_script(8612, &ScreecherSpirit::Create);
	
	GossipScript * StewardOfTimeGossip = (GossipScript*) new StewardOfTime();
	mgr->register_gossip_script(20142, StewardOfTimeGossip);
}