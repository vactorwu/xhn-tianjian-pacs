//查找有申请项目无报告项目的诊疗项目
select * from clinic_item_dict
where item_class='C' and item_code not in
 (select distinct order_item_code  from lab_order_vs_report);

select * from comm.clinic_item_dict_bak
where item_class='C' and item_code not in
 (select distinct order_item_code  from lab_order_vs_report);
// lab_sheet_master 的主键为lab_sheet_id
// lab_sheet_items 的主键为lab_sheet_id 和lab_item_code(为诊疗项目代码)
//clinic_vs_charge 的主键为


select distinct clinic_item_dict.item_code,
clinic_item_dict.item_name ,
clinic_item_name_dict.item_code
 from clinic_item_dict,clinic_item_name_dict 
where clinic_item_dict.item_name=clinic_item_name_dict.item_name
 and clinic_item_dict.item_class='C' and 
clinic_item_name_dict.item_code<>clinic_item_dict.item_code;

select * from lab_order_vs_report
where order_item_code='JY1095';//'JY2737';
select * from clinic_item_dict where item_code='JY1095';
select * from clinic_item_name_dict where item_code='JY1095';
select * from clinic_item_dict where item_class='C';
select * from clinic_item_name_dict where item_class='C';

select distinct clinic_item_dict.item_code,
clinic_item_dict.item_name ,
clinic_item_name_dict.item_code
 from clinic_item_dict,clinic_item_name_dict 
where clinic_item_dict.item_name=clinic_item_name_dict.item_name
 and clinic_item_dict.item_class='C' and 
clinic_item_name_dict.item_code<>clinic_item_dict.item_code;
select * from clinic_item_name_dict
where item_name=(
select item_NAME from clinic_item_dict where item_code='02.01182'
);
select * from clinic_item_dict
where item_name like '急查肌酸%';

select item_NAME from clinic_item_dict where item_code='JCX000001'
select * from clinic_item_name_dict
where input_code like 'KLOCD%';
select * from clinic_item_dict
where input_code like 'KLOCD%';
select * from clinic_vs_charge
where clinic_item_code='JY1073';
select * from current_price_list 
where item_name like '%';
select * from price_list 
where item_name like 'JCGX%';
select * from clinic_vs_charge
where clinic_item_code='JY1073';
select * from clinic_vs_charge
where clinic_item_code='JY4013';

select * from current_price_list 
where item_name like '血肌酸激酶%';
select * from current_price_list 
where item_name like '急查血谷%';

select * from clinic_item_dict
where item_name like '餐后2h%';
select * from clinic_item_dict
where item_code like 'JY27030';
select * from lab_order_vs_report
where order_item_code='62.23308';
select * from lab_report_item_dict
where item_name like '血肌酸%';


select * from clinic_item_name_dict
where item_name like '餐后2h%';
select * from price_list
where item_name like '餐后2h%';
select * from clinic_vs_charge;
where clinic_item_code='62.23308';
select * from price_item_name_dict  where item_class='C' and input_code like'JCGB%';

select * from lab_order_vs_report
where order_item_code='JY1081';
select item_name from clinic_item_name_dict
where item_code='JY1081';
select * from lab_report_item_dict where item_code='JY1081';
select * from clinic_vs_charge
where clinic_item_code='JY1081';
select  *  from  current_price_list where item_name like '急查碱性磷酸酶%';
select price_item_name_dict.item_code,price_item_name_dict.item_name,
current_price_list.item_code from price_item_name_dict,curret_price_list
where (price_item_name_dict.item_name = curret_price_list.Item_name)
	and (price_item_name_dict.item_code <> curret_price_list.Item_code);
//结果值固定的报告项目的结果类型要在LAB_RESULT_TYPE_VS_VALUES有定义！！！
select * from lab_report_item_dict
where item_name like '奥抗%';
//select * from clinic_item_name_dict where item_class='C';
select * from lab_result_type_vs_values;


 SELECT "LAB"."INSTRUMENT_CONFIG"."INSTRUMENT_ID",   
         "LAB"."INSTRUMENT_CONFIG"."INSTRUMENT_ITEM_CODE",   
         "LAB"."INSTRUMENT_CONFIG"."REPORT_ITEM_CODE",   
         "COMM"."LAB_REPORT_ITEM_DICT"."ITEM_NAME"  
    FROM "COMM"."LAB_REPORT_ITEM_DICT",   
         "LAB"."INSTRUMENT_CONFIG"  
   WHERE ( COMM."LAB_REPORT_ITEM_DICT"."ITEM_CODE" = LAB."INSTRUMENT_CONFIG"."REPORT_ITEM_CODE" );
select * from instrument_dict;

 SELECT "LAB"."INSTRUMENT_CONFIG"."INSTRUMENT_ID",   
         "LAB"."INSTRUMENT_CONFIG"."INSTRUMENT_ITEM_CODE",   
         "LAB"."INSTRUMENT_CONFIG"."REPORT_ITEM_CODE",   
         "COMM"."LAB_REPORT_ITEM_DICT"."ITEM_NAME"  
    FROM "COMM"."LAB_REPORT_ITEM_DICT",   
         "LAB"."INSTRUMENT_CONFIG"  
   WHERE ( COMM."LAB_REPORT_ITEM_DICT"."ITEM_CODE" = LAB."INSTRUMENT_CONFIG"."REPORT_ITEM_CODE" )
order by  "LAB"."INSTRUMENT_CONFIG"."INSTRUMENT_ID";

select * from instrument_config order by INSTRUMENT_ID;
select * from instrument_config where   INSTRUMENT_ID='ACT10';
select * from instrument_config where   INSTRUMENT_ID='MIDTRON';