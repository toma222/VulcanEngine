
#include "kon/core/Logging.hpp"
#include "kon/debug/Test.hpp"
#include "kon/scene/ComponentList.hpp"
#include "kon/scene/Components.hpp"
#include "kon/scene/Entity.hpp"
#include "kon/scene/Scene.hpp"
#include "kon/types/String.hpp"
#include "kon/types/ArrayList.hpp"
#include "kon/types/UUID.hpp"

#include <kon/resource/Resource.hpp>
#include <kon/resource/ResourceModel.hpp>

void TestResources();
void TestMaps();

struct TestComponent
{
	TestComponent() = default;
	TestComponent(int _i)
		: i(_i) {}

	int i{0};
};

int main(int argc, char const *argv[])
{
	TestMaps();
	TestResources();
}

void TestResources()
{
	using namespace kon;

	KonCoreTest<int>("Load model", 11484, 
	[](){
		ResourceModel model;
		model.LoadResource("models/viking_room.obj");
		return model.GetShape()->indicies.Index(); 
	});
}

#include <kon/types/Map.hpp>


void TestMaps()
{
	using namespace kon;

	KonCoreTest<String>("AppendString", "Hello World", 
	[](){
		String a("Hello");
		String b(" World");
		String concat = a.AppendString(b);
		return concat;
	});

	KonCoreTest<String>("Substring", "Hello", 
	[](){
		String a("HelloWorld");
		String concat = a.SubString(0, 5);
		return concat;
	});

	KonCoreTest<int>("Index of char", 4, 
	[](){
		String a("Hello");

		return a.IndexOfChar('o');
	});

	KonCoreTest<int>("Index of string", 6, 
	[](){
		String a("Hello World");

		return a.IndexOfString("World");
	});

	KonCoreTest<bool>("HasDuplicate", true, 
	[](){
		UnorderedMap<String, u32> map;
		String s("hows it going"); // stack
		map.Enter(s, s.Hash());
		// map.Enter(String("what up")); // temp
		return map.HasDuplicate(String("hows it going").Hash());
	});

	KonCoreTest<bool>("HasDuplicate(inv)", true, 
	[](){
		UnorderedMap<String, u32> map;
		String s("hows it going"); // stack
		map.Enter(s, s.Hash());
		// map.Enter(String("what up")); // temp
		return map.HasDuplicate(String("im not in the list").Hash());
	});

	HashMap<int, u64> map;
	UUID n;

	KonCoreTest<int>("Better Hash Map Emplace 10000", 4,
	[&]() {
		for(int i = 0; i < 10000; i++)
		{
			map.Emplace(i, UUID().uuid);
		}

		return 4;
	});

	KonCoreTest<int>("Better Hash Map Emplace 1", 4,
	[&]() {
		map.Emplace(101, n.uuid);
		return 4;
	});

	KonCoreTest<int>("Better Hash Map Get", 101,
	[&]() {
		return map.Get(n.uuid);
	});

	KonCoreTest<int>("Better Hash Map Erase", 4,
	[&]() {
		// UUID id;
		// map.Emplace(4, id.uuid);
		map.Remove(n.uuid);

		return 4; //map.Get(id.uuid);
	});
}

