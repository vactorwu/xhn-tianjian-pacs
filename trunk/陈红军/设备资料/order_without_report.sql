//������������Ŀ�ޱ�����Ŀ��������Ŀ
select * from clinic_item_dict
where item_class='C' and item_code not in
 (select distinct order_item_code  from lab_order_vs_report);

select * from comm.clinic_item_dict_bak
where item_class='C' and item_code not in
 (select distinct order_item_code  from lab_order_vs_report);
// lab_sheet_master ������Ϊlab_sheet_id
// lab_sheet_items ������Ϊlab_sheet_id ��lab_item_code(Ϊ������Ŀ����)
//clinic_vs_charge ������Ϊ


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
where item_name like '���鼡��%';

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
where item_name like 'Ѫ���ἤø%';
select * from current_price_list 
where item_name like '����Ѫ��%';

select * from clinic_item_dict
where item_name like '�ͺ�2h%';
select * from clinic_item_dict
where item_code like 'JY27030';
select * from lab_order_vs_report
where order_item_code='62.23308';
select * from lab_report_item_dict
where item_name like 'Ѫ����%';


select * from clinic_item_name_dict
where item_name like '�ͺ�2h%';
select * from price_list
where item_name like '�ͺ�2h%';
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
select  *  from  current_price_list where item_name like '�����������ø%';
select price_item_name_dict.item_code,price_item_name_dict.item_name,
current_price_list.item_code from price_item_name_dict,curret_price_list
where (price_item_name_dict.item_name = curret_price_list.Item_name)
	and (price_item_name_dict.item_code <> curret_price_list.Item_code);
//���ֵ�̶��ı�����Ŀ�Ľ������Ҫ��LAB_RESULT_TYPE_VS_VALUES�ж��壡����
select * from lab_report_item_dict
where item_name like '�¿�%';
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