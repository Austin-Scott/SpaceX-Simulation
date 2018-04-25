/*
This script calculates some statistics about reliability
*/

drop procedure if exists getStatistics;

delimiter $$

create procedure getStatistics()
begin
	
    declare averageSuccessfulLaunches, averageSuccessfulLandings real;
    set averageSuccessfulLaunches = (select count(*) from Mission M where M.LaunchSuccess=true)/(select count(*) from Mission);
    set averageSuccessfulLandings = (select count(*) from FlownBy F where F.LandingSuccess=true)/(select count(*) from FlownBy);
    select averageSuccessfulLaunches, averageSuccessfulLandings;
end$$

delimiter ;

call getStatistics();