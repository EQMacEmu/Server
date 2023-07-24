alter table spells_new change `field124` `disallow_sit` int(11) not null default 0;
alter table spells_new change `field125` `deities0` int(11) not null default 0;
alter table spells_new change `field142` `npc_no_cast` int(11) not null default 0;
alter table spells_new change `field143` `ai_pt_bonus` int(11) not null default 0;
alter table spells_new change `field152` `small_targets_only` int(11) not null default 0;
alter table spells_new change `field153` `use_persistent_particles` int(11) not null default 0;
update spells_new set field158 = 0;
alter table spells_new change `field158` `effectdescnum2` int(11) not null default 0;

alter table spells_new change `field163` `resist_per_level` int(11) not null default 0;
alter table spells_new change `field164` `resist_cap` int(11) not null default 0;
alter table spells_new change `field181` `pvp_duration` int(11) not null default 0;
alter table spells_new change `field182` `pvp_duration_cap` int(11) not null default 0;
alter table spells_new change `field184` `cast_not_standing` int(11) not null default 0;
alter table spells_new change `field199` `wear_off_message` int(11) not null default 0;
alter table spells_new change `field210` `allow_spellscribe` int(11) not null default 0;

alter table spells_new change `pushback` `pushback` float(14,2) NOT NULL default '0';
alter table spells_new change `pushup` `pushup` float(14,2) NOT NULL default '0';

alter table spells_new drop column `field160`;
alter table spells_new drop column `field165`;
alter table spells_new drop column `field169`;
alter table spells_new drop column `field170`;
alter table spells_new drop column `field171`;
alter table spells_new drop column `field172`;
alter table spells_new drop column `field183`;
alter table spells_new drop column `field193`;
alter table spells_new drop column `field196`;
alter table spells_new drop column `field198`;
alter table spells_new drop column `field203`;
alter table spells_new drop column `field204`;
alter table spells_new drop column `field205`;
alter table spells_new drop column `field206`;
alter table spells_new drop column `field209`;
alter table spells_new drop column `field215`;
alter table spells_new drop column `field216`;
alter table spells_new drop column `field217`;
alter table spells_new drop column `field220`;
alter table spells_new drop column `field221`;
alter table spells_new drop column `field222`;
alter table spells_new drop column `field223`;
alter table spells_new drop column `field225`;
alter table spells_new drop column `field226`;
alter table spells_new drop column `field232`;
alter table spells_new drop column `field233`;
alter table spells_new drop column `field234`;
alter table spells_new drop column `field235`;
alter table spells_new drop column `field236`;

alter table spells_new drop column `classes16`;
alter table spells_new drop column `viral_targets`;
alter table spells_new drop column `viral_timer`;
alter table spells_new drop column `viral_range`;
alter table spells_new drop column `songcap`;
alter table spells_new drop column `bonushate`;
alter table spells_new drop column `MinResist`;
alter table spells_new drop column `MaxResist`;
alter table spells_new drop column `ConeStartAngle`;
alter table spells_new drop column `ConeStopAngle`;
alter table spells_new drop column `not_extendable`;
alter table spells_new drop column `rank`;
alter table spells_new drop column `CastRestriction`;
alter table spells_new drop column `aemaxtargets`;
alter table spells_new drop column `maxtargets`;
alter table spells_new drop column `persistdeath`;
alter table spells_new drop column `min_dist`;
alter table spells_new drop column `min_dist_mod`;
alter table spells_new drop column `max_dist`;
alter table spells_new drop column `max_dist_mod`;
alter table spells_new drop column `min_range`;
alter table spells_new drop column `InCombat`;
alter table spells_new drop column `OutOfCombat`;

update spells_new sn
inner join spells_en sen ON sen.id = sn.id
set sn.pushup = sen.pushup
where sn.pushup != sen.pushup;

update spells_new sn
inner join spells_en sen ON sen.id = sn.id
set sn.pushback = sen.pushback
where sn.pushback != sen.pushback;

update spells_new sn
inner join spells_en sen ON sen.id = sn.id
set sn.descnum = sen.descnum
where sn.descnum != sen.descnum;

update spells_new sn
inner join spells_en sen ON sen.id = sn.id
set sn.typedescnum = sen.typedescnum
where sn.typedescnum != sen.typedescnum;

update spells_new sn
inner join spells_en sen ON sen.id = sn.id
set sn.effectdescnum = sen.effectdescnum
where sn.effectdescnum != sen.effectdescnum;

update spells_new sn
inner join spells_en sen ON sen.id = sn.id
set sn.effectdescnum2 = sen.effectdescnum2
where sn.effectdescnum2 != sen.effectdescnum2;