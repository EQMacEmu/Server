#include "../client.h"

void command_randtest(Client *c, const Seperator *sep) {
	
	if (!sep->IsNumber(1)) {
		c->Message(CC_Default, "Usage: #randtest [iterations]");
		return;
	}
	int total = atoi(sep->arg[1]);
	if (total < 1 || total > 10000000) {
		c->Message(CC_Default, "Usage: #randtest [iterations] min value 1, max 1000000");
		return;
	}
	int lastval = -1;
	int maxlastval = 0;
	int maxtimes = 0;
	int maxcount = 0;
	int results[100];
	for (int i = 0; i < 100; i++) {
		results[i] = 0;
	}

	for (int i = 0; i < total; i++) {
		int value = zone->random.Int(0, 99);
		if (lastval == value) {
			maxlastval++;
		}
		else {
			if (maxlastval > maxtimes) {
				maxcount = 1;
				maxtimes = maxlastval;
			}
			else if (maxlastval == maxtimes) {
				maxcount++;
			}
			maxlastval = 0;
		}
		lastval = value;
		results[value]++;
	}
	for (int i = 0; i < 100; i++) {
		c->Message(CC_Default, "Random Results [%i], %i (%.2f %s)", i, results[i], (float)results[i] / (float)total * 100.0f, "%");
	}
	c->Message(CC_Default, "Same number happened %i times in a row, %i times", maxtimes + 1, maxcount);
}

