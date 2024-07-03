
#include "kon/debug/Test.hpp"
#include "kon/types/String.hpp"
#include "kon/types/ArrayList.hpp"

#include <kon/resource/Resource.hpp>
#include <kon/resource/ResourceModel.hpp>

void TestResources();

int main(int argc, char const *argv[])
{
	TestResources();

    return 0; //(new CoreUnitTest)->run();
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

