
#include "kon/debug/Test.hpp"
#include "kon/types/String.hpp"
#include "kon/types/ArrayList.hpp"

#include <kon/resource/Resource.hpp>
#include <kon/resource/ResourceModel.hpp>

void TestResources();
void TestMaps();

int main(int argc, char const *argv[])
{
	TestMaps();
	TestResources();

    return 0;
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

	KonCoreTest<bool>("Enter", true, 
	[](){
		UnorderedMap<String> map;
		String s("hows it going"); // stack
		map.Enter(s);
		return map.Enter(String("what up")); // temp
	});

	KonCoreTest<String>("Get", "hows it going", 
	[](){
		UnorderedMap<String> map;
		String s("hows it going"); // stack
		map.Enter(s);
		map.Enter(String("what up")); // temp
		return map.Get(s.Hash());
	});

	KonCoreTest<bool>("HasDuplicate", true, 
	[](){
		UnorderedMap<String> map;
		String s("hows it going"); // stack
		map.Enter(s);
		map.Enter(String("what up")); // temp
		return map.HasDuplicate(String("hows it going"));
	});

	KonCoreTest<bool>("HasDuplicate(inv)", true, 
	[](){
		UnorderedMap<String> map;
		String s("hows it going"); // stack
		map.Enter(s);
		map.Enter(String("what up")); // temp
		return map.HasDuplicate(String("im not in the list"));
	});

	KonCoreTest<int>("Better Hash Map", 4,
	[](){
		BetterHashMap<int, u32> map;
		map.Emplace(2, 234523452);
		map.Emplace(5, 121245222);
		map.Emplace(4, 504943233);
		//map.Remove(504943233);
		map.Emplace(10,680998661);
		map.Emplace(1, 387844444);
		// map.Remove(504943233);
		return map.Get(504943233);
	});
}

