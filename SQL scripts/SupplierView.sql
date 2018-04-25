/*
Creates a view for payload supplier information
*/

create view Supplier as
select P.Supplier, count(*) as PayloadsFlown, avg(P.PayloadMass) as AveragePayloadMass, sum(P.CrewMembers) as TotalCrewFlown
from Payload P
group by P.Supplier;