// SoftwareRendererTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <gtest\gtest.h>
#include <graphtypes.h>

TEST(graphtypetests, vector_arithmetic_tests) {
	Vec3<int> from(5, 8, 3);
	Vec3<int> to(10, 12, 3);
	Vec3<int> result = to - from;

	ASSERT_EQ(result.x(), 5);
	ASSERT_EQ(result.y(), 4);
	ASSERT_EQ(result.z(), 0);

	result = from.translate(2, 4, 3);
	ASSERT_EQ(result.x(), 7);
	ASSERT_EQ(result.y(), 12);
	ASSERT_EQ(result.z(), 6);
}

int _tmain(int argc, _TCHAR* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

