//select * from lab_result_temp  where  Working_id like 'B%' and
//result_date_time >=to_date ('00122500','yymmddhh24')
//and result_date_time <=to_date ('00122523','yymmddhh24');
//select * from lab_test_master where working_id in 
//(select working_id from lab_result_temp  where  Working_id like 'B56' and
//result_date_time >=to_date ('00122500','yymmddhh24')
//and result_date_time <=to_date ('00122523','yymmddhh24')) and 
//results_rpt_date_time >=to_date ('00122500','yymmddhh24')
//and results_rpt_date_time <=to_date ('00122523','yymmddhh24');
////
//select * from lab_result_temp where working_id='A115';
//select * from lab_test_master where result_status ='1' and working_id like 'B%'
//and results_rpt_date_time >=to_date ('00122500','yymmddhh24')
//and results_rpt_date_time <=to_date ('00122523','yymmddhh24');
//select * from lab_result where test_no='001226%';
//select * from lab_test_master where working_id like 'B%' and results_rpt_date_time >=to_date ('00122500','yymmddhh24')
//and results_rpt_date_time <=to_date ('00122523','yymmddhh24');
//select * from lab_result where test_no='0012251381';
////insert into lab_result
//select test_no,item_no,print_order,report_item_name,report_item_code,result,
//units,abnormal_indicator,instrument_id,result_date_time
//from lab_test_master ,lab_result_temp 
//where  lab_test_master.working_id=lab_result_temp.working_id and lab_result_temp.instrument_id='CX4'
//and lab_test_master.results_rpt_date_time >=to_date ('00122500','yymmddhh24')
//and lab_test_master.results_rpt_date_time <=to_date ('00122523','yymmddhh24');
//and lab_test_master.test_no not in( select distinct test_no from lab_result);
////commit;
//select distinct test_no
//from lab_test_master ,lab_result_temp 
//where  lab_test_master.working_id=lab_result_temp.working_id and lab_result_temp.instrument_id='CX5' 
//and lab_test_master.results_rpt_date_time >=to_date ('00122500','yymmddhh24')
//and lab_test_master.results_rpt_date_time <=to_date ('00122523','yymmddhh24')
//and lab_test_master.test_no not in( select distinct test_no from lab_result);
//SELECT NAME,WORKING_ID, test_no,item_no,print_order,report_item_name,report_item_code,result,
//units,abnormal_indicator,instrument_id,result_date_time FROM LAB_RESULT ,LAB_TEST_MASTER
////WHERE LAB_TEST_MASTER.test_no = LAB_RESULT.TEST_NO AND LAB_TEST_MASTER.WORKING_ID LIKE 'B%';
//select * from lab_test_master where working_id LIKE 'C00001' ;
//select * from lab_result where test_no='0012283044';
//select * from lab_result_temp  where working id='A00038';
//select * from lab_test_items where test_no= '0012283044';//'0012283103';
//select * from dept_dict;
//SELECT * FROM LAB_RESULT WHERE TEST_NO='0012282532';
SELECT * FROM LAB_RESULT_TEMP where working_id  LIKE 'C00888';