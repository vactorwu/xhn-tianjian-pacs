
//result_status Ϊ1�յ����룬 3�������棬 4���ձ���

//�н��δȷ�ϵģ�
select * from lab_test_master where result_status=3
and  lab_test_master.results_rpt_date_time >=to_date ('00122500','yymmddhh24')
and lab_test_master.results_rpt_date_time <=to_date ('01010423','yymmddhh24')
and test_no  in (select distinct test_no from lab_result);

//�������޽����
select * from lab_test_master where result_status=1
and  lab_test_master.results_rpt_date_time >=to_date ('00122500','yymmddhh24')
and lab_test_master.results_rpt_date_time <=to_date ('01010423','yymmddhh24')
and test_no not in (select distinct test_no from lab_result);

//��ָ���������ŵĽ������ʱ���뵽��������ý��״̬Ϊ�������棡

//1-����ʱ�������2000��12��25�� ��������ΪB71�Ľ����������
insert into lab_result
select test_no,item_no,print_order,report_item_name,report_item_code,result,
units,abnormal_indicator,instrument_id,result_date_time
from lab_test_master ,lab_result_temp 
where  lab_test_master.working_id=lab_result_temp.working_id and lab_result_temp.Working_id='B71'
and lab_test_master.results_rpt_date_time >=to_date ('00122500','yymmddhh24')
and lab_test_master.results_rpt_date_time <=to_date ('00122523','yymmddhh24')
and lab_result_temp.result_date_time>=to_date ('00122500','yymmddhh24')
and lab_result_temp.and result_date_time<=to_date ('00122523','yymmddhh24');
//2-�����״̬��Ϊ�������棡
update lab_test_master set result_status=3
where  lab_test_master.working_id=lab_result_temp.working_id and lab_result_temp.Working_id='B71'
and lab_test_master.results_rpt_date_time >=to_date ('00122500','yymmddhh24')
and lab_test_master.results_rpt_date_time <=to_date ('00122523','yymmddhh24');
and lab_test_master.test_no not in( select distinct test_no from lab_result);

//3-�����ʱ������е���Ӧ��¼
delete from lab_result_temp
where working_id='B71' and result_date_time>=to_date ('00122500','yymmddhh24')
and result_date_time<=to_date ('00122523','yymmddhh24');

//4-�ύ�޸ĵ����ݿ�
commit;