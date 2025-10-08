struct PlayerStartLocationInfo
{
	// in
	int race;
	int classnum;
	int deity;
	int city_ix; // this is from character create struct, index of the button

	// out
	int zone_id;
	float x, y, z, heading;
	int bind_zone_id;
	float bind_x, bind_y, bind_z;
};

void FillPlayerStartLocationInfo(PlayerStartLocationInfo *i);
