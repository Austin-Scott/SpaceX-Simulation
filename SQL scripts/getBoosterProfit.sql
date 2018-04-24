/*
This script calculates booster finances, and stores it into a table called BoosterProfit.
Accounts for Booster construction costs, launch costs, and profit from customer payments.
Customer only pays if launch is successful.
*/

drop table if exists BoosterProfit;
create table BoosterProfit (
	BoosterID char(5) not null,
    successfulFlights integer,
    cost integer,
    grossProfit integer,
    netProfit integer,
    primary key(BoosterID)
);

drop procedure if exists getBoosterProfit;
delimiter $$

create procedure getBoosterProfit()
begin
	declare done int default 0;
    declare id char(5);
    declare flights, successfulFlights, blockNum, cost, profit int;
    declare cur cursor for select F.BoosterID, count(*), B.BlockNumber 
		from Booster B, flownby F 
        where B.BoosterID=F.BoosterID 
		group by F.BoosterID;
    declare continue handler for not found set done = 1;
    open cur;
    repeat
		fetch cur into id, flights, blockNum;
        if not done then
			if exists (select * from Mission M, flownby F where F.MissionNumber=M.MissionNumber and F.BoosterID=id and M.launchSuccess=false) then
				/*A booster can only ever have one unsuccessful flight*/
				set successfulFlights=flights-1;
			else 
				set successfulFlights=flights;
			end if;
			if blockNum=0 then
				/*Falcon 1 costs $5 million to build and $2 million to launch. Customer is charged $9 million*/
				set cost = (flights*2)+5;
                set profit = successfulFlights*9;
			elseif blockNum<5 then
				/*Falcon 9 Block 1-4 costs $27 million to build and $37 million to launch. Customer is charged $70 million*/
				set cost = (flights*37)+27;
                set profit = successfulFlights*70;
			elseif blockNum=5 then
				/*Falcon 9 Block 5 costs $41 million to build and $33 million to launch. Customer is charged $60 million*/
				set cost = (flights*33)+41;
                set profit = successfulFlights*60;
			else
				/*BFR costs $335 million to build but only $4 million to launch. Customer is charged $10 million*/
				set cost = (flights*4)+335;
                set profit = successfulFlights*10;
			end if;
            insert into BoosterProfit values (id, successfulFlights, cost, profit, profit-cost);
		end if;
	until done end repeat;
    close cur;
end$$
delimiter ;

call getBoosterProfit();

select * from BoosterProfit;
        
            