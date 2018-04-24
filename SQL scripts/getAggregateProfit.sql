/*
This script adds up the net profit columns in BoosterProfit and DragonProfit to give a running total company profit in million of dollars.
*/

drop procedure if exists getAggregateProfit;

delimiter $$

create procedure getAggregateProfit()
begin
	declare totalBoosterProfit, totalDragonProfit int;
    delete from BoosterProfit;
    delete from DragonProfit;
	call getBoosterProfit();
    call getDragonProfit();
    set totalBoosterProfit = (select sum(B.NetProfit) from BoosterProfit B);
    set totalDragonProfit = (select sum(D.NetProfit) from DragonProfit D);
    select totalBoosterProfit, totalDragonProfit, (totalBoosterProfit+totalDragonProfit);
end$$

delimiter ;

call getAggregateProfit();