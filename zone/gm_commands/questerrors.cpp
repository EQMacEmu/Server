#include "../client.h"
#include "../quest_parser_collection.h"

void command_questerrors(Client *c, const Seperator *sep)
{
	std::list<std::string> quest_errors;
	parse->GetErrors(quest_errors);

	if (quest_errors.size()) {
		c->Message(CC_Default, "Quest errors currently are as follows:");

		int error_index = 0;
		for (auto quest_error : quest_errors) {
			if (error_index >= 30) {
				c->Message(CC_Default, "Maximum of 30 errors shown.");
				break;
			}

			c->Message(CC_Default, quest_error.c_str());
			error_index++;
		}
	}
	else {
		c->Message(CC_Default, "There are no Quest errors currently.");
	}
}

