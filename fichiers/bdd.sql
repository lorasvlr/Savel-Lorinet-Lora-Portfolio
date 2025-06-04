/* DROP CONSTRAINT takes_part_fk1;
DROP CONSTRAINT takes_part_fk2;
DROP CONSTRAINT takes_part_fk3;
DROP CONSTRAINT belongs_to_fk1;
DROP CONSTRAINT belongs_to_fk2;
DROP CONSTRAINT fin_fk1;
DROP CONSTRAINT fin_fk2;
DROP CONSTRAINT fin_fk3;
DROP CONSTRAINT start_fk1;
DROP CONSTRAINT start_fk2;
DROP CONSTRAINT start_fk3;
DROP CONSTRAINT match_fk1;
DROP CONSTRAINT match_fk2;
DROP CONSTRAINT match_fk3;
DROP CONSTRAINT characteristics_fk1;
DROP CONSTRAINT characteristics_fk2;
DROP CONSTRAINT plays_fk1;
DROP CONSTRAINT plays_fk2;
DROP VAR day;
DROP VAR season;
DROP VAR club;
DROP VAR team;
DROP VAR league;
DROP VAR date;
DROP VAR player;
DROP VAR plays;
DROP VAR characteristics;
DROP VAR match;
DROP VAR start;
DROP VAR fin;
DROP VAR belongs_to;
DROP VAR takes_part; */



VAR day BASE RELATION{
day_nr INTEGER,
season_id CHARACTER}KEY{day_nr, season_id};


VAR season BASE RELATION{
season_id CHARACTER}KEY{season_id};


VAR club BASE RELATION{
club_id INTEGER,
club_name CHARACTER,
club_acronym CHARACTER}KEY{club_id};


VAR team BASE RELATION{
team_id INTEGER,
team_name CHARACTER,
coach_name_firstname CHARACTER}KEY{team_id};


VAR league BASE RELATION{
league_id INTEGER,
league_name CHARACTER}KEY{league_id};


VAR date BASE RELATION{
date INTEGER}KEY{date};


VAR player BASE RELATION{
player_id INTEGER,
player_name CHARACTER,
player_firstname CHARACTER,
date_of_birth CHARACTER,
nationality CHARACTER,
birth_country CHARACTER,
weight INTEGER,
size INTEGER}KEY{player_id};


VAR plays BASE RELATION{
player_id INTEGER,
season_id CHARACTER,
day_nr INTEGER,
position CHARACTER,
starting_time INTEGER,
yellow_cards INTEGER,
red_cards BOOLEAN,
shirt_nr INTEGER}KEY{player_id,season_id,day_nr};

CONSTRAINT plays_fk1
plays{player_id}<=player{player_id};
CONSTRAINT plays_fk2
plays{day_nr,season_id}<=day{day_nr,season_id};


VAR characteristics BASE RELATION{
season_id CHARACTER,
league_id INTEGER,
number_of_teams INTEGER}KEY{season_id, league_id};

CONSTRAINT characteristics_fk1
characteristics{season_id}<=season{season_id};
CONSTRAINT characteristics_fk2
characteristics{league_id}<=league{league_id};


VAR match BASE RELATION{
attendance INTEGER,
local_goals INTEGER,
visitor_goals INTEGER,
match_date_time INTEGER,
local INTEGER,
visitor INTEGER,
day_nr INTEGER,
season_id CHARACTER}KEY{day_nr,season_id,local,visitor};

CONSTRAINT match_fk1
match{local}rename{local as team_id}<=team{team_id};
CONSTRAINT match_fk2
match{day_nr,season_id}<=day{day_nr,season_id};
CONSTRAINT match_fk3
match{visitor}rename{visitor as team_id}<=team{team_id};


VAR start BASE RELATION{
player_id INTEGER,
date INTEGER,
team_id INTEGER}KEY{player_id,date,team_id};

CONSTRAINT start_fk1
start{team_id}<=team{team_id};
CONSTRAINT start_fk2
start{date}<=DATE{date};
CONSTRAINT start_fk3
start{player_id}<=player{player_id};


var fin BASE RELATION{
player_id INTEGER,
date INTEGER,
team_id INTEGER}KEY{player_id,date,team_id};

CONSTRAINT fin_fk1
fin{team_id}<=team{team_id};
CONSTRAINT fin_fk2
fin{date}<=date{date};
CONSTRAINT fin_fk3
fin{player_id}<=player{player_id};


VAR belongs_to BASE RELATION{
club_id INTEGER,
team_id INTEGER}KEY{club_id,team_id};

CONSTRAINT belongs_to_fk1
belongs_to{club_id}<=club{club_id};
CONSTRAINT belongs_to_fk2
belongs_to{team_id}<=team{team_id};


VAR takes_part BASE RELATION{
league_id INTEGER,
team_id INTEGER,
season_id CHARACTER}KEY{league_id,team_id,season_id};

CONSTRAINT takes_part_fk1
takes_part{league_id}<=league{league_id};
CONSTRAINT takes_part_fk2
takes_part{team_id}<=team{team_id};
CONSTRAINT takes_part_fk3
takes_part{season_id}<=season{season_id};
