#include "../../client.h"
#include "../../worldserver.h"

extern WorldServer worldserver;

void SetWeather(Client *c, const Seperator *sep)
{
	if (!(sep->arg[2][0] == '0' || sep->arg[2][0] == '1' || sep->arg[2][0] == '2')) 
	{
		c->Message(Chat::White, "Usage: #weather <0/1/2> - Off/Rain/Snow <0/1> - Serverwide <minutes> - Duration");
		return;
	}

	uint8 weather_type = 0;
	if (sep->IsNumber(1) && sep->arg[2][0] != 0)
		weather_type = atoi(sep->arg[2]);

	uint8 serverwide = 0;
	if (sep->IsNumber(2) && sep->arg[3][0] != 0)
		serverwide = atoi(sep->arg[3]);

	//Turn off weather
	if(weather_type == 0)
	{
		c->Message(Chat::Yellow, "Turning off weather.");
		zone->zone_weather = 0;
		zone->weather_intensity = 0;

		if (serverwide == 1)
		{
			auto pack = new ServerPacket(ServerOP_Weather, sizeof(ServerWeather_Struct));
			ServerWeather_Struct* ws = (ServerWeather_Struct*)pack->pBuffer;
			ws->type = 0;
			ws->intensity = 0;
			ws->timer = 0;
			worldserver.SendPacket(pack);
			safe_delete(pack);
		}
		else
		{
			zone->weatherSend();
		}
	}
	if(zone->zone_weather == 0)
	{
		uint8 intensity = 3;
		uint16 timer = 0;
		if (sep->IsNumber(3) && sep->arg[4][0] != 0)
			timer = atoi(sep->arg[4]) * 60;

		// Snow
		if (weather_type == 2)
		{
			if(timer > 0)
				c->Message(Chat::Yellow, "Changing weather to snow for %d seconds.", timer);
			else
				c->Message(Chat::Yellow, "Changing weather to snow until the next cycle.");
			zone->zone_weather = 2;
			zone->weather_intensity = intensity;

			if (serverwide == 1)
			{
				auto pack = new ServerPacket(ServerOP_Weather, sizeof(ServerWeather_Struct));
				ServerWeather_Struct* ws = (ServerWeather_Struct*)pack->pBuffer;
				ws->type = zone->zone_weather;
				ws->intensity = intensity;
				ws->timer = timer;
				worldserver.SendPacket(pack);
				safe_delete(pack);
			}
			else
			{

				zone->weatherSend(timer*1000);
			}

			return;
		}
		// Rain
		else if (weather_type == 1)
		{
			if(timer > 0)
				c->Message(Chat::Yellow, "Changing weather to rain for %d seconds.", timer);
			else
				c->Message(Chat::Yellow, "Changing weather to rain until the next cycle.");
			zone->zone_weather = 1;
			zone->weather_intensity = intensity;

			if (serverwide == 1)
			{
				auto pack = new ServerPacket(ServerOP_Weather, sizeof(ServerWeather_Struct));
				ServerWeather_Struct* ws = (ServerWeather_Struct*)pack->pBuffer;
				ws->type = zone->zone_weather;
				ws->intensity = intensity;
				ws->timer = timer;
				worldserver.SendPacket(pack);
				safe_delete(pack);
			}
			else
			{
				zone->weatherSend(timer*1000);
			}

			return;
		}
	}
	else
	{
		if(weather_type != 0)
		{
			c->Message(Chat::Yellow, "You cannot change from one type of weather to another without first stopping it.");
			return;
		}
	}
}

