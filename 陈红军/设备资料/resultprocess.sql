
//result_status 为1收到申请， 3初步报告， 4最终报告

//有结果未确认的；
select * from lab_test_master where result_status=3
and  lab_test_master.results_rpt_date_time >=to_date ('00122500','yymmddhh24')
and lab_test_master.results_rpt_date_time <=to_date ('01010423','yymmddhh24')
and test_no  in (select distinct test_no from lab_result);

//由申请无结果的
select * from lab_test_master where result_status=1
and  lab_test_master.results_rpt_date_time >=to_date ('00122500','yymmddhh24')
and lab_test_master.results_rpt_date_time <=to_date ('01010423','yymmddhh24')
and test_no not in (select distinct test_no from lab_result);

//将指定工作单号的结果从临时表导入到结果表，并置结果状态为初步报告！

//1-将临时结果表中2000年12月25日 工作单号为B71的结果插入结果表！
insert into lab_result
select test_no,item_no,print_order,report_item_name,report_item_code,result,
units,abnormal_indicator,instrument_id,result_date_time
from lab_test_master ,lab_result_temp 
where  lab_test_master.working_id=lab_result_temp.working_id and lab_result_temp.Working_id='B71'
and lab_test_master.results_rpt_date_time >=to_date ('00122500','yymmddhh24')
and lab_test_master.results_rpt_date_time <=to_date ('00122523','yymmddhh24')
and lab_result_temp.result_date_time>=to_date ('00122500','yymmddhh24')
and lab_result_temp.and result_date_time<=to_date ('00122523','yymmddhh24');
//2-将结果状态置为初步报告！
update lab_test_master set result_status=3
where  lab_test_master.working_id=lab_result_temp.working_id and lab_result_temp.Working_id='B71'
and lab_test_master.results_rpt_date_time >=to_date ('00122500','yymmddhh24')
and lab_test_master.results_rpt_date_time <=to_date ('00122523','yymmddhh24');
and lab_test_master.test_no not in( select distinct test_no from lab_result);

//3-清除临时结果表中的相应记录
delete from lab_result_temp
where working_id='B71' and result_date_time>=to_date ('00122500','yymmddhh24')
and result_date_time<=to_date ('00122523','yymmddhh24');

//4-提交修改到数据库
commit;