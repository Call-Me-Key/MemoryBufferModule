// Semaphore.cpp : Defines the entry point for the application.
//
#include <iostream>
#include <iterator>
#include <tuple>
#include <array>
#include <exception>

#include "MemoryBuffer.h"

/*======Контекст======
Сервер отправляет данные в сыром виде.
В начале пакета всегда есть размер 
пакета в uint32_t и другие спецификаторы 
пакета.
Возможны вложенные пакеты.
Хотелось уменьшить частоту использования
С-поинтеров при работе с пакетами, но оставить
возможность для других классов свободно
дессериализовывать сырые данные.
========Контекст======*/

///Примерная маркировка пакетов
enum Pack_type : uint32_t
{
	null = 0,
	transform = 1,
	identify = 2
};

///Примерная структура пакета
template<typename T>
struct ExampleStruct
{
	ExampleStruct(T&& inp_data) : data(std::move(inp_data)) {};
	const uint32_t size = sizeof(*this);
	T data;
};

///Пример фабрики для пакетов не требующих сохранения последовательности
template<typename... Args>
constexpr auto ExampleFactory(Args... args)
{
	return ExampleStruct<std::tuple<Args...>>(std::tuple(std::move(args)...));
}

//Примерная структура трансформа объектта
struct ExampleTransform
{
	ExampleTransform(float inp_x, float inp_y) : x(inp_x), y(inp_y) {};
	const Pack_type type = Pack_type::transform;
	float x = 0.0f;
	float y = 0.0f;
};
//Примерная структура идентификации
struct ExampleIdentify
{
	ExampleIdentify(uint32_t inp_id) : id(inp_id) {};
	const Pack_type type = Pack_type::identify;
	uint32_t id = 0;
};

template<typename T>
auto GetDataStructure(std::byte* target)
{
	return reinterpret_cast<T*>(target);
}

int main()
{
	float x_1 = 10.f, y_1 = 15;
	uint32_t id_1 = 1;

	float x_2 = 20.f, y_2 = 25;
	uint32_t id_2 = 2;

	std::cout << "serialization into byte array emulation: " << std::endl <<
		"id: " << id_1 << " position: " << x_1 << " : " << y_1 
		<< std::endl <<
		"id: " << id_2 << " position: " << x_2 << " : " << y_2
		<< std::endl;

	auto object_pack_1 = ExampleFactory(ExampleStruct(ExampleTransform(x_1, y_1)), 
		ExampleStruct(ExampleIdentify(id_1)));
	auto object_pack_2 = ExampleFactory(ExampleStruct(ExampleTransform(x_2, y_2)), 
		ExampleStruct(ExampleIdentify(id_2)));

	MemoryBuffer mem;

	mem.push_back(object_pack_1);
	mem.push_back(object_pack_2);

	std::cout << std::endl << "deserialization from byte array emulation: " << std::endl;
	for (auto it = std::begin(mem); it != std::end(mem); it++)
	{
		std::cout << "object at " << it->first << std::endl;
		for (size_t i = 0; i < 2; i++)
		{
			///эмулирование десереализации пакетов другими классами объекта
			auto neted_it = it[i];
			if (*reinterpret_cast<Pack_type*>(neted_it.data()) == Pack_type::identify)
			{
				std::cout << "Id: " 
					<< GetDataStructure<ExampleIdentify>(neted_it.data())->id << std::endl;
			}
			else if (*reinterpret_cast<Pack_type*>(neted_it.data()) == Pack_type::transform)
			{
				std::cout << "Position: " <<
					GetDataStructure<ExampleTransform>(neted_it.data())->x << " : " <<
					GetDataStructure<ExampleTransform>(neted_it.data())->y << std::endl;
			}
		}
	}
	return 0;
}
