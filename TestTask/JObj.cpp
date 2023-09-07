#include "JObj.h"

//������� ��� �������� ������� ��� �������
//1:��� ������, 2:������ �������� � ������� ���������� ������,
//���� ��� �������� J_NULL �� ���������� ������ ������
//���������� ��������� �� ��������� ������
Cmx_obj_t* make_cmxobj(Type_cmxobj type, Value_t meaning)
{
	Cmx_obj_t* cmx_obj = (Cmx_obj_t*)malloc(sizeof(Cmx_obj_t));
	cmx_obj->type = type;
	cmx_obj->valCnt = 0;

	if (meaning.type == J_NULL)
		return cmx_obj;

	add_value(cmx_obj, meaning);

	return cmx_obj;
}

//������� ���������� �������� � ������
//1:��������� �� ������ � ������� ����������� ��������,
//2:�������� ����������� ���������� Value_t
//� ������ ������ ���������� ����, ����� ��� ������
uint8_t add_value(Cmx_obj_t* cmx_obj, Value_t meaning)
{
	cmx_obj->valCnt++;
	Value_t* tmpMeaning = (Value_t*)malloc(sizeof(Value_t) * cmx_obj->valCnt);

	if (!tmpMeaning)
		return MEMORY_ERROR;

	for (uint8_t idx = 0; idx < cmx_obj->valCnt - 1; ++idx)
	{
		if (cmx_obj->type == OBJ && cmx_obj->value[idx].key == meaning.key)
			return KEY_ERROR;

		tmpMeaning[idx] = cmx_obj->value[idx];
	}

	//���� �������� ����������� � ������, �� ���� �� �����
	if (cmx_obj->type == ARR)
		meaning.key = NULL;

	tmpMeaning[cmx_obj->valCnt - 1] = meaning;

	cmx_obj->value = tmpMeaning;

	return DONE;
}

//����������� ����� � ������� �� �����
//1:��������� �� ������ � ������� ����� �����
//2:������ � ������ ��������
//���������� ��������� �� ������ �������� � �������� ������
Cmx_obj_t* find_values(Cmx_obj_t* source_obj, const char* key)
{
	Value_t mean = { J_NULL, NULL, 0 };
	Cmx_obj_t* obj = make_cmxobj(ARR, mean);

	for (uint16_t idx = 0; idx < source_obj->valCnt; ++idx)
	{
		if (source_obj->value[idx].key == key)
		{
			add_value(obj, source_obj->value[idx]);
		}

		if (source_obj->value[idx].type == CMX_OBJ)
		{
			Cmx_obj_t* tmp_obj = find_values(&source_obj->value[idx].var_value.cmx_obj, key);
			for (uint16_t j = 0; j < tmp_obj->valCnt; ++j)
				add_value(obj, tmp_obj->value[j]);
		}
	}

	return obj;
}

//����� � ������� �� �����
//1:��������� �� ������ � ������� ����� �����
//2:������ � ������ ��������
//���������� ��������� �� ��������
Value_t* find_value(Cmx_obj_t* cmx_obj, const char* key)
{
	Value_t mean = { J_NULL, NULL, 0 };

	for (uint16_t idx = 0; idx < cmx_obj->valCnt; ++idx)
	{
		if (cmx_obj->value[idx].key == key)
		{
			return &cmx_obj->value[idx];
		}
	}
	return NULL;
}

static uint16_t dynamic_arr_sz = 256;
static uint16_t idx = 0;
uint8_t* msg_arr = NULL;
//������� ��� ��������� ������ ��� ���������
//���������� true ��� false
bool init_msgarr()
{
	msg_arr = (uint8_t*)malloc(dynamic_arr_sz);
	if (msg_arr)
		return true;
	
	return false;
}

//��������������� ������� ���������� �������� � ������
//1:�������� ��� ��������� � ������
void add_ellement(uint8_t ellem)
{
	if(idx < dynamic_arr_sz)
		msg_arr[idx++] = ellem;
	else
	{
		dynamic_arr_sz += 256;
		uint8_t* tmp_arr = (uint8_t*)malloc(dynamic_arr_sz);
		for (uint16_t j = 0; j < idx; ++j)
			tmp_arr[j] = msg_arr[j];
		msg_arr = tmp_arr;
	}
}

//������� ��� ����� ��������� � ������ ��� ��������
//��������� ��������� �� ����� ������� ����� ������������
//��������� ���������� � ������������ ������ msg_arr
//������� ��� ������ ��� ������ ������� init_msgarr()
void make_msg(Cmx_obj_t* cmx_obj)
{
	add_ellement(cmx_obj->valCnt);			//���������� �������� � �������
	add_ellement(cmx_obj->type);			//��� ���������� �������	

	for (uint8_t j = 0; j < cmx_obj->valCnt; ++j)	//������ ��������
	{
		add_ellement(cmx_obj->value[j].type);
		if (cmx_obj->value[j].key != NULL)
		{
			uint8_t k = 0;
			while (cmx_obj->value[j].key[k] != '\0')	//���� ��������
			{
				add_ellement(cmx_obj->value[j].key[k++]);
			}
			add_ellement('\0');
		}
		else
			add_ellement(NULL);

		if (cmx_obj->value[j].type == CMX_OBJ)
		{
			make_msg(&cmx_obj->value[j].var_value.cmx_obj);		//���� �������� ������ �� ������ � ��������
		}
		else if (cmx_obj->value[j].type == STRING)
		{
			{
				uint8_t k = 0;
				while (cmx_obj->value[j].var_value.string[k] != '\0')	//�������� ���� STRING
				{
					add_ellement(cmx_obj->value[j].var_value.string[k++]);//�������� ������ � ������
				}
				add_ellement('\0');//����� ������
			}
		}
		else
		{
			if (cmx_obj->value[j].type == NUMBER)
			{
				uint8_t* simple_val = (uint8_t*)&cmx_obj->value[j].var_value;
				for (uint8_t k = 0; k < sizeof(int); ++k)
				{
					add_ellement(simple_val[k]);//�������� �� ������  
				}
			}
			else if (cmx_obj->value[j].type == REAL_NUMBER)
			{
				uint8_t* simple_val = (uint8_t*)&cmx_obj->value[j].var_value;
				for (uint8_t k = 0; k < sizeof(float); ++k)
				{
					add_ellement(simple_val[k]);//�������� �� ������ 
				}
			}
			else
			{
				uint8_t* simple_val = (uint8_t*)&cmx_obj->value[j].var_value;
				add_ellement(*simple_val);//BOOLEAN � J_NULL �������� ���� ����
			}
		}
	}
}

//������� ���������� ������� �� ���������
//1:��������� �� ������ � ����������
//���������� ��������� �� ������
Cmx_obj_t* extract_msg(uint8_t* msg)
{
	static uint16_t j = 0;
	Cmx_obj_t* cmx_obj = NULL;
	Var_value_t var_value = {};
	Value_t value = { J_NULL};

	if(msg[j + 1] == OBJ)
		cmx_obj = make_cmxobj(OBJ, value);
	else
		cmx_obj = make_cmxobj(ARR, value);

	cmx_obj->valCnt = msg[j++];						//���������� �������� �������

	cmx_obj->type = (Type_cmxobj)msg[j++];			//��� ���������� �������

	//�������� ������ ��� ��������
	cmx_obj->value = (Value_t*)malloc(sizeof(Value_t) * cmx_obj->valCnt);
	for (uint8_t idx = 0; idx < cmx_obj->valCnt; ++idx)
	{
		cmx_obj->value[idx].type = (Type_JObj)msg[j++];		//��� ��������

		if (cmx_obj->type == OBJ)
		{		//�������� ���� ��������
			cmx_obj->value[idx].key = (const char*)msg + j;
			while (msg[j] != '\0')
			{
				j++;
			}
			j++;	//��������� ���������� ������������� ������� � j
		}
		else
		{
			cmx_obj->value[idx].key = NULL;
			j++;
		}
			

		if (cmx_obj->value[idx].type == CMX_OBJ)
		{
			cmx_obj->value[idx].var_value.cmx_obj = *extract_msg(msg);

		}
		else if(cmx_obj->value[idx].type == STRING)
		{
			{		//�������� ������
				cmx_obj->value[idx].var_value.string = (const char*)msg + j;
				while (msg[j] != '\0')//��������� ���������� ������������� ������� � j
				{
					j++;
				}
				j++;	
			}
		}
		else
		{
			if (cmx_obj->value[idx].type == NUMBER)
			{
				uint8_t* simple_val = (uint8_t*)&cmx_obj->value[idx].var_value;
				for (uint8_t k = 0; k < sizeof(int); ++k)
				{
					simple_val[k] = msg[j++];
				}
			}
			else if (cmx_obj->value[idx].type == REAL_NUMBER)
			{
				uint8_t* simple_val = (uint8_t*)&cmx_obj->value[idx].var_value;
				for (uint8_t k = 0; k < sizeof(float); ++k)
				{
					simple_val[k] = msg[j++];
				}
			}
			else
			{
				uint8_t* simple_val = (uint8_t*)&cmx_obj->value[idx].var_value;
				*simple_val = msg[j++];			//BOOLEAN � J_NULL �������� ���� ����
			}
		}
	}	

	return cmx_obj;
}

//������� ��� ������������ �������� �������� ������� ��� �������
//1:��������� ��������� �� ������ �������� �������� ����� ����������
void free_value(Cmx_obj_t* cmx_obj)
{
	if (cmx_obj)
	{
		if (cmx_obj->valCnt)
		{
			for (int idx = cmx_obj->valCnt - 1; idx >= 0 ; --idx)
			{
				if (cmx_obj->value[idx].type == CMX_OBJ)
					free_value(&cmx_obj->value[idx].var_value.cmx_obj);
			}
			if (cmx_obj->value)
			{
				free(cmx_obj->value);
				/*cmx_obj->value = NULL;*/
			}
				
			
	//		free(cmx_obj);
		}
	}
}

//������� ��� ������������ ������ ���������� ��� ������ ��� ������
//��������� ��������� �� ������ ������� �������������
void free_obj(Cmx_obj_t* cmx_obj)
{
	free_value(cmx_obj); 
	if (cmx_obj)
		free(cmx_obj);


	
}
