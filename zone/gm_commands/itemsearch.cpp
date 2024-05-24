#include "../client.h"

void command_itemsearch(Client *c, const Seperator *sep){
	if (sep->arg[1][0] == 0)
		c->Message(CC_Default, "Usage: #itemsearch [search string]");
	else
	{
		const char *search_criteria = sep->argplus[1];

		const EQ::ItemData *item = nullptr;
		EQ::SayLinkEngine linker;
		linker.SetLinkType(EQ::saylink::SayLinkItemData);

		if (Seperator::IsNumber(search_criteria)) {
			item = database.GetItem(atoi(search_criteria));
			if (item) {
				linker.SetItemData(item);

				c->Message(CC_Default, fmt::format(" {} : {} ", item->ID, linker.GenerateLink().c_str()).c_str());

			}
			else {
				c->Message(CC_Default, fmt::format("Item {} not found", search_criteria).c_str());
			}
			return;
		}

		int count = 0;
		std::string sName;
		std::string sCriteria;
		sCriteria = search_criteria;
		for (auto & c : sCriteria) c = toupper(c);
		uint32 it = 0;
		while ((item = database.IterateItems(&it))) 
		{
			sName = item->Name;
			for (auto & c : sName) c = toupper(c);
			if (sName.find(sCriteria) != std::string::npos) 
			{
				linker.SetItemData(item);
				c->Message(CC_Default, fmt::format(" {} : {} ", (int)item->ID, linker.GenerateLink().c_str()).c_str());

				++count;
			}

			if (count == 50)
				break;
		}

		if (count == 50)
			c->Message(CC_Default, "50 items shown...too many results.");
		else
			c->Message(CC_Default, fmt::format(" {} items found", count).c_str());
	}
}

