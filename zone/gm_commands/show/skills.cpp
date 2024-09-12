#include "../../client.h"

void ShowSkills(Client* c, const Seperator* sep)
{
	auto t = c;
	if (c->GetTarget() && c->GetTarget()->IsClient()) {
		t = c->GetTarget()->CastToClient();
	}

	for (const auto& s : EQ::skills::GetSkillTypeMap()) {
		if (t->CanHaveSkill(s.first) && t->MaxSkill(s.first)) {
			c->Message(Chat::White, 
				fmt::format("Skills for {} : ID: {}  Name: {} Current: {} Max: {}  Raw: {}", 
					c->GetTargetDescription(t, TargetDescriptionType::UCSelf),
					s.first,
					s.second,
					t->GetSkill(s.first),
					t->MaxSkill(s.first),
					t->GetRawSkill(s.first)
				).c_str()
			);
		}
	}
}
