#include "../client.h"

void command_spellinfo(Client *c, const Seperator *sep){
	if (sep->arg[1][0] == 0)
		c->Message(CC_Default, "Usage: #spellinfo [spell_id]");
	else {
		short int spell_id = atoi(sep->arg[1]);
		const struct SPDat_Spell_Struct *s = &spells[spell_id];
		c->Message(CC_Default, "Spell info for spell #%d:", spell_id);
		c->Message(CC_Default, "  name: %s", s->name);
		c->Message(CC_Default, "  player_1: %s", s->player_1);
		c->Message(CC_Default, "  teleport_zone: %s", s->teleport_zone);
		c->Message(CC_Default, "  you_cast: %s", s->you_cast);
		c->Message(CC_Default, "  other_casts: %s", s->other_casts);
		c->Message(CC_Default, "  cast_on_you: %s", s->cast_on_you);
		c->Message(CC_Default, "  spell_fades: %s", s->spell_fades);
		c->Message(CC_Default, "  range: %f", s->range);
		c->Message(CC_Default, "  aoerange: %f", s->aoerange);
		c->Message(CC_Default, "  pushback: %f", s->pushback);
		c->Message(CC_Default, "  pushup: %f", s->pushup);
		c->Message(CC_Default, "  cast_time: %d", s->cast_time);
		c->Message(CC_Default, "  recovery_time: %d", s->recovery_time);
		c->Message(CC_Default, "  recast_time: %d", s->recast_time);
		c->Message(CC_Default, "  buffdurationformula: %d", s->buffdurationformula);
		c->Message(CC_Default, "  buffduration: %d", s->buffduration);
		c->Message(CC_Default, "  AEDuration: %d", s->AEDuration);
		c->Message(CC_Default, "  mana: %d", s->mana);
		c->Message(CC_Default, "  base[12]: %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", s->base[0], s->base[1], s->base[2], s->base[3], s->base[4], s->base[5], s->base[6], s->base[7], s->base[8], s->base[9], s->base[10], s->base[11]);
		c->Message(CC_Default, "  base22[12]: %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", s->base2[0], s->base2[1], s->base2[2], s->base2[3], s->base2[4], s->base2[5], s->base2[6], s->base2[7], s->base2[8], s->base2[9], s->base2[10], s->base2[11]);
		c->Message(CC_Default, "  max[12]: %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", s->max[0], s->max[1], s->max[2], s->max[3], s->max[4], s->max[5], s->max[6], s->max[7], s->max[8], s->max[9], s->max[10], s->max[11]);
		c->Message(CC_Default, "  components[4]: %d, %d, %d, %d", s->components[0], s->components[1], s->components[2], s->components[3]);
		c->Message(CC_Default, "  component_counts[4]: %d, %d, %d, %d", s->component_counts[0], s->component_counts[1], s->component_counts[2], s->component_counts[3]);
		c->Message(CC_Default, "  NoexpendReagent[4]: %d, %d, %d, %d", s->NoexpendReagent[0], s->NoexpendReagent[1], s->NoexpendReagent[2], s->NoexpendReagent[3]);
		c->Message(CC_Default, "  formula[12]: 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x", s->formula[0], s->formula[1], s->formula[2], s->formula[3], s->formula[4], s->formula[5], s->formula[6], s->formula[7], s->formula[8], s->formula[9], s->formula[10], s->formula[11]);
		c->Message(CC_Default, "  goodEffect: %d", s->goodEffect);
		c->Message(CC_Default, "  Activated: %d", s->Activated);
		c->Message(CC_Default, "  resisttype: %d", s->resisttype);
		c->Message(CC_Default, "  effectid[12]: 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x", s->effectid[0], s->effectid[1], s->effectid[2], s->effectid[3], s->effectid[4], s->effectid[5], s->effectid[6], s->effectid[7], s->effectid[8], s->effectid[9], s->effectid[10], s->effectid[11]);
		c->Message(CC_Default, "  targettype: %d", s->targettype);
		c->Message(CC_Default, "  basediff: %d", s->basediff);
		c->Message(CC_Default, "  skill: %d", s->skill);
		c->Message(CC_Default, "  zonetype: %d", s->zonetype);
		c->Message(CC_Default, "  EnvironmentType: %d", s->EnvironmentType);
		c->Message(CC_Default, "  TimeOfDay: %d", s->TimeOfDay);
		c->Message(CC_Default, "  classes[15]: %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
			s->classes[0], s->classes[1], s->classes[2], s->classes[3], s->classes[4],
			s->classes[5], s->classes[6], s->classes[7], s->classes[8], s->classes[9],
			s->classes[10], s->classes[11], s->classes[12], s->classes[13], s->classes[14]);
		c->Message(CC_Default, "  CastingAnim: %d", s->CastingAnim);
		c->Message(CC_Default, "  SpellAffectIndex: %d", s->SpellAffectIndex);
		c->Message(CC_Default, " RecourseLink: %d", s->RecourseLink);
	}
}

