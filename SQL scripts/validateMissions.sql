/*
Makes sure Mission IDs are entered correctly
*/

drop trigger if exists pda.validateMissions;

delimiter $$
	create trigger validateMissions before insert on Mission for each row 
		if (select count(*) from Mission)>0 then
			if new.MissionNumber-1 <> (select max(M.MissionNumber) from Mission M) then
				set new.MissionNumber = null;
			end if;
		end if;$$
delimiter ;