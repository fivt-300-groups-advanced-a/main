#include <typeinfo>
#include <iostream>
#include <fstream>
#include <set>

#include "io/inputstreamreader.h"
#include "io/outputstreamwriter.h"

#include "sorters/standartsorter.h"

#include "externalfilesorter.h"

#include <gtest/gtest.h>

int main(int argc, char **argv)
{
	InputStreamReader<int> reader;
	OutputStreamWriter<int> writer;
	ExternalFileSorter<int> sorter;

	sorter.sort(100, reader, writer, StandartSorter<int>(), std::less<int>());
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
