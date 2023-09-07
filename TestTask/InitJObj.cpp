#include "InitJObj.h"
#include <iostream>

void init_jobj()
{
//�������� root ������=================================================================
	//������ ����������� � ������������� ������ ��� ��������
	Var_value_t var_value = { 0 };
	var_value.number = 777;
	//�������������� ��������� ������� ��������� ��������  
	//1:��� ��������, 2:����, 3:��������
	Value_t value = { NUMBER, "value one", var_value};
	//������ ����������� ������ �� ���������
	//1:��� ������������ �������, 2: �������� ��� ��������� � ������
	//(����� ���� ������ ���� ��� meaning J_NULL)
	Cmx_obj_t* root_obj = make_cmxobj(OBJ, value);
	//������ �������� 
	
	//�������������� ��������
	var_value.float_number = 24.33;
	value = {REAL_NUMBER,"value two", var_value};
	if (add_value(root_obj, value))				//��������� �������� � ������
		std::cout << "Error \"" << value.key << "\"\n";

	var_value.boolean = true;
	value = { BOOLEAN,"value three", var_value };
	if (add_value(root_obj, value))				//��������� �������� � ������
		std::cout << "Error \"" << value.key << "\"\n";

	var_value.string = "Value string";
	value = {STRING, "value four", var_value };
	if (add_value(root_obj, value))				//��������� �������� � ������
		std::cout << "Error \"" << value.key << "\"\n";

//�������� ������=============================================================================
	//������ ������ � ������ ���������
	var_value.string = "element array one";
	value = { STRING, NULL, var_value };		//���� �������� �� ����� ������ ��� ��� ������
	Cmx_obj_t* arr = make_cmxobj(ARR, value);

	var_value.string = "element array two";
	value = { STRING, NULL, var_value };
	if (add_value(arr, value))				//��������� �������� � ������
		std::cout << "Error \"" << "Element array" << "\"\n";

	var_value.string = "element array three";
	value = { STRING, NULL, var_value };
	if (add_value(arr, value))				//��������� �������� � ������
		std::cout << "Error \"" << "Element array" << "\"\n";

	var_value.string = "element array four";
	value = { STRING, NULL, var_value };
	if (add_value(arr, value))				//��������� �������� � ������
		std::cout << "Error \"" << "Element array" << "\"\n";

	var_value.string = "element array fife";
	value = { STRING, NULL, var_value };
	if (add_value(arr, value))				//��������� �������� � ������
		std::cout << "Error \"" << "Element array" << "\"\n";

//������ �������� root �����=====================================================
	//��������� ��������� ������ � ������
	var_value.cmx_obj = *arr;
	value = { CMX_OBJ, "Array one", var_value };
	if (add_value(root_obj, value))				//��������� �������� � ������
		std::cout << "Error \"" << value.key << "\"\n";

//�������� ����� ������===============================================================
	//������ ����� ������
	var_value.number = 333;
	value = { NUMBER, "New obj value one", var_value };
	Cmx_obj_t* new_obj = make_cmxobj(OBJ, value);

	//�������������� ��������
	var_value.float_number = 44.63;
	value = { REAL_NUMBER,"New obj value two", var_value };
	if (add_value(new_obj, value))				//��������� �������� � ������
		std::cout << "Error \"" << value.key << "\"\n";

	var_value.boolean = false;
	value = { BOOLEAN,"New obj value three", var_value };
	if (add_value(new_obj, value))				//��������� �������� � ������
		std::cout << "Error \"" << value.key << "\"\n";

	var_value.string = "Hello!";
	value = { STRING, "New obj value four", var_value };
	if (add_value(new_obj, value))				//��������� �������� � ������
		std::cout << "Error \"" << value.key << "\"\n";

//������ �������� root �����=====================================================
	//��������� ��������� ������ � root ������
	var_value.cmx_obj = *new_obj;
	value = { CMX_OBJ, "New obj", var_value };
	if (add_value(root_obj, value))				//��������� �������� � ������
		std::cout << "Error \"" << value.key << "\"\n";

	//��������� �������� null
	var_value.null = j_null;
	value = { J_NULL, "Empty value", var_value };
	if (add_value(root_obj, value))				//��������� �������� � ������
		std::cout << "Error \"" << value.key << "\"\n";


//������ ������ �������=====================================================================
//������ ������
	var_value.string = "88";
	value = { STRING, "id", var_value };
	Cmx_obj_t* test_obj1 = make_cmxobj(OBJ, value);

	var_value.string = "test 88";
	value = { STRING, "pagetitle", var_value };
	if (add_value(test_obj1, value))				//��������� �������� � ������
		std::cout << "Error \"" << value.key << "\"\n";

	var_value.string = "";
	value = { STRING, "longtitle", var_value };
	if (add_value(test_obj1, value))				//��������� �������� � ������
		std::cout << "Error \"" << value.key << "\"\n";

	var_value.string = "��������";
	value = { STRING, "description", var_value };
	if (add_value(test_obj1, value))				//��������� �������� � ������
		std::cout << "Error \"" << value.key << "\"\n";

	var_value.string = "test/id88";
	value = { STRING, "alias", var_value };
	if (add_value(test_obj1, value))				//��������� �������� � ������
		std::cout << "Error \"" << value.key << "\"\n";

//������ 2========================================================================
	var_value.string = "99";
	value = { STRING, "id", var_value };
	Cmx_obj_t* test_obj2 = make_cmxobj(OBJ, value);

	var_value.string = "test 88";
	value = { STRING, "pagetitle", var_value };
	if (add_value(test_obj2, value))				//��������� �������� � ������
		std::cout << "Error \"" << value.key << "\"\n";

	var_value.string = "";
	value = { STRING, "longtitle", var_value };
	if (add_value(test_obj2, value))				//��������� �������� � ������
		std::cout << "Error \"" << value.key << "\"\n";

	var_value.string = "��������";
	value = { STRING, "description", var_value };
	if (add_value(test_obj2, value))				//��������� �������� � ������
		std::cout << "Error \"" << value.key << "\"\n";

	var_value.string = "test/id99";
	value = { STRING, "alias", var_value };
	if (add_value(test_obj2, value))				//��������� �������� � ������
		std::cout << "Error \"" << value.key << "\"\n";

//������ 3========================================================================

	var_value.string = "150";
	value = { STRING, "id", var_value };
	Cmx_obj_t* test_obj3 = make_cmxobj(OBJ, value);

	var_value.string = "test 88";
	value = { STRING, "pagetitle", var_value };
	if (add_value(test_obj3, value))				//��������� �������� � ������
		std::cout << "Error \"" << value.key << "\"\n";

	var_value.string = "";
	value = { STRING, "longtitle", var_value };
	if (add_value(test_obj3, value))				//��������� �������� � ������
		std::cout << "Error \"" << value.key << "\"\n";

	var_value.string = "��������";
	value = { STRING, "description", var_value };
	if (add_value(test_obj3, value))				//��������� �������� � ������
		std::cout << "Error \"" << value.key << "\"\n";

	var_value.string = "test/id150";
	value = { STRING, "alias", var_value };
	if (add_value(test_obj3, value))				//��������� �������� � ������
		std::cout << "Error \"" << value.key << "\"\n";

//������ 4========================================================================

	var_value.string = "350";
	value = { STRING, "id", var_value };
	Cmx_obj_t* test_obj4 = make_cmxobj(OBJ, value);

	var_value.string = "test 88";
	value = { STRING, "pagetitle", var_value };
	if (add_value(test_obj4, value))				//��������� �������� � ������
		std::cout << "Error \"" << value.key << "\"\n";

	var_value.string = "";
	value = { STRING, "longtitle", var_value };
	if (add_value(test_obj4, value))				//��������� �������� � ������
		std::cout << "Error \"" << value.key << "\"\n";

	var_value.string = "��������";
	value = { STRING, "description", var_value };
	if (add_value(test_obj4, value))				//��������� �������� � ������
		std::cout << "Error \"" << value.key << "\"\n";

	var_value.string = "test/id350";
	value = { STRING, "alias", var_value };
	if (add_value(test_obj4, value))				//��������� �������� � ������
		std::cout << "Error \"" << value.key << "\"\n";

//�������� ������� � ���������� �������� � ����========================================================================
	var_value.cmx_obj = *test_obj1;
	value = { CMX_OBJ, NULL, var_value };
	Cmx_obj_t* test_arr = make_cmxobj(ARR, value);

	var_value.cmx_obj = *test_obj2;
	value = { CMX_OBJ, NULL, var_value };
	if (add_value(test_arr, value))				//��������� �������� � ������
		std::cout << "Error \"" << value.key << "\"\n";

	var_value.cmx_obj = *test_obj3;
	value = { CMX_OBJ, NULL, var_value };
	if (add_value(test_arr, value))				//��������� �������� � ������
		std::cout << "Error \"" << value.key << "\"\n";

	var_value.cmx_obj = *test_obj4;
	value = { CMX_OBJ, NULL, var_value };
	if (add_value(test_arr, value))				//��������� �������� � ������
		std::cout << "Error \"" << value.key << "\"\n";

	var_value.cmx_obj = *test_arr;
	value = { CMX_OBJ, "Test array", var_value };
	if (add_value(root_obj, value))				//��������� �������� � ������
		std::cout << "Error \"" << value.key << "\"\n";

//������� ������������� ����������=========================================================================
	//����������� ����� � ������� �������� �� �����
	Cmx_obj_t* obj = find_values(root_obj, "new obj");

//������ ������� �������� "alias" �� ������ �������� � ������� �������� "id" == "350"
	{
		//���������� �������� �������
		for (uint8_t idx = 0; idx < test_arr->valCnt; ++idx)
		{
			//������� ���������� �������� �� ����������� ������� �� �����
			Value_t* val = find_value(&test_arr->value[idx].var_value.cmx_obj, "id");
			//���� �������� "id" == "350"
			if (val->var_value.string == "350")
			{
				//���� � ���� �B������ �������� � ������ "alias"
				val = find_value(&test_arr->value[idx].var_value.cmx_obj, "alias");
				//������� ��������
				printf("\"alias\" field value: %s",val->var_value.string);
			}
		}
	}
	
//�������� ��������� ��� ��������
	init_msgarr();			//�������� ������ ��� ������ ���������
	make_msg(root_obj);		//�������� ���������

//���������� ��������� �� �������
	Cmx_obj_t* my_obj = extract_msg(msg_arr);	//��������� ���������

//����������� ������
	free_obj(root_obj);
	free_obj(my_obj);
	free(msg_arr);
}