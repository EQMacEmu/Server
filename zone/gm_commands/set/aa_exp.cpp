#include "../../client.h"
#include "../../groups.h"
#include "../../raids.h"

void SetAAEXP(Client *c, const Seperator *sep)
{
	const auto arguments = sep->argnum;
	if (arguments < 2 || !sep->IsNumber(2)) {
		c->Message(Chat::White, "Usage: #set aa_exp [Amount]");
		return;
	}

	auto t = c;
	if (c->GetTarget() && c->GetTarget()->IsClient()) {
		t = c->GetTarget()->CastToClient();
	}

	const uint32 aa_experience = Strings::ToUnsignedInt(sep->arg[2]);

	t->SetEXP(
		t->GetEXP(),
		aa_experience,
		false
	);

	c->Message(
		Chat::White,
		fmt::format(
			"{} now {} {} AA Experience.",
			c->GetTargetDescription(t, TargetDescriptionType::UCYou),
			c == t ? "have" : "has",
			Strings::Commify(aa_experience)
		).c_str()
	);
}
