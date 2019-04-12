#include "analyze/algorithm/movement/MovementDetector.cpp"
#include "common/Logger.hpp"


bool checkTwoObjectsMovements()
{/*
	using namespace am::analyze::algorithm;
	//	|x|
	//	|+|x|
	// o1 & o2: |+|   o2: |x|
	std::vector<Pixel>v1{ {1, 1} };
	std::vector<Pixel>v2{ { 1,1 },{ 1,2 },{ 2,1 } };

	Object o1(v1);
	Object o2(v2);

	
	std::multiset<Object, comparators::Descending> m1, m2;
	m1.emplace(o1);
	m2.emplace(o2);

	std::shared_ptr<am::common::Logger> logger(new am::common::Logger());
	 // to be updated
	auto result = movement::getMovementsFromRects(m1, m2, logger);

	if (result[0].mov[movement::MovementType::RIGHT] == true && result[0].mov[movement::MovementType::TOP] == true
		&& result[0].mov[movement::MovementType::LEFT] == false && result[0].mov[movement::MovementType::BOTTOM] == false)
		return true;
		*/
	return false;
}


int main()
{
	if (!checkTwoObjectsMovements())
		printf("checkTwoObjectsMovements failed: UT failed\n");
	else
		printf("MovementDetector UT passed\n");

	return 0;
}