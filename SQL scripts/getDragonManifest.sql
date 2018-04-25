/*
This script gives you a manifest of everywhere a Dragon Capsule/Big Falcon Spaceship has gone.
*/

drop procedure if exists getDragonManifest;

delimiter $$

create procedure getDragonManifest(DragonCapsule char(4))
begin
	declare done, counter, crew, num int default 0;
    declare BFS boolean;
    declare destination, origin varchar(25);
    declare destName, originName varchar(255);
    declare dateOf Date;
    declare cur cursor for select P.DestinationSite, P.CrewMembers, M.LaunchSite, M.date, M.MissionNumber from Payload P, Mission M where P.MissionNumber=M.MissionNumber and P.DragonSerial=DragonCapsule order by M.MissionNumber asc;
    declare continue handler for not found set done = 1;
    
    create temporary table if not exists manifest (
		FlightNumber int not null,
        Origin varchar(255),
        Destination varchar(255),
        CrewMembers int,
        DateFlown date,
        primary key(FlightNumber)
	);
    delete from manifest;
    
    open cur;
    
     repeat
		fetch cur into destination, crew, origin, dateOf, num;
        if not done then
			set counter = counter + 1;
            set BFS = (select D.BFS from Dragon D where D.SerialNumber=DragonCapsule);
            set originName = (select L.Name from LaunchSite L where L.SiteID=origin);
            if BFS=1 then
				set destName = (select L.Name from LandingSite L where L.SiteID=destination);
			else
				set destName = (select distinct P.orbit from Payload P where P.MissionNumber=num and P.DragonSerial=DragonCapsule);
			end if;
            insert into manifest values ( counter, originName, destName, crew, dateOf );
		end if;
	until done end repeat;
    close cur;
    
    select * from manifest;
end$$

delimiter ;

call getDragonManifest('C314');