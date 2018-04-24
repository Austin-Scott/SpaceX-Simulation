/*
This script calculates Dragon Capsule/Big Falcon Spaceship finances, and stores it into a table called DragonProfit.
Accounts for Dragon/BFS construction costs, refurbishment costs, and profit from customer payments.
Customer only pays if launch is successful.
*/


drop table if exists DragonProfit;
create table DragonProfit (
	CapsuleSerial char(4) not null,
    SuccessfulFlights int,
    Cost int,
    GrossProfit int,
    NetProfit int,
    primary key(CapsuleSerial)
);

drop procedure if exists getDragonProfit;

delimiter $$

create procedure getDragonProfit()
begin
	declare done int default 0;
    declare id char(4);
    declare BFS boolean;
    declare flights, successfulFlights, cost, profit int;
    declare cur cursor for select P.DragonSerial, count(*), avg(D.BFS) from Payload P, Dragon D where P.DragonSerial=D.SerialNumber group by P.DragonSerial;
    declare continue handler for not found set done = 1;
    open cur;
    repeat
		fetch cur into id, flights, BFS;
        if not done then
			if exists (select * from Payload P, Mission M where P.DragonSerial=id and P.MissionNumber=M.MissionNumber and M.LaunchSuccess=0) then
				/*A Dragon/BFS can only ever have one unsuccessful flight*/
				set successfulFlights=flights-1;
			else 
				set successfulFlights=flights;
			end if;
			if BFS=0 then
				/*Dragon capsule costs $18 million to build and $3 million per flight for refurbishments. Customers can rent a Dragon capsule for $8 million per flight.*/
				set cost = (flights*3)+18;
                set profit = successfulFlights*8;
			else
				/*BFS costs $212 million to build and $1 million per flight to maintain. Customers pay $5 million for their BFS per flight.*/
                set cost = flights+212;
                set profit = successfulFlights*5;
			end if;
            insert into DragonProfit values (id, successfulFlights, cost, profit, profit-cost);
		end if;
	until done end repeat;
    close cur;
end$$

delimiter ;

call getDragonProfit();
select * from DragonProfit;