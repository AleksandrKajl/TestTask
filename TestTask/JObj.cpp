#include "JObj.h"

//Функция для создания объекта или массива
//1:Тип обекта, 2:Первое значение с которым создасться объект,
//если тип значения J_NULL то создасться пустой объект
//Возвращает указатель на созданный объект
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

//Функция добавления значения в объект
//1:указатель на объект в который добавляется значение,
//2:Значение описываемое структурой Value_t
//В случае успеха возвращает ноль, иначе код ошибки
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

	//Если значения добавляется в массив, то ключ не нужен
	if (cmx_obj->type == ARR)
		meaning.key = NULL;

	tmpMeaning[cmx_obj->valCnt - 1] = meaning;

	cmx_obj->value = tmpMeaning;

	return DONE;
}

//Рекурсивный поиск в объекте по ключу
//1:Указатель на объект в котором будет поиск
//2:строку с ключём значения
//Возвращает указатель на объект значений с заданным ключом
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

//Поиск в объекте по ключу
//1:Указатель на объект в котором будет поиск
//2:строку с ключём значения
//Возвращает указатель на значение
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
//Функция для выделения памяти под сообщение
//Возвращает true или false
bool init_msgarr()
{
	msg_arr = (uint8_t*)malloc(dynamic_arr_sz);
	if (msg_arr)
		return true;
	
	return false;
}

//Вспомогательная функция добовления значения в массив
//1:Значение для занесения в массив
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

//Функция для сбора сообщения в массив для передачи
//Принимает указатель на обект который будет передоваться
//Сообщение собирается в динамический массив msg_arr
//Который был создан при вызове функции init_msgarr()
void make_msg(Cmx_obj_t* cmx_obj)
{
	add_ellement(cmx_obj->valCnt);			//Количество значений в объекте
	add_ellement(cmx_obj->type);			//Тип составного объекта	

	for (uint8_t j = 0; j < cmx_obj->valCnt; ++j)	//Массив значений
	{
		add_ellement(cmx_obj->value[j].type);
		if (cmx_obj->value[j].key != NULL)
		{
			uint8_t k = 0;
			while (cmx_obj->value[j].key[k] != '\0')	//Ключ значения
			{
				add_ellement(cmx_obj->value[j].key[k++]);
			}
			add_ellement('\0');
		}
		else
			add_ellement(NULL);

		if (cmx_obj->value[j].type == CMX_OBJ)
		{
			make_msg(&cmx_obj->value[j].var_value.cmx_obj);		//Если составно объект то входим в рекурсию
		}
		else if (cmx_obj->value[j].type == STRING)
		{
			{
				uint8_t k = 0;
				while (cmx_obj->value[j].var_value.string[k] != '\0')	//Значение типа STRING
				{
					add_ellement(cmx_obj->value[j].var_value.string[k++]);//Копируем строку в массив
				}
				add_ellement('\0');//Конец строки
			}
		}
		else
		{
			if (cmx_obj->value[j].type == NUMBER)
			{
				uint8_t* simple_val = (uint8_t*)&cmx_obj->value[j].var_value;
				for (uint8_t k = 0; k < sizeof(int); ++k)
				{
					add_ellement(simple_val[k]);//Копируем по байтно  
				}
			}
			else if (cmx_obj->value[j].type == REAL_NUMBER)
			{
				uint8_t* simple_val = (uint8_t*)&cmx_obj->value[j].var_value;
				for (uint8_t k = 0; k < sizeof(float); ++k)
				{
					add_ellement(simple_val[k]);//Копируем по байтно 
				}
			}
			else
			{
				uint8_t* simple_val = (uint8_t*)&cmx_obj->value[j].var_value;
				add_ellement(*simple_val);//BOOLEAN и J_NULL занимают один байт
			}
		}
	}
}

//Функция извлечения объекта из сообщения
//1:Указатель на массив с сообщением
//Возвращает указатель на объект
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

	cmx_obj->valCnt = msg[j++];						//Количество значений объекта

	cmx_obj->type = (Type_cmxobj)msg[j++];			//Тип составного объекта

	//Выделяем память под значения
	cmx_obj->value = (Value_t*)malloc(sizeof(Value_t) * cmx_obj->valCnt);
	for (uint8_t idx = 0; idx < cmx_obj->valCnt; ++idx)
	{
		cmx_obj->value[idx].type = (Type_JObj)msg[j++];		//Тип значения

		if (cmx_obj->type == OBJ)
		{		//Копируем ключ значения
			cmx_obj->value[idx].key = (const char*)msg + j;
			while (msg[j] != '\0')
			{
				j++;
			}
			j++;	//Добавляем количество скопированных символа в j
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
			{		//Копируем строку
				cmx_obj->value[idx].var_value.string = (const char*)msg + j;
				while (msg[j] != '\0')//Добавляем количество скопированных символа в j
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
				*simple_val = msg[j++];			//BOOLEAN и J_NULL занимают один байт
			}
		}
	}	

	return cmx_obj;
}

//Функция для рекурсивного удаления значений объекта или массива
//1:Принимает указатель на объект значения которого нужно освободить
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

//Функция для освобождения памяти выделенной под объект или массив
//Принимает указатель на объект который освобождается
void free_obj(Cmx_obj_t* cmx_obj)
{
	free_value(cmx_obj); 
	if (cmx_obj)
		free(cmx_obj);


	
}
