#include <gtest/gtest.h>
#include "parray.hpp"
#include "datapar.hpp"
#include <vector>

TEST(testing, test){
	pasl::pctl::parray<int> const& arr(1);
	ASSERT_EQ(1, 1);
}
