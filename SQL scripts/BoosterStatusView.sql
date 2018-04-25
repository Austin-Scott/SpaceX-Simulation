/*
Creates a view to look at overall Booster information
*/

create view BoosterStatus as 
select F.BoosterID, count(*) as TotalFlights, M.Title as MissionLastFLown, M.date as DateLastFlown, B.flightStatus
from flownby F, Mission M, Booster B
where F.BoosterID=B.BoosterID and M.MissionNumber = (select max(F2.MissionNumber)
													 from flownby F2
													 where F2.BoosterID=F.BoosterID)
group by BoosterID;