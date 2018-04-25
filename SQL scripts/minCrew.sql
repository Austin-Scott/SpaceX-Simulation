/*
Sets minimum number of crew members on BFR flights to be 10.
*/

drop trigger if exists pda.minCrew;

delimiter $$
	create trigger minCrew before insert on Payload for each row 
		if new.DragonSerial <> null and new.CrewMembers<10 and exists (select * from Dragon D where D.BFS=1 and D.SerialNumber = new.DragonSerial) then
			set new.CrewMembers=10;
		end if;$$
delimiter ;